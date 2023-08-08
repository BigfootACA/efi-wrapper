#ifndef EFI_WRAPPER_EFI_H
#define EFI_WRAPPER_EFI_H
#include<stdio.h>
#include<stddef.h>
#include<stdbool.h>
#include<setjmp.h>
#include"../pe.h"
#include"efi_spec.h"
#include"efi_context.h"
#include"protocols/loaded_image.h"
#ifdef ELF_FOUND
#include<libelf.h>
#include<gelf.h>
#ifdef DWFL_FOUND
#include<elfutils/libdwfl.h>
#endif
#endif
typedef struct efi_file{
	char path[4096];
	char name[256];
	void*data;
	bool permissive;
	jmp_buf jmp;
	efi_status st;
	void*exit_data;
	size_t exit_size;
	size_t length;
	uint32_t pe_off;
	pe_file_header file_header;
	pe_opt_header opt_header;
	pe_section_header*sections;
	pe_symbol*symbols;
	efi_image_entry_point entry;
	efi_handle image_handle;
	efi_loaded_image_protocol loaded;
	efi_run_context*context;
	efi_device_path*device_path;
	mem_pool*pool;
	void*image;
	size_t image_size;
	#ifdef ELF_FOUND
        Elf*elf_dbg;
        int elf_dbg_fd;
	GElf_Shdr elf_dbg_shdr;
        Elf_Data*elf_dbg_symbols;
	#ifdef DWFL_FOUND
	Dwfl*elf_dbg_dwfl;
	Dwfl_Module *elf_dbg_mod;
	#endif
	#endif
}efi_file;
typedef struct efi_symbol{
	char name[128];
	size_t address;
	char src[2048];
	size_t line;
}efi_symbol;
extern bool efi_parse(efi_file*file);
extern bool efi_off_addr_to_readable(efi_file*file,size_t addr,char*buff,size_t len);
extern bool efi_addr_to_readable(efi_file*file,size_t addr,char*buff,size_t len);
extern bool efi_address_is_loaded_image(efi_file*file,size_t addr,size_t*off);
extern bool efi_address_has_section_flag(efi_file*file,size_t addr,pe_section_flag flag);
extern pe_section_header*efi_lookup_section(efi_file*file,size_t addr,uint16_t*sid);
extern bool efi_lookup_symbol(efi_file*file,efi_symbol*dst,size_t addr,size_t*off);
extern efi_status efi_run_with(efi_file*file,bool fork_run,efi_handle handle,efi_system_table*table);
extern efi_status efi_run(efi_file*file,bool fork_run);
extern efi_file*efi_file_new(efi_run_context*ctx);
extern efi_file*efi_image_handle_get_file(efi_handle handle);
extern efi_handle efi_file_get_image_handle(efi_file*file);
extern bool efi_load_elf_debug(efi_file*file,int cfd,const char*path);
extern bool efi_load_by_data(efi_file*file,void*data,size_t length);
extern bool efi_load_by_dup_data(efi_file*file,void*data,size_t length);
extern bool efi_load_by_fd(efi_file*file,int fd);
extern bool efi_load_by_file(efi_file*file,FILE*f);
extern bool efi_load_by_at_path(efi_file*file,int cfd,const char*path);
extern bool efi_load_by_path(efi_file*file,const char*path);
extern bool efi_load_data(efi_file*file);
extern list**efi_running_stack();
extern efi_file*efi_running_get();
extern bool efi_is_running(efi_file*file);
extern bool efi_running_push(efi_file*file);
extern void efi_running_pop();
extern void efi_running_remove(efi_file*file);
extern efi_file*efi_running_lookup_ptr(size_t ptr);
#define OPT_HDR_GET(_file,_val,_field,_fail) \
	switch((_file)->opt_header.common.magic){\
		case PE32_MAGIC:(_val)=(_file)->opt_header.pe32._field;break;\
		case PE64_MAGIC:(_val)=(_file)->opt_header.pe64._field;break;\
		default:{_fail;}break;\
	}

#endif //EFI_WRAPPER_EFI_H
