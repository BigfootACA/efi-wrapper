#include<stddef.h>
#include<string.h>
#include<stdbool.h>
#include"efi/efi_file.h"
#include"defines.h"
#include"logger.h"

#define SS sizeof(pe_symbol)
#define SFH sizeof(pe_file_header)
#define SSH sizeof(pe_section_header)
#define SOH sizeof(pe_opt_header)
#define SOH32L offsetof(pe_opt_header_pe32,data_dir)
#define SOH32H sizeof(pe_opt_header_pe32)
#define SOH64L offsetof(pe_opt_header_pe64,data_dir)
#define SOH64H sizeof(pe_opt_header_pe64)
#define SOHC sizeof(pe_opt_header_common)

static bool load_file_header(efi_file*file){
	bool ret=false;
	pe_file_header*fh;
	file->pe_off=*(uint32_t*)(file->data+0x3c);
	xlog(LOG_INFO,"PE header offset: 0x%x",file->pe_off);
	if(file->pe_off+SFH>=file->length)
		EDONE(xerror("invalid pe header offset"));
	fh=&file->file_header;
	memcpy(fh,file->data+file->pe_off,SFH);
	if(!pe_hdr_check_magic(fh))
		EDONE(xerror("pe magic mismatch"));
	pe_print_file_header(fh);
	if(!pe_hdr_check_machine(fh))
		EDONE(xerror("pe machine type mismatch"));
	if(!pe_hdr_check_flag(fh,FLAG_EXECUTABLE_IMAGE))
		EDONE(xerror("pe file not executable image"));
	ret=true;
	done:
	return ret;
}

static bool load_opt_header(efi_file*file){
	bool ret=false;
	pe_opt_header*oh;
	pe_file_header*fh=&file->file_header;
	if(fh->opt_hdr_size<=SOHC||fh->opt_hdr_size>SOH)
		EDONE(xerror("invalid pe opt header size"));
	if(file->pe_off+SFH+fh->opt_hdr_size>=file->length)
		EDONE(xerror("invalid pe opt header offset"));
	oh=&file->opt_header;
	memcpy(oh,file->data+file->pe_off+SFH,fh->opt_hdr_size);
	switch(oh->common.magic){
		case PE32_MAGIC:
			if(fh->opt_hdr_size>SOHC+SOH32H||fh->opt_hdr_size<SOHC+SOH32L)
				EDONE(xerror("invalid pe32 opt header size"));
		break;
		case PE64_MAGIC:
			if(fh->opt_hdr_size>SOHC+SOH64H||fh->opt_hdr_size<SOHC+SOH64L)
				EDONE(xerror("invalid pe32+ opt header size"));
		break;
		default:EDONE(xerror("invalid pe opt header magic"));
	}
	pe_print_opt_header(fh,oh);
	ret=true;
	done:
	return ret;
}

static bool load_sections(efi_file*file){
	bool ret=false;
	pe_file_header*fh=&file->file_header;
	size_t off=file->pe_off+SFH+fh->opt_hdr_size;
	size_t size=fh->sections_cnt*SSH;
	file->sections=file->data+off;
	if(off+size>=file->length)
		EDONE(xerror("pe sections header out of file"));
	pe_print_section_headers(file->sections,fh->sections_cnt);
	ret=true;
	done:
	return ret;
}

static bool load_symbols(efi_file*file){
	bool ret=false;
	pe_file_header*fh=&file->file_header;
	size_t size=fh->symbols_cnt*SS;
	if(fh->symbol_table==0)return true;
	if(fh->symbols_cnt==0)return true;
	if(fh->symbol_table+size>file->length)
		EDONE(xerror("pe symbols table out of file"));
	file->symbols=file->data+fh->symbol_table;
	ret=true;
	done:
	return ret;
}

bool efi_parse(efi_file*file){
	if(!load_file_header(file))return false;
	if(!load_opt_header(file))return false;
	if(!load_sections(file))return false;
	if(!load_symbols(file))return false;
	return true;
}
