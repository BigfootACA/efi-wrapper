//
// Created by bigfoot on 2023/5/8.
//

#define _GNU_SOURCE
#include"internal.h"
#include"path.h"
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/statfs.h>
#define PROTO_FS_MAGIC (efi_guid){0xc868a1ac,0x1eec,0x4a25,{0xbb,0x47,0xd5,0x0b,0xfc,0xc3,0xab,0xcb}}
#define PROTO_FILE_MAGIC (efi_guid){0x4de03048,0xa40f,0x4516,{0x9a,0x39,0x0d,0x3a,0xe4,0x30,0x6a,0x17}}

typedef struct efi_file_protocol_private_posix_fd efi_file_protocol_private_posix_fd;
typedef struct efi_simple_file_system_protocol_private_posix_fd efi_simple_file_system_protocol_private_posix_fd;

struct efi_simple_file_system_protocol_private_posix_fd{
	efi_simple_file_system_protocol_private data;
	char16 name[256];
	bool writable;
	int fd;
};

struct efi_file_protocol_private_posix_fd{
	efi_file_protocol_private data;
	efi_simple_file_system_protocol_private_posix_fd*fs;
	efi_file_protocol_private_posix_fd*parent;
	char16 name[256];
	uint64_t attrs;
	uint64_t mode;
	bool folder;
	char path[4096];
	DIR*dir;
	int refcnt;
	int fd;
};
static efi_simple_file_system_protocol fs_proto;
static efi_file_protocol file_proto;

static struct fs_device_path{
	vendor_device_path vendor;
	efi_device_path end;
}dp={
	{
		{
			HARDWARE_DEVICE_PATH,
			HW_VENDOR_DP,
			(uint16_t)(sizeof(vendor_device_path)),
		},
		PROTO_FS_MAGIC
	},{
		END_DEVICE_PATH_TYPE,
		END_ENTIRE_DEVICE_PATH_SUBTYPE,
		(uint16_t)sizeof(efi_device_path),
	}
};

static efiapi efi_status efi_fd_file_get_driver_name(
	efi_component_name_protocol *proto,
	char8                       *lang,
	char16                      **name
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str8,lang),
		call_arg(str16,name),
		NULL
	);
	static char16*def_name=(char16*)L"Posix File System";
	if(name)*name=def_name;
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_get_controller_name(
	efi_component_name_protocol *proto,
	efi_handle                  controller,
	efi_handle                  child,
	char8                       *lang,
	char16                      **name
){
	log_call(
		call_arg(ptr,proto),
		call_arg(handle,controller),
		call_arg(handle,child),
		call_arg(str8,lang),
		call_arg(str16,name),
		NULL
	);
	static char16*def_name=(char16*)L"Posix File System";
	if(name)*name=def_name;
	log_return(efi_success);
}

static efi_file_protocol_private_posix_fd*new_file(efi_run_context*ctx){
	efi_file_protocol_private_posix_fd*f;
	if(!(f=mem_allocate(
		ctx->mem[efi_bs_data].pool,
		sizeof(efi_file_protocol_private_posix_fd)
	)))return NULL;
	memset(f,0,sizeof(efi_file_protocol_private_posix_fd));
	memcpy(&f->data.protocol,&file_proto,sizeof(file_proto));
	memcpy(&f->data.magic,&PROTO_FILE_MAGIC,sizeof(efi_guid));
	return f;
}

static efiapi efi_status efi_fd_file_open(
	efi_file_protocol*proto,
	efi_file_protocol**handle,
	char16*file,
	uint64_t mode,
	uint64_t attrs
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		call_arg(str16,file),
		call_arg(hex,mode),
		call_arg(hex,attrs),
		NULL
	);
	DIR*e=NULL;
	struct stat st;
	char buff[4096],resolve[4096],*path;
	int flags=0,perm=S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH,fd=-1,ret;
	efi_file_protocol_private_posix_fd*nf=NULL,*p=
		(efi_file_protocol_private_posix_fd*)proto;
	bool r=(mode&EFI_FILE_MODE_READ)==EFI_FILE_MODE_READ;
	bool w=(mode&EFI_FILE_MODE_WRITE)==EFI_FILE_MODE_WRITE;
	bool c=(mode&EFI_FILE_MODE_CREATE)==EFI_FILE_MODE_CREATE;
	bool o=(attrs&EFI_FILE_READ_ONLY)==EFI_FILE_READ_ONLY;
	bool d=(attrs&EFI_FILE_DIRECTORY)==EFI_FILE_DIRECTORY;
	if(handle)*handle=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!handle||!file||!*file||!p->folder)log_return(efi_invalid_parameter);
	if(p->fs->writable){
		if(r&&w)flags|=O_RDWR;
		else if(r)flags|=O_RDONLY;
		else if(w)flags|=O_WRONLY;
	}else if(r)flags|=O_RDONLY;
	else log_return(efi_write_protected);
	if(o)perm&=~(S_IWUSR);
	if(d)flags|=O_DIRECTORY,perm|=S_IXUSR|S_IXGRP|S_IXOTH;
	if(ustr2astr(buff,sizeof(buff),file)<=0||!buff[0]){
		xerror("convert path to UTF-8 failed");
		log_return(efi_invalid_parameter);
	}
	path_merge(resolve,sizeof(resolve),'/',p->path,buff);
	path=resolve[0]?resolve:".";
	xlog(LOG_CALL,"resolved path: %s",path);
	if(strcmp(resolve,"..")==0||strncmp(resolve,"../",3)==0)
		log_return(efi_not_found);
	errno=0;
	if(faccessat(p->fs->fd,path,F_OK,0)!=0&&errno==ENOENT){
		if(!c)log_return(efi_not_found);
		if(!p->fs->writable)log_return(efi_write_protected);
		if((ret=(d?
			mkdirat(p->fs->fd,path,perm):
			openat(p->fs->fd,path,flags|O_CREAT,perm)
		))<0){
			xerror("new at %d %s with %x failed: %m",p->fs->fd,path,flags);
			log_return(efi_device_error);
		}
		if(d)fd=ret;
	}
	if(fd<0&&(fd=openat(p->fs->fd,path,flags))<0){
		xerror("open at %d %s with %x failed: %m",p->fs->fd,path,flags);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	if(fstat(fd,&st)!=0){
		xerror("fstat %d failed: %m",fd);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}else if(S_ISDIR(st.st_mode))d=true;
	if(d&&!(e=fdopendir(fd))){
		xerror("fdopendir %d failed: %m",fd);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	if(!(nf=new_file(efi_current_ctx))){
		if(e)closedir(e);
		else close(fd);
		log_return(efi_out_of_resources);
	}
	strncpy(nf->path,resolve,sizeof(nf->path)-1);
	if(path_basename(buff,sizeof(buff),resolve))
		astr2ustr(nf->name,sizeof(nf->name),buff);
	nf->mode=mode,nf->attrs=attrs;
	nf->folder=d,nf->fd=fd,nf->dir=e;
	nf->parent=p,nf->fs=p->fs;
	nf->refcnt=1,p->refcnt++;
	*handle=&nf->data.protocol;
	xlog(LOG_CALL,"out handle=%p",*handle);
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_close(
	efi_file_protocol*proto
){
	log_call(
		call_arg(ptr,proto),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	p->refcnt--;
	if(p->refcnt>0)log_return(efi_success);
	if(p->parent)efi_fd_file_close(&p->parent->data.protocol);
	if(p->folder)closedir(p->dir);
	else close(p->fd);
	memset(proto,0,sizeof(efi_file_protocol));
	mem_free(proto);
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_delete(
	efi_file_protocol*proto
){
	log_call(
		call_arg(ptr,proto),
		NULL
	);
	int fd,flags=0;
	efi_status st=efi_success;
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs->writable||!p->path[0])log_return(efi_write_protected);
	fd=p->fs?p->fs->fd:AT_FDCWD;
	if(p->folder)flags|=AT_REMOVEDIR;
	errno=0;
	if(unlinkat(fd,p->path,flags)<0){
		xerror("unlink at %d %s %d failed: %m",fd,p->path,flags);
		st=efi_warn_delete_failure;
	}
	efi_fd_file_close(proto);
	log_return(st);
}

static void efi_fd_file_fill_stat(struct stat*st,efi_file_info*info){
	info->attr=0;
	if(S_ISDIR(st->st_mode))info->attr|=EFI_FILE_DIRECTORY;
	else if(!S_ISREG(st->st_mode))info->attr|=EFI_FILE_SYSTEM;
	if((st->st_mode&S_IWUSR)!=S_IWUSR)info->attr|=EFI_FILE_READ_ONLY;
	efi_timespec_to_time(&st->st_atim,&info->atime);
	efi_timespec_to_time(&st->st_ctim,&info->ctime);
	efi_timespec_to_time(&st->st_mtim,&info->mtime);
	info->file_size=st->st_size;
	info->physical_size=st->st_size;
}

static efiapi efi_status efi_fd_file_read(
	efi_file_protocol*proto,
	uintn_t*size,
	void*buffer
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,size),
		call_arg(ptr,buffer),
		NULL
	);
	size_t ss;
	ssize_t ret;
	struct stat st;
	struct dirent*d;
	efi_file_info*info;
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!size)log_return(efi_invalid_parameter);
	if(p->folder){
		if(!p->dir)log_return(efi_not_ready);
		info=buffer;
		ss=sizeof(efi_file_info)+(256*sizeof(char16));
		if(*size<ss){
			*size=ss;
			log_return(efi_buffer_too_small);
		}
		if(!buffer)log_return(efi_invalid_parameter);
		memset(buffer,0,ss);
		info->size=ss;
		while(true){
			errno=0;
			if(!(d=readdir(p->dir))){
				if(errno==0){
					*size=0;
					break;
				}
				xerror("readdir %d failed: %m",p->fd);
				log_return(efi_errno_to_status(errno));
			}
			if(strcmp(d->d_name,".")==0)continue;
			if(strcmp(d->d_name,"..")==0)continue;
			if(fstatat(p->fd,d->d_name,&st,0)!=0){
				xerror("fstatat %d %s failed: %m",p->fd,d->d_name);
				log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
			}
			if(d->d_name[0]=='.')info->attr|=EFI_FILE_HIDDEN;
			if(d->d_type==DT_DIR)info->attr|=EFI_FILE_DIRECTORY;
			else if(d->d_type!=DT_REG)info->attr|=EFI_FILE_SYSTEM;
			xlog(LOG_CALL,"out file_name: %s",d->d_name);
			efi_fd_file_fill_stat(&st,info);
			astr2ustr(
				info->file_name,
				ss-offsetof(efi_file_info,file_name),
				d->d_name
			);
			break;
		}
	}else{
		if(!buffer)log_return(efi_invalid_parameter);
		ss=*size,*size=0,errno=0;
		if((ret=read(p->fd,buffer,ss))<0){
			xerror("read %d size %zu failed: %m",p->fd,ss);
			log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
		}
		*size=(size_t)ret;
	}
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_write(
	efi_file_protocol*proto,
	uintn_t*size,
	void*buffer
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,size),
		call_arg(ptr,buffer),
		NULL
	);
	size_t ss;
	ssize_t ret;
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs->writable)log_return(efi_write_protected);
	if(p->folder)log_return(efi_unsupported);
	if(!buffer)log_return(efi_invalid_parameter);
	ss=*size,*size=0,errno=0;
	if((ret=write(p->fd,buffer,ss))<0){
		xerror("write %d size %zu failed: %m",p->fd,ss);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	*size=(size_t)ret;
	log_return(efi_unsupported);
}

static efiapi efi_status efi_fd_file_set_position(
	efi_file_protocol*proto,
	uint64_t pos
){
	log_call(
		call_arg(ptr,proto),
		call_arg(unsigned,pos),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	errno=0;
	if(lseek64(p->fd,pos,SEEK_SET)<0){
		xerror("lseek %d set pos %llu failed: %m",p->fd,(unsigned long long)pos);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_get_position(
	efi_file_protocol*proto,
	uint64_t*pos
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pos),
		NULL
	);
	ssize_t ret;
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	errno=0;
	if((ret=lseek(p->fd,0,SEEK_CUR))<0){
		xerror("lseek %d cur failed: %m",p->fd);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	*pos=(size_t)ret;
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_get_info(
	efi_file_protocol *proto,
	const efi_guid    *type,
	uintn_t           *size,
	void              *buffer
){
	log_call(
		call_arg(ptr,proto),
		call_arg(guid,type),
		call_arg(ptr,size),
		call_arg(ptr,buffer),
		NULL
	);
	size_t min,cnt;
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!type||!size)log_return(efi_invalid_parameter);
	if(efi_guid_equals(type,&gEfiFileSystemVolumeLabelInfoIdGuid))
		min=ustrsize(p->fs->name);
	else if(efi_guid_equals(type,&gEfiFileSystemInfoGuid))
		min=sizeof(efi_file_system_info)+ustrsize(p->fs->name);
	else if(efi_guid_equals(type,&gEfiFileInfoGuid))
		min=sizeof(efi_file_info)+ustrsize(p->name);
	else log_return(efi_unsupported);
	if(min>*size){
		*size=min;
		log_return(efi_buffer_too_small);
	}
	if(!buffer)log_return(efi_buffer_too_small);
	memset(buffer,0,min);
	if(efi_guid_equals(type,&gEfiFileSystemVolumeLabelInfoIdGuid)){
		memcpy(buffer,p->fs->name,MIN(min,sizeof(p->fs->name)));
	}else if(efi_guid_equals(type,&gEfiFileSystemInfoGuid)){
		struct statfs st;
		efi_file_system_info*info=buffer;
		if((fstatfs(p->fd,&st))<0){
			xerror("fstatfs %d failed: %m",p->fd);
			log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
		}
		info->size=min;
		info->read_only=!p->fs->writable;
		info->block_size=st.f_bsize;
		info->free_space=st.f_bsize*st.f_bavail;
		info->vol_size=st.f_bsize*st.f_blocks;
		cnt=MIN(min-offsetof(efi_file_system_info,vol_label),sizeof(p->fs->name));
		memcpy(info->vol_label,p->fs->name,cnt);
	}else if(efi_guid_equals(type,&gEfiFileInfoGuid)){
		struct stat st;
		efi_file_info*info=buffer;
		if((fstat(p->fd,&st))<0){
			xerror("fstat %d failed: %m",p->fd);
			log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
		}
		info->size=min;
		efi_fd_file_fill_stat(&st,info);
		cnt=MIN(min-offsetof(efi_file_info,file_name),sizeof(p->name));
		memcpy(info->file_name,p->name,cnt);
	}else log_return(efi_unsupported);
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_set_info(
	efi_file_protocol *proto,
	const efi_guid    *type,
	uintn_t           size,
	void              *buffer
){
	log_call(
		call_arg(ptr,proto),
		call_arg(guid,type),
		call_arg(unsigned,size),
		call_arg(ptr,buffer),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs->writable)log_return(efi_write_protected);
	// TODO: unimplemented
	log_return(efi_unsupported);
}

static efiapi efi_status efi_fd_file_flush(
	efi_file_protocol*proto
){
	log_call(
		call_arg(ptr,proto),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs->writable||p->folder)log_return(efi_write_protected);
	errno=0;
	if(fdatasync(p->fd)<0){
		xerror("fdatasync %d failed: %m",p->fd);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	log_return(efi_success);
}

static efiapi efi_status efi_fd_file_open_ex(
	efi_file_protocol *proto,
	efi_file_protocol **handle,
	char16            *file,
	uint64_t          mode,
	uint64_t          attrs,
	efi_file_io_token *token
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		call_arg(str16,file),
		call_arg(hex,mode),
		call_arg(hex,attrs),
		call_arg(ptr,token),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_fd_file_read_ex(
	efi_file_protocol*proto,
	efi_file_io_token*token
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,token),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_fd_file_write_ex(
	efi_file_protocol*proto,
	efi_file_io_token*token
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,token),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_fd_file_flush_ex(
	efi_file_protocol*proto,
	efi_file_io_token*token
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,token),
		NULL
	);
	efi_file_protocol_private_posix_fd*p=
		(efi_file_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FILE_MAGIC))
		log_return(efi_invalid_parameter);
	if(!p->fs||!efi_guid_equals(&p->fs->data.magic,&PROTO_FS_MAGIC))
		log_return(efi_invalid_parameter);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_fd_fs_open_volume(
	efi_simple_file_system_protocol*proto,
	efi_file_protocol**root
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,root),
		NULL
	);
	efi_file_protocol_private_posix_fd*f;
	efi_simple_file_system_protocol_private_posix_fd*p=
		(efi_simple_file_system_protocol_private_posix_fd*)proto;
	if(root)*root=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!p||!efi_guid_equals(&p->data.magic,&PROTO_FS_MAGIC)||!root)
		log_return(efi_invalid_parameter);
	if(!(f=new_file(efi_current_ctx)))log_return(efi_out_of_resources);
	errno=0;
	if((f->fd=dup(p->fd))<0){
		xerror("dup %d failed: %m",f->fd);
		mem_free(f);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	if(!(f->dir=fdopendir(f->fd))){
		xerror("fdopendir %d failed: %m",p->fd);
		close(f->fd);
		mem_free(f);
		log_return(errno==0?efi_device_error:efi_errno_to_status(errno));
	}
	f->folder=true,f->fs=p,f->refcnt=1;
	*root=&f->data.protocol;
	xlog(LOG_CALL,"out root=%p",*root);
	log_return(efi_success);
}

static const efi_component_name_protocol comp_name={
	.get_driver_name     = efi_fd_file_get_driver_name,
	.get_controller_name = efi_fd_file_get_controller_name,
	.supported_languages = (char8*)"eng",
};

static const efi_component_name2_protocol comp_name2={
	.get_driver_name     = efi_fd_file_get_driver_name,
	.get_controller_name = efi_fd_file_get_controller_name,
	.supported_languages = (char8*)"en",
};

static efi_file_protocol file_proto={
	.revision = EFI_FILE_IO_INTERFACE_REVISION,
	.open     = efi_fd_file_open,
	.close    = efi_fd_file_close,
	.delete   = efi_fd_file_delete,
	.read     = efi_fd_file_read,
	.write    = efi_fd_file_write,
	.set_pos  = efi_fd_file_set_position,
	.get_pos  = efi_fd_file_get_position,
	.get_info = efi_fd_file_get_info,
	.set_info = efi_fd_file_set_info,
	.flush    = efi_fd_file_flush,
	.read_ex  = efi_fd_file_read_ex,
	.write_ex = efi_fd_file_write_ex,
	.flush_ex = efi_fd_file_flush_ex,
};

static efi_simple_file_system_protocol fs_proto={
	.revision    = EFI_FILE_IO_INTERFACE_REVISION,
	.open_volume = efi_fd_fs_open_volume,
};

int efi_simple_file_system_posix_fd_get_path(
	efi_file_protocol*fp,
	int*fd,char**path
){
	if(fd)*fd=-1;
	if(path)*path=NULL;
	if(!fp)RET(EINVAL);
	efi_file_protocol_private_posix_fd*f=
		(efi_file_protocol_private_posix_fd*)fp;
	if(!f||!efi_guid_equals(&f->data.magic,&PROTO_FILE_MAGIC))RET(ENOTSUP);
	if(fd&&f->fs)*fd=f->fs->fd;
	if(path&&f->path[0])*path=f->path;
	RET(0);
}

efi_status efi_new_simple_file_system_from_posix_fd(
	int fd,
	bool writable,
	efi_handle*handle,
	efi_simple_file_system_protocol**proto
){
	static const size_t size=sizeof(efi_simple_file_system_protocol_private_posix_fd);
	efi_simple_file_system_protocol_private_posix_fd*ret;
	if(!efi_current_ctx)return efi_not_ready;
	if(!handle)return efi_invalid_parameter;
	if(!(ret=mem_allocate(
		efi_current_ctx->mem[efi_bs_data].pool,size
	)))return efi_out_of_resources;
	memset(ret,0,size);
	memcpy(&ret->data.protocol,&fs_proto,sizeof(ret->data.protocol));
	ustrcpy(ret->name,(char16*)L"Posix Folder");
	ret->data.magic=PROTO_FS_MAGIC;
	ret->writable=writable;
	ret->fd=fd;
	if(proto)*proto=(efi_simple_file_system_protocol*)ret;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiDevicePathProtocolGuid,&dp,
		&gEfiComponentNameProtocolGuid,&comp_name,
		&gEfiComponentName2ProtocolGuid,&comp_name2,
		&gEfiSimpleFileSystemProtocolGuid,ret,
		NULL
	);
}

efi_status efi_new_simple_file_system_from_posix_cfd(
	int cfd,
	const char*path,
	bool writable,
	efi_handle*handle,
	efi_simple_file_system_protocol**proto
){
	int fd=openat(cfd,path,O_DIRECTORY|O_RDONLY);
	if(fd<0)return efi_errno_to_status(errno);
	return efi_new_simple_file_system_from_posix_fd(
		fd,writable,handle,proto
	);
}

efi_status efi_new_simple_file_system_from_posix_path(
	const char*path,
	bool writable,
	efi_handle*handle,
	efi_simple_file_system_protocol**proto
){
	return efi_new_simple_file_system_from_posix_cfd(
		AT_FDCWD,path,writable,handle,proto
	);
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	char*cfg;
	bool writable;
	if(!ctx||!ins)
		return efi_invalid_parameter;
	if(!ins->cfg||!(cfg=ins->cfg->config.ptr))
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	if(cfg[0]!='R'||cfg[2]!=':'||!cfg[3])return efi_invalid_parameter;
	if(cfg[1]=='O')writable=false;
	else if(cfg[1]=='W')writable=true;
	else return efi_invalid_parameter;
	return efi_new_simple_file_system_from_posix_path(
		cfg+3,writable,&ins->handle,
		(efi_simple_file_system_protocol**)&ins->cust_data
	);
}

static const efi_service efi_posix_fd_simple_file_system_service={
	.guid     = &gEfiWrapperPosixFDSimpleFileSystemProtocolGuid,
	.name     = "PosixFDSimpleFileSystemProtocol",
	.provides = {
		&gEfiSimpleFileSystemProtocolGuid,
		NULL
	},
	.depends  = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start    = svc_start,
};

static const efi_service_cfg efi_posix_fd_simple_file_system_service_ro_root_cfg={
	.name       = "UnixRoot",
	.service    = &efi_posix_fd_simple_file_system_service,
	.config.ptr = "RO:/",
};

DECLARE_EFI_SERVICE_CONFIG(efi_posix_fd_simple_file_system_service_ro_root_cfg)
