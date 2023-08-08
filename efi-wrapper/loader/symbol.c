#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include"efi/efi_file.h"
#include"defines.h"

bool efi_off_addr_to_readable(efi_file*file,size_t addr,char*buff,size_t len){
	efi_symbol sym;
	size_t so=0,bo=0;
	if(!file||!buff||len<=0)return false;
	memset(buff,0,len);
	snprintf(
		buff+bo,len-bo-1,
		"EFI(0x%12zx) 0x%08zx",
		(size_t)file->image,addr
	);
	bo+=strlen(buff+bo);
	if(efi_lookup_symbol(file,&sym,addr,&so)){
		if(bo<len){
			strncpy(buff+bo,sym.name[0]?sym.name:"(unknown)",len-bo-1);
			bo+=strlen(buff+bo);
		}
		if(bo<len)snprintf(buff+bo,len-bo-1,"+0x%zx",so);
	}
	return true;
}

bool efi_addr_to_readable(efi_file*file,size_t addr,char*buff,size_t len){
	size_t xo=0;
	if(!efi_address_is_loaded_image(file,addr,&xo))return false;
	return efi_off_addr_to_readable(file,xo,buff,len);
}

bool efi_address_is_loaded_image(efi_file*file,size_t addr,size_t*off){
	if(!file||!file->image||file->image_size<=0)return false;
	size_t start=(size_t)file->image;
	size_t end=(size_t)file->image+file->image_size;
	if(addr<start||addr>end)return false;
	if(off)*off=addr-start;
	return true;
}

bool efi_address_has_section_flag(efi_file*file,size_t addr,pe_section_flag flag){
	pe_section_header*sec;
	if(!file)return false;
	if(!(sec=efi_lookup_section(file,addr,NULL)))return false;
	return pe_hdr_sect_check_flag(sec,flag);
}

pe_section_header*efi_lookup_section(efi_file*file,size_t addr,uint16_t*sid){
	pe_section_header*sec;
	for(uint16_t i=0;i<file->file_header.sections_cnt;i++){
		sec=&file->sections[i];
		if(addr<sec->virtual_address)continue;
		if(addr>=sec->virtual_address+sec->raw_data_size)continue;
		if(sid)*sid=i+1;
		return sec;
	}
	return NULL;
}

static bool efi_symbol_get_name(efi_file*file,pe_symbol*sym,char*buff,size_t len){
	char*str;
	pe_file_header*fh;
	size_t toff,off,size;
	if(!buff)return false;
	fh=&file->file_header;
	size=fh->symbols_cnt*sizeof(pe_symbol);
	toff=fh->symbol_table+size;
	memset(buff,0,len);
	if(!sym->name.short_name[0]){
		off=toff+sym->name.name.long_num;
		str=file->data+off;
		if(off<file->length){
			strncpy(buff,str,len-1);
			return true;
		}
		xerror("symbol string out of file");
	}else{
		strncpy(
			buff,sym->name.short_name,
			MIN(len-1,sizeof(sym->name))
		);
		return true;
	}
	return false;
}

static bool efi_lookup_builtin_symbol(
	efi_file*file,
	efi_symbol*dst,
	size_t addr,
	size_t*off
){
	uint16_t sid=0;
	size_t offset,no;
	pe_symbol*sym,*symbol=NULL;
	pe_section_header*section=NULL;
	if(!file||!dst||!file->symbols)return false;
	if(!(section=efi_lookup_section(file,addr,&sid)))return false;
	offset=addr-section->virtual_address;
	for(uint32_t i=0;i<file->file_header.symbols_cnt;i++){
		sym=&file->symbols[i];
		if(sym->section!=sid)continue;
		if(sym->value>offset)continue;
		no=offset-sym->value;
		if(symbol&&no>offset-symbol->value)continue;
		if(off)*off=no;
		symbol=sym;
	}
	if(!symbol)return false;
	memset(dst,0,sizeof(efi_symbol));
	dst->address=symbol->value;
	efi_symbol_get_name(file,symbol,dst->name,sizeof(dst->name));
	return true;
}

#ifdef ELF_FOUND
static bool efi_lookup_elf_debug_symbol(
	efi_file*file,
	efi_symbol*dst,
	uint64_t addr,
	size_t*off
){
	char*buf;
	GElf_Sym sym;
	#ifdef DWFL_FOUND
	int lineno=0;
	const char*src;
	Dwfl_Line*line;
	#endif
	bool found=false;
	if(!file||!dst||!file->elf_dbg)return false;
	memset(dst,0,sizeof(efi_symbol));
	size_t count=file->elf_dbg_shdr.sh_size/file->elf_dbg_shdr.sh_entsize;
	for(size_t i=0;i<count;i++){
		if(!gelf_getsym(file->elf_dbg_symbols,i,&sym))continue;
		if(sym.st_size<=0||!IS_BETWEEN_MEM(addr,sym.st_value,sym.st_size))continue;
		found=true;
		break;
	}
	if(!found)return false;
	if(off)*off=addr-sym.st_value;
	if((buf=elf_strptr(file->elf_dbg,file->elf_dbg_shdr.sh_link,sym.st_name)))
		strncpy(dst->name,buf,sizeof(dst->name)-1);
	dst->address=sym.st_value;
	#ifdef DWFL_FOUND
	if(
		file->elf_dbg_mod&&
		(line=dwfl_module_getsrc(file->elf_dbg_mod,addr))&&
		(src=dwfl_lineinfo(line,&addr,&lineno,NULL,NULL,NULL))
	){
		strncpy(dst->src,src,sizeof(dst->src)-1);
		dst->line=lineno;
	}
	#endif
	return true;
}

#ifdef DWFL_FOUND
static const Dwfl_Callbacks callbacks={
	.find_elf        = dwfl_build_id_find_elf,
	.section_address = dwfl_offline_section_address,
	.find_debuginfo  = dwfl_standard_find_debuginfo,
};
#endif

bool efi_load_elf_debug(efi_file*file,int cfd,const char*path){
	int fd=-1;
	bool found=false;
	GElf_Shdr shdr;
	Elf_Scn*scn=NULL;
	if(
		!file||!path||file->elf_dbg||
		file->elf_dbg_fd>=0||file->elf_dbg_symbols
	)return false;
	elf_version(EV_CURRENT);
	if((file->elf_dbg_fd=openat(cfd,path,O_RDONLY))<0)
		DONE(xerror("open elf debug file %s failed: %m",path));
	if(!(file->elf_dbg=elf_begin(file->elf_dbg_fd,ELF_C_READ,NULL)))
		DONE(xerror("load elf debug file %s failed: %s",path,elf_errmsg(elf_errno())));
	while((scn=elf_nextscn(file->elf_dbg,scn))){
		if(!gelf_getshdr(scn,&shdr))continue;
		if(shdr.sh_type!=SHT_SYMTAB)continue;
		memcpy(&file->elf_dbg_shdr,&shdr,sizeof(file->elf_dbg_shdr));
		found=true;
		break;
	}
	if(!found)DONE(xerror("no symbol table found in %s",path));
	if(!(file->elf_dbg_symbols=elf_getdata(scn,NULL)))
		DONE(xerror("get symbol table from %s failed",path));
	#ifdef DWFL_FOUND
	if((file->elf_dbg_dwfl=dwfl_begin(&callbacks))){
		if((fd=dup(file->elf_dbg_fd))<0)goto done;
		if(!(file->elf_dbg_mod=dwfl_report_offline(
			file->elf_dbg_dwfl,
			"",path,fd
		)))xerror("load dwfl module from %s failed",path);
		close(fd);
	}else xerror("init dwfl from %s failed",path);
	#endif
	xlog(LOG_INFO,"Loaded ELF Debug file %s",path);
	return true;
	done:
	if(fd>=0)close(fd);
	if(file->elf_dbg)elf_end(file->elf_dbg);
	if(file->elf_dbg_fd>=0)close(file->elf_dbg_fd);
	file->elf_dbg=NULL,file->elf_dbg_fd=-1;
	memset(&file->elf_dbg_shdr,0,sizeof(file->elf_dbg_shdr));
	return false;
}
#else
static bool efi_lookup_elf_debug_symbol(
	efi_file*file cdecl_attr_unused,
	efi_symbol*dst cdecl_attr_unused,
	size_t addr cdecl_attr_unused,
	size_t*off cdecl_attr_unused
){
	return false;
}
bool efi_load_elf_debug(
	efi_file*file cdecl_attr_unused,
	int cfd cdecl_attr_unused,
	const char*path cdecl_attr_unused
){
	return false;
}
#endif

bool efi_lookup_symbol(efi_file*file,efi_symbol*dst,size_t addr,size_t*off){
	if(efi_lookup_builtin_symbol(file,dst,addr,off))return true;
	if(efi_lookup_elf_debug_symbol(file,dst,addr,off))return true;
	return false;
}
