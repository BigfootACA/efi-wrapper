#include<string.h>
#include<stdbool.h>
#include<sys/mman.h>
#include"mem.h"
#include"logger.h"
#include"defines.h"
#include"efi/efi_file.h"
#define PAGE_SIZE 4096

static bool efi_alloc_image(efi_file*file){
	char buff[64];
	bool ret=false;
	mem_pool*pool;
	pe_subsystem subsys;
	efi_memory_type type;
	efi_run_context*ctx=file->context?:efi_current_ctx;
	OPT_HDR_GET(file,file->image_size,image_size,return false)
	OPT_HDR_GET(file,subsys,subsystem,return false)
	if(!ctx)EDONE(xerror("no context set"));
	switch(subsys){
		case SUBSYSTEM_EFI_APPLICATION:
			type=efi_loader_code;
		break;
		case SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
		case SUBSYSTEM_EFI_RUNTIME_DRIVER:
		case SUBSYSTEM_EFI_ROM:
			type=efi_bs_code;
		break;
		default:EDONE(xerror("unsupported subsystem"));
	}
	if(!(pool=file->context->mem[type].pool))EDONE(xerror("no available memory pool"));
	file->image=mem_aligned_allocate(pool,PAGE_SIZE,file->image_size);
	if(!file->image)EDONE(xerror("allocate image failed"));
	xlog(LOG_INFO,
		"Allocate size 0x%zx (%zu bytes / %s) image at 0x%zx",
		file->image_size,file->image_size,
		make_readable_str_buf(buff,sizeof(buff),file->image_size,1,0),
		(size_t)file->image
	);
	memset(file->image,0,file->image_size);
	ret=true;
	done:
	return ret;
}

static bool efi_load_sections(efi_file*file){
	size_t size;
	void*src,*dst;
	bool ret=false;
	char name[16],rwx[4],buff[64];
	OPT_HDR_GET(file,size,headers_size,return false)
	xlog(LOG_INFO,
		"Loading headers to 0x%012zx (0x%08zx/%zu bytes)",
		(size_t)file->image,size,size
	);
	memcpy(file->image,file->data,size);
	for(uint16_t i=0;i<file->file_header.sections_cnt;i++){
		pe_section_header*sec=&file->sections[i];
		memset(name,0,sizeof(name));
		memcpy(name,sec->name,sizeof(sec->name));
		memcpy(rwx,"---",4);
		if(pe_hdr_sect_check_flag(sec,SCN_MEM_READ))rwx[0]='r';
		if(pe_hdr_sect_check_flag(sec,SCN_MEM_WRITE))rwx[1]='w';
		if(pe_hdr_sect_check_flag(sec,SCN_MEM_EXECUTE))rwx[2]='x';
		src=file->data+sec->raw_data_ptr;
		dst=file->image+sec->virtual_address;
		xlog(LOG_INFO,
			"Loading %s section %-2d %-8s from 0x%08x to 0x%012zx(0x%08x) (0x%08x / %u bytes / %s)",
			rwx,i+1,name,sec->raw_data_ptr,(size_t)dst,
			sec->virtual_address,sec->raw_data_size,sec->raw_data_size,
			make_readable_str_buf(buff,sizeof(buff),sec->raw_data_size,1,0)
		);
		if(sec->raw_data_ptr+sec->raw_data_size>file->length)
			EDONE(xerror("data out of file"));
		if(sec->virtual_address+sec->raw_data_size>file->image_size)
			EDONE(xerror("data out of image"));
		memcpy(dst,src,sec->raw_data_size);
	}
	ret=true;
	done:
	return ret;
}

static bool efi_relocation(efi_file*file){
	void*end;
	uint64_t*v64;
	uint32_t*v32;
	pe_data_dir*dir;
	size_t relocs=0;
	pe_section_header*hdr;
	switch(file->opt_header.common.magic){
		case PE32_MAGIC:dir=&file->opt_header.pe32.data_dir[5];break;
		case PE64_MAGIC:dir=&file->opt_header.pe64.data_dir[5];break;
		default:return false;
	}
	if(dir->size==0)return true;
	uintn_t image=(uintn_t)file->image;
	pe_relocation_base*rel=file->image+dir->virt_addr;
	if(!(hdr=efi_lookup_section(file,dir->virt_addr,NULL))){
		xerror("relocation address not found in any sections");
		return false;
	}
	if(!pe_hdr_sect_check_flag(hdr,SCN_MEM_READ)){
		xerror("relocation address in not writable sections");
		return false;
	}
	end=file->image+hdr->virtual_address+hdr->raw_data_size;
	for(;(void*)rel<end&&rel->virtual_address>0;rel=(void*)rel+rel->block_size){
		pe_relocation*ri=(void*)rel+sizeof(pe_relocation_base);
		size_t size=(rel->block_size-sizeof(pe_relocation_base))/sizeof(pe_relocation);
		for(size_t y=0;y<size;y++){
			void*ptr=file->image+rel->virtual_address+ri[y].offset;
			v64=ptr,v32=ptr,relocs++;
			switch(ri[y].type){
				case REL_ABSOLUTE:break;
				case REL_DIR64:*v64+=(uint64_t)image;break;
				case REL_HIGHLOW:*v32+=(uint32_t)image;break;
				default:xerror("unsupported relocation type %d",ri[y].type);return false;
                        }
                }
	}
	xlog(LOG_INFO,"processed %zu relocations",relocs);
	return true;
}

static bool efi_locate_entry(efi_file*file){
	char buff[16];
	bool ret=false,found=false;
	uint32_t entry=file->opt_header.common.entrypoint;
	if(entry>=file->image_size)
		EDONE(xerror("entry point out of image"));
	for(uint16_t i=0;i<file->file_header.sections_cnt;i++){
		pe_section_header*sec=&file->sections[i];
		if(entry<sec->virtual_address)continue;
		if(entry>sec->virtual_address+sec->raw_data_size)continue;
		if(!pe_hdr_sect_check_flag(sec,SCN_MEM_EXECUTE))
			EDONE(xerror("entry section not executable"));
		memset(buff,0,sizeof(buff));
		memcpy(buff,sec->name,sizeof(sec->name));
		found=true;
		break;
	}
	if(!found)EDONE(xerror("entry not found on any sections"));
	file->entry=file->image+entry;
	xlog(LOG_INFO,
		"Entry point at 0x%012zx (Image+0x%08x) in section %s",
		(size_t)file->entry,entry,buff
	);
	ret=true;
	done:
	return ret;
}

static inline int get_mmap_prot(pe_section_header*sec){
	int protect=0;
	if(pe_hdr_sect_check_flag(sec,SCN_MEM_READ))protect|=PROT_READ;
	if(pe_hdr_sect_check_flag(sec,SCN_MEM_WRITE))protect|=PROT_WRITE;
	if(pe_hdr_sect_check_flag(sec,SCN_MEM_EXECUTE))protect|=PROT_EXEC;
	return protect;
}

static char*mmap_prot_to_str(int prot,char*buff){
	memcpy(buff,"---",4);
	if((prot&PROT_READ)==PROT_READ)buff[0]='r';
	if((prot&PROT_WRITE)==PROT_WRITE)buff[1]='w';
	if((prot&PROT_EXEC)==PROT_EXEC)buff[2]='x';
	return buff;
}

static bool proc_overlap(efi_file*file,pe_section_header*cur,void**area,size_t*size){
	bool ret=false;
	void*reg,*ca;
	int p1,p2;
	size_t cs;
	char buff1[4],buff2[64];
	pe_section_header*sec;
	for(uint16_t x=0;x<file->file_header.sections_cnt;x++){
		sec=&file->sections[x];
		reg=file->image+sec->virtual_address;
		if(!IS_BETWEEN_MEM(*area,reg,sec->raw_data_size))continue;
		p1=get_mmap_prot(cur),p2=get_mmap_prot(sec);
		if(p1==p2)continue;
		ca=*area,cs=0;
		while(
			(*size)>PAGE_SIZE&&
			IS_BETWEEN_MEM(*area,reg,sec->raw_data_size)
		)(*area)+=PAGE_SIZE,(*size)-=PAGE_SIZE,cs+=PAGE_SIZE;
		if(cs>0){
			xlog(LOG_INFO,
				"set overlap protect %s to 0x%zx - 0x%zx (0x%08zx / %zu bytes / %s)",
				mmap_prot_to_str(p1|p2,buff1),(size_t)ca,(size_t)ca+cs,
				cs,cs,make_readable_str_buf(buff2,sizeof(buff2),cs,1,0)
			);
			if(mprotect(ca,cs,p1|p2)!=0)
				EDONE(xerror("mprotect overlap failed: %m"));
		}
	}
	ret=true;
	done:
	return ret;
}

static bool efi_memory_protect(efi_file*file){
	bool ret=false;
	size_t size;
	int protect;
	char buff1[4],buff2[64];
	void*dst,*area;
	pe_section_header*sec;
	if(file->permissive){
		xlog(LOG_INFO,
			"Set memory permissive 0x%zx length 0x%zx",
			(size_t)file->image,file->image_size
		);
		mprotect(file->image,file->image_size,PROT_READ|PROT_WRITE|PROT_EXEC);
		return true;
	}
	OPT_HDR_GET(file,size,headers_size,return false)
	mprotect(file->image,file->image_size,PROT_NONE);
	mprotect(file->image,size,PROT_READ);
	for(uint16_t i=0;i<file->file_header.sections_cnt;i++){
		sec=&file->sections[i],protect=get_mmap_prot(sec);
		dst=file->image+sec->virtual_address,size=sec->raw_data_size;
		area=(void*)round_down((size_t)dst,(size_t)PAGE_SIZE);
		if(area!=dst){
			size+=dst-area;
			if(!proc_overlap(file,sec,&area,&size))goto done;
		}
		if(size>0){
			xlog(LOG_INFO,
				"set regions protect %s to 0x%zx - 0x%zx (0x%08zx / %zu bytes / %s)",
				mmap_prot_to_str(protect,buff1),(size_t)area,(size_t)area+size,
				size,size,make_readable_str_buf(buff2,sizeof(buff2),size,1,0)
			);
			if(mprotect(area,size,protect)!=0)
				EDONE(xerror("mprotect failed: %m"));
		}
	}
	ret=true;
	done:
	return ret;
}

void call_debugger_load_symbol(const char*path,void*addr){
	xlog(LOG_INFO,"add-symbol-file %s -o %p",path,addr);
}

bool efi_load_data(efi_file*file){
	if(file->image||file->image_size>0)return false;
	if(!efi_alloc_image(file))return false;
	if(!efi_load_sections(file))return false;
	if(!efi_relocation(file))return false;
	if(!efi_locate_entry(file))return false;
	if(!efi_memory_protect(file))return false;
	call_debugger_load_symbol(file->path,file->image);
	return true;
}
