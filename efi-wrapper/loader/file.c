#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include"efi/efi_file.h"
#include"defines.h"

efi_file*efi_file_new(efi_run_context*ctx){
	efi_file*file;
	mem_pool*pool;
	if(!ctx||!(pool=ctx->mem[efi_bs_data].pool))return NULL;
	if(!(file=mem_allocate(pool,sizeof(efi_file))))return NULL;
	memset(file,0,sizeof(efi_file));
	file->pool=pool,file->context=ctx;
	#ifdef ELF_FOUND
	file->elf_dbg_fd=-1;
	#endif
	return file;
}

static void efi_update_name(efi_file*file){
	char*p;
	size_t cnt;
	if((cnt=strlen(file->path))>0){
		for(size_t i=0;i<cnt;i++)if(file->path[i]=='\\')file->path[i]='/';
		for(size_t i=cnt-1;file->path[i]==' ';i--)file->path[i]=0;
		for(size_t i=cnt-1;file->path[i]=='/';i--)file->path[i]=0;
		strncpy(file->name,(p=strrchr(file->path,'/'))?p+1:file->path,sizeof(file->name)-1);
	}
}

bool efi_load_by_data(efi_file*file,void*data,size_t length){
	bool have_path=false,have_name=false;
	if(!file||!data||length<=0x40)return false;
	if(file->data||file->length>0)return false;
	file->data=data,file->length=length;
	if(!file->path[0])snprintf(
		file->path,sizeof(file->path)-1,
		"memory@0x%zx",(size_t)data
	),have_path=true;
	if(!file->name[0])efi_update_name(file),have_name=true;
	if(file->name[0])xlog(
		LOG_INFO,"Loading efi %s (%s)",
		file->name,file->path
	);
	if(!efi_parse(file))
		EDONE(xerror("parse efi failed"));
	if(!efi_load_data(file))
		EDONE(xerror("load efi failed"));
	return file;
	done:
	if(have_name)memset(file->name,0,sizeof(file->name));
	if(have_path)memset(file->path,0,sizeof(file->path));
	errno=ENOEXEC;
	return false;
}

bool efi_load_by_dup_data(efi_file*file,void*data,size_t length){
	void*dup;
	if(!file||!data||length<=0x40)return false;
	if(!(dup=mem_allocate(file->pool,length)))return false;
	memcpy(dup,data,length);
	return efi_load_by_data(file,dup,length);
}

bool efi_load_by_fd(efi_file*file,int fd){
	void*data;
	char lp[256];
	struct stat64 st;
	bool ret,have_path=false,have_name=false;
	if(!file)return false;
	if(fstat64(fd,&st)<0)return false;
	if(st.st_size<=0){
		xerror("file %d invalid size",fd);
		errno=EINVAL;
		return false;
	}
	data=mmap(
		NULL,st.st_size,PROT_READ,
		MAP_PRIVATE|MAP_DENYWRITE,
		fd,0
	);
	if(data==NULL||data==MAP_FAILED){
		xerror(
			"mmap %d size %zu bytes failed: %m",
			fd,(size_t)st.st_size
		);
		return false;
	}
	xlog(
		LOG_INFO,"Map %d size %zu bytes to 0x%012zx - 0x%012zx",
		fd,(size_t)st.st_size,(size_t)data,(size_t)data+(size_t)st.st_size
	);
	if(!file->path[0]){
		memset(lp,0,sizeof(lp));
		snprintf(lp,sizeof(lp)-1,"/proc/self/fd/%d",fd);
		readlink(lp,file->path,sizeof(file->path));
		have_path=true;
	}
	if(!file->name[0])efi_update_name(file),have_name=true;
	if(!(ret=efi_load_by_data(file,data,st.st_size))){
		if(have_name)memset(file->name,0,sizeof(file->name));
		if(have_path)memset(file->path,0,sizeof(file->path));
	}
	return ret;
}

bool efi_load_by_file(efi_file*file,FILE*f){
	return efi_load_by_fd(file,fileno(f));
}

static void try_load(efi_file*file,int cfd,const char*path){
	size_t len;
	char*p,buff[4096];
	memset(buff,0,sizeof(buff));
	strncpy(buff,path,sizeof(buff)-1);
	if(!(p=strchr(buff,'.')))p=buff+strlen(path);
	len=p-buff;
	strncpy(p,".debug",sizeof(buff)-len);
	if(efi_load_elf_debug(file,cfd,buff))return;
	strncpy(p,".so",sizeof(buff)-len);
	if(efi_load_elf_debug(file,cfd,buff))return;
	strncpy(p,".elf",sizeof(buff)-len);
	if(efi_load_elf_debug(file,cfd,buff))return;
}

bool efi_load_by_at_path(efi_file*file,int cfd,const char*path){
	int fd;
	bool ret,have_path=false,have_name=false;
	if(!file)return NULL;
	fd=openat(cfd,path,O_RDONLY|O_CLOEXEC);
	if(fd<0){
		xerror("openat %s from %d failed: %m",path,cfd);
		return NULL;
	}
	if(cfd!=AT_FDCWD)xlog(LOG_INFO,"Open %s as %d",path,fd);
	else xlog(LOG_INFO,"Open %s from %d as %d",path,cfd,fd);
	if(!file->path[0])strncpy(
		file->path,path,sizeof(file->path)-1
	),have_path=true;
	if(!file->name[0])efi_update_name(file),have_name=true;
	if(!(ret=efi_load_by_fd(file,fd))){
		if(have_name)memset(file->name,0,sizeof(file->name));
		if(have_path)memset(file->path,0,sizeof(file->path));
	}else try_load(file,cfd,path);
	close(fd);
	return ret;
}

bool efi_load_by_path(efi_file*file,const char*path){
	return efi_load_by_at_path(file,AT_FDCWD,path);
}
