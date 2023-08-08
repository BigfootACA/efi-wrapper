#include<stdbool.h>
#include"pe.h"

bool pe_check_machine(pe_machine_type type){
	switch(type){
		#if defined(__x86_64__)||defined(__amd64__)
		case MACHINE_AMD64:return true;
		#elif defined(__i386__)
		case MACHINE_I386:return true;
		#elif defined(__arm__)
		case MACHINE_ARM:return true;
		case MACHINE_ARMV7:return true;
		case MACHINE_ARMNT:return true;
		case MACHINE_THUMB:return true;
		#elif defined(__aarch64__)
		case MACHINE_ARM64:return true;
		#elif defined(__riscv)&&(__riscv_xlen == 32)
		case IMAGE_FILE_MACHINE_RISCV32:return true;
		#elif defined(__riscv)&&(__riscv_xlen == 64)
		case IMAGE_FILE_MACHINE_RISCV64:return true;
		#endif
		default:return false;
	}
}

bool pe_hdr_check_machine(pe_file_header*hdr){
	return pe_check_machine(hdr->machine);
}

bool pe_hdr_check_magic(pe_file_header*hdr){
	return hdr->magic.u32==PE_MAGIC;
}

bool pe_check_flag(pe_flag flags,pe_flag flag){
	return (flags&flag)==flag;
}

bool pe_hdr_check_flag(pe_file_header*hdr,pe_flag flag){
	return pe_check_flag(hdr->flags,flag);
}

bool pe_sect_check_flag(pe_section_flag flags,pe_section_flag flag){
	return (flags&flag)==flag;
}

bool pe_hdr_sect_check_flag(pe_section_header*sect,pe_section_flag flag){
	return pe_sect_check_flag(sect->flags,flag);
}
