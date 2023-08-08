#include<time.h>
#include<string.h>
#include"defines.h"
#include"logger.h"
#include"pe.h"

void pe_print_file_header(pe_file_header*hdr){
	time_t ts;
	char buff[64];
	const char*str;
	if(!hdr)return;
	ts=hdr->timestamp;
	memset(buff,0,sizeof(buff));
	if(ts>0)strftime(
		buff,sizeof(buff),
		": %Y-%m-%d %H:%M:%S",
		localtime(&ts)
	);
	xlog(LOG_DUMP,"PE Header:");
	xlog(LOG_DUMP,"  Machine:   0x%04x (%s)",
		hdr->machine,pe_machine_type_to_string(hdr->machine,"Unknown"));
	if(hdr->sections_cnt>0)xlog(
		LOG_DUMP,"  Sections:  0x%04x (%u)",
		hdr->sections_cnt,hdr->sections_cnt
	);
	if(hdr->timestamp>0)xlog(
		LOG_DUMP,"  Timestamp: 0x%08x (%u%s) (32-bit)",
		hdr->timestamp,hdr->timestamp,buff
	);
	if(hdr->symbol_table>0)xlog(
		LOG_DUMP,"  Symbol Table Pointer:  0x%08x",
		hdr->symbol_table
	);
	if(hdr->opt_hdr_size>0)xlog(
		LOG_DUMP,"  Optional Header Size:  0x%04x (%u bytes / %s)",
		hdr->opt_hdr_size,hdr->opt_hdr_size,
		make_readable_str_buf(buff,sizeof(buff),hdr->opt_hdr_size,1,0)
	);
	if(hdr->flags!=0){
		xlog(LOG_DUMP,"  Characteristics: 0x%04x",hdr->flags);
		for(size_t i=0;i<sizeof(uint16_t)*8-1;i++){
			uint16_t flag=1<<i;
			str=pe_flag_to_string(flag,"Unknown");
			if(pe_hdr_check_flag(hdr,flag))
				xlog(LOG_DUMP,"    0x%04x (%s)",flag,str);
		}
	}
}

static void pe_print_data_dirs(size_t len,pe_data_dir*dirs,size_t length){
	char buff[64];
	xlog(LOG_DUMP,"  Data Dirs:");
	for(size_t i=0;i<length;i++){
		if(sizeof(pe_data_dir)*(i+1)>len)continue;
		if(dirs[i].virt_addr==0)continue;
		if(dirs[i].size==0)continue;
		xlog(
			LOG_DUMP,"   %4zu: Virtual Address: 0x%08x; Size: 0x%08x (%u bytes / %s)",
			i,dirs[i].virt_addr,dirs[i].size,dirs[i].size,
			make_readable_str_buf(buff,sizeof(buff),dirs[i].size,1,0)
		);
	}
}

static void pe_print_opt_header_common(pe_opt_header_common*common){
	char buff[64];
	const char*str;
	switch(common->magic){
		case PE32_MAGIC:str="PE32";break;
		case PE64_MAGIC:str="PE32+";break;
		default:str="Unknown";break;
	}
	xlog(LOG_DUMP,"PE Optional Header:");
	xlog(LOG_DUMP,"  Magic:              0x%04x (%s)",common->magic,str);
	if(common->linker_major!=0&&common->linker_minor!=0)xlog(
		LOG_DUMP,"  Linker Version:     0x%02x.0x%02x (%u.%u)",
		common->linker_major,common->linker_minor,
		common->linker_major,common->linker_minor
	);
	if(common->code_size!=0)xlog(
		LOG_DUMP,"  Code Size:          0x%08x (%u bytes / %s)",
		common->code_size,common->code_size,
		make_readable_str_buf(buff,sizeof(buff),common->code_size,1,0)
	);
	if(common->init_data_size!=0)xlog(
		LOG_DUMP,"  Inited Data Size:   0x%08x (%u bytes / %s)",
		common->init_data_size,common->init_data_size,
		make_readable_str_buf(buff,sizeof(buff),common->init_data_size,1,0)
	);
	if(common->uninit_data_size!=0)xlog(
		LOG_DUMP,"  Uninited Data Size: 0x%08x (%u bytes / %s)",
		common->uninit_data_size,common->uninit_data_size,
		make_readable_str_buf(buff,sizeof(buff),common->uninit_data_size,1,0)
	);
	if(common->entrypoint!=0)xlog(
		LOG_DUMP,"  Entrypoint:         0x%08x",
		common->entrypoint
	);
	if(common->code_base!=0)xlog(
		LOG_DUMP,"  Code Base:          0x%08x",
		common->code_base
	);
}

static void pe_print_opt_header_pe32(pe_file_header*fh,pe_opt_header_pe32*pe32){
	char buff[64];
	if(offsetof(pe_opt_header_pe32,data_dir)>fh->opt_hdr_size)return;
	if(pe32->data_base!=0)xlog(
		LOG_DUMP,"  Data Base:          0x%08x",
		pe32->data_base
	);
	if(pe32->image_base!=0)xlog(
		LOG_DUMP,"  Image Base:         0x%08x",
		pe32->image_base
	);
	if(pe32->section_align!=0)xlog(
		LOG_DUMP,"  Section Align:      0x%08x (%u)",
		pe32->section_align,pe32->section_align
	);
	if(pe32->file_align!=0)xlog(
		LOG_DUMP,"  File Align:         0x%08x (%u)",
		pe32->file_align,pe32->file_align
	);
	if(pe32->os_major!=0&&pe32->os_minor!=0)xlog(
		LOG_DUMP,"  OS Version:         0x%04x.0x%04x (%u.%u)",
		pe32->os_major,pe32->os_minor,
		pe32->os_major,pe32->os_minor
	);
	if(pe32->image_major!=0&&pe32->image_minor!=0)xlog(
		LOG_DUMP,"  Image Version:      0x%04x.0x%04x (%u.%u)",
		pe32->image_major,pe32->image_minor,
		pe32->image_major,pe32->image_minor);
	if(pe32->subsystem_major!=0&&pe32->subsystem_minor!=0)xlog(
		LOG_DUMP,"  Subsystem Version:  0x%04x.0x%04x (%u.%u)",
		pe32->subsystem_major,pe32->subsystem_minor,
		pe32->subsystem_major,pe32->subsystem_minor
	);
	if(pe32->win32_version!=0)xlog(
		LOG_DUMP,"  Win32 Version:      0x%08x (%u)",
		pe32->win32_version,pe32->win32_version
	);
	if(pe32->image_size!=0)xlog(
		LOG_DUMP,"  Image Size:         0x%08x (%u bytes / %s)",
		pe32->image_size,pe32->image_size,
		make_readable_str_buf(buff,sizeof(buff),pe32->image_size,1,0)
	);
	if(pe32->headers_size!=0)xlog(
		LOG_DUMP,"  Headers Size:       0x%08x (%u bytes / %s)",
		pe32->headers_size,pe32->headers_size,
		make_readable_str_buf(buff,sizeof(buff),pe32->headers_size,1,0)
	);
	if(pe32->checksum!=0)xlog(
		LOG_DUMP,"  Checksum:           0x%08x (%u)",
		pe32->checksum,pe32->checksum
	);
	if(pe32->subsystem!=0)xlog(
		LOG_DUMP,"  Subsystem:          0x%08x (%u: %s)",
		pe32->subsystem,pe32->subsystem,
		pe_subsystem_to_string(pe32->subsystem,"unknown")
	);
	if(pe32->dll_flags!=0)xlog(
		LOG_DUMP,"  Dll Flags:          0x%08x",
		pe32->dll_flags
	);
	if(pe32->stack_reserve_size!=0)xlog(
		LOG_DUMP,"  Stack Reserve Size: 0x%08x (%u bytes / %s)",
		pe32->stack_reserve_size,pe32->stack_reserve_size,
		make_readable_str_buf(buff,sizeof(buff),pe32->stack_reserve_size,1,0)
	);
	if(pe32->stack_commit_size!=0)xlog(
		LOG_DUMP,"  Stack Commit Size:  0x%08x (%u bytes / %s)",
		pe32->stack_commit_size,pe32->stack_commit_size,
		make_readable_str_buf(buff,sizeof(buff),pe32->stack_commit_size,1,0)
	);
	if(pe32->heap_reserve_size!=0)xlog(
		LOG_DUMP,"  Heap Reserve Size:  0x%08x (%u bytes / %s)",
		pe32->heap_reserve_size,pe32->heap_reserve_size,
		make_readable_str_buf(buff,sizeof(buff),pe32->heap_reserve_size,1,0)
	);
	if(pe32->heap_commit_size!=0)xlog(
		LOG_DUMP,"  Heap Commit Size:   0x%08x (%u bytes / %s)",
		pe32->heap_commit_size,pe32->heap_commit_size,
		make_readable_str_buf(buff,sizeof(buff),pe32->heap_commit_size,1,0)
	);
	if(pe32->loader_flags!=0)xlog(
		LOG_DUMP,"  Loader Flags:       0x%08x",
		pe32->loader_flags
	);
	if(pe32->data_dir_size!=0)xlog(
		LOG_DUMP,"  Data Dir Size:      0x%08x (%u)",
		pe32->data_dir_size,pe32->data_dir_size
	);
	pe_print_data_dirs(
		fh->opt_hdr_size-offsetof(pe_opt_header_pe32,data_dir),
		pe32->data_dir,pe32->data_dir_size
	);
}

static void pe_print_opt_header_pe64(pe_file_header*fh,pe_opt_header_pe64*pe64){
	char buff[64];
	if(offsetof(pe_opt_header_pe64,data_dir)>fh->opt_hdr_size)return;
	if(pe64->image_base!=0)xlog(
		LOG_DUMP,"  Image Base:         0x%012llx",
		(unsigned long long)pe64->image_base
	);
	if(pe64->section_align!=0)xlog(
		LOG_DUMP,"  Section Align:      0x%08x (%u)",
		pe64->section_align,pe64->section_align
	);
	if(pe64->file_align!=0)xlog(
		LOG_DUMP,"  File Align:         0x%08x (%u)",
		pe64->file_align,pe64->file_align
	);
	if(pe64->os_major!=0&&pe64->os_minor!=0)xlog(
		LOG_DUMP,"  OS Version:         0x%04x.0x%04x (%u.%u)",
		pe64->os_major,pe64->os_minor,
		pe64->os_major,pe64->os_minor
	);
	if(pe64->image_major!=0&&pe64->image_minor!=0)xlog(
		LOG_DUMP,"  Image Version:      0x%04x.0x%04x (%u.%u)",
		pe64->image_major,pe64->image_minor,
		pe64->image_major,pe64->image_minor
	);
	if(pe64->subsystem_major!=0&&pe64->subsystem_minor!=0)xlog(
		LOG_DUMP,"  Subsystem Version:  0x%04x.0x%04x (%u.%u)",
		pe64->subsystem_major,pe64->subsystem_minor,
		pe64->subsystem_major,pe64->subsystem_minor
	);
	if(pe64->win32_version!=0)xlog(
		LOG_DUMP,"  Win32 Version:      0x%08x (%u)",
		pe64->win32_version,pe64->win32_version
	);
	if(pe64->image_size!=0)xlog(
		LOG_DUMP,"  Image Size:         0x%08x (%u bytes / %s)",
		pe64->image_size,pe64->image_size,
		make_readable_str_buf(buff,sizeof(buff),pe64->image_size,1,0)
	);
	if(pe64->headers_size!=0)xlog(
		LOG_DUMP,"  Headers Size:       0x%08x (%u bytes / %s)",
		pe64->headers_size,pe64->headers_size,
		make_readable_str_buf(buff,sizeof(buff),pe64->headers_size,1,0)
	);
	if(pe64->checksum!=0)xlog(
		LOG_DUMP,"  Checksum:           0x%08x (%u)",
		pe64->checksum,pe64->checksum
	);
	if(pe64->subsystem!=0)xlog(
		LOG_DUMP,"  Subsystem:          0x%08x (%u: %s)",
		pe64->subsystem,pe64->subsystem,
		pe_subsystem_to_string(pe64->subsystem,"unknown")
	);
	if(pe64->dll_flags!=0)xlog(
		LOG_DUMP,"  Dll Flags:          0x%08x",
		pe64->dll_flags
	);
	if(pe64->stack_reserve_size!=0)xlog(
		LOG_DUMP,"  Stack Reserve Size: 0x%012llx (%llu bytes / %s)",
		(unsigned long long)pe64->stack_reserve_size,
		(unsigned long long)pe64->stack_reserve_size,
		make_readable_str_buf(buff,sizeof(buff),pe64->stack_reserve_size,1,0)
	);
	if(pe64->stack_commit_size!=0)xlog(
		LOG_DUMP,"  Stack Commit Size:  0x%012llx (%llu bytes / %s)",
		(unsigned long long)pe64->stack_commit_size,
		(unsigned long long)pe64->stack_commit_size,
		make_readable_str_buf(buff,sizeof(buff),pe64->stack_commit_size,1,0)
	);
	if(pe64->heap_reserve_size!=0)xlog(
		LOG_DUMP,"  Heap Reserve Size:  0x%012llx (%llu bytes / %s)",
		(unsigned long long)pe64->heap_reserve_size,
		(unsigned long long)pe64->heap_reserve_size,
		make_readable_str_buf(buff,sizeof(buff),pe64->heap_reserve_size,1,0)
	);
	if(pe64->heap_commit_size!=0)xlog(
		LOG_DUMP,"  Heap Commit Size:   0x%012llx (%llu bytes / %s)",
		(unsigned long long)pe64->heap_commit_size,
		(unsigned long long)pe64->heap_commit_size,
		make_readable_str_buf(buff,sizeof(buff),pe64->heap_commit_size,1,0)
	);
	if(pe64->loader_flags!=0)xlog(
		LOG_DUMP,"  Loader Flags:       0x%08x",
		pe64->loader_flags
	);
	if(pe64->data_dir_size!=0)xlog(
		LOG_DUMP,"  Data Dir Size:      0x%08x (%u)",
		pe64->data_dir_size,pe64->data_dir_size
	);
	pe_print_data_dirs(
		fh->opt_hdr_size-offsetof(pe_opt_header_pe64,data_dir),
		pe64->data_dir,pe64->data_dir_size
	);
}

void pe_print_opt_header(pe_file_header*fh,pe_opt_header*hdr){
	pe_print_opt_header_common(&hdr->common);
	switch(hdr->common.magic){
		case PE32_MAGIC:pe_print_opt_header_pe32(fh,&hdr->pe32);break;
		case PE64_MAGIC:pe_print_opt_header_pe64(fh,&hdr->pe64);break;
		default:;
	}
}

static void pe_print_section(const char*prefix,pe_section_header*sect){
	char buff[16];
	const char*str;
	memset(buff,0,sizeof(buff));
	memcpy(buff,sect->name,sizeof(sect->name));
	xlog(LOG_DUMP,"%sName: %s",prefix,buff);
	if(sect->misc.physical_address!=0&&sect->misc.virtual_size!=0)xlog(
		LOG_DUMP,"%sPhysical address / Virtual size: 0x%08x (%u bytes / %s)",
		prefix,sect->misc.physical_address,sect->misc.virtual_size,
		make_readable_str_buf(buff,sizeof(buff),sect->misc.virtual_size,1,0)
	);
	if(sect->virtual_address!=0)xlog(
		LOG_DUMP,"%sVirtual address: 0x%08x (%u)",
		prefix,sect->virtual_address,sect->virtual_address
	);
	if(sect->raw_data_ptr!=0&&sect->raw_data_size!=0)xlog(
		LOG_DUMP,"%sRaw Data:     Pointer 0x%08x Size  0x%08x (%u bytes / %s)",
		prefix,sect->raw_data_ptr,sect->raw_data_size,sect->raw_data_size,
		make_readable_str_buf(buff,sizeof(buff),sect->raw_data_size,1,0)
	);
	if(sect->relocations_ptr!=0&&sect->relocations_cnt!=0)xlog(
		LOG_DUMP,"%sRelocations:  Pointer 0x%08x Count 0x%08x (%u)",
		prefix,sect->relocations_ptr,sect->relocations_cnt,sect->relocations_cnt
	);
	if(sect->line_numbers_ptr!=0&&sect->line_numbers_cnt!=0)xlog(
		LOG_DUMP,"%sLine Numbers: Pointer 0x%08x Count 0x%08x (%u)",
		prefix,sect->line_numbers_ptr,sect->line_numbers_cnt,sect->line_numbers_cnt
	);
	if(sect->flags!=0){
		xlog(LOG_DUMP,"%sFlags: 0x%08x",prefix,sect->flags);
		for(size_t i=0;i<sizeof(uint32_t)*8-1;i++){
			uint32_t flag=1<<i;
			str=pe_section_flag_to_string(flag,"Unknown");
			if(pe_hdr_sect_check_flag(sect,flag))
				xlog(LOG_DUMP,"%s  0x%08x (%s)",prefix,flag,str);
		}
	}
}

void pe_print_section_header(pe_section_header*sect){
	xlog(LOG_DUMP,"PE Section Header:");
	pe_print_section("  ",sect);
}

void pe_print_section_headers(pe_section_header*sect,size_t len){
	xlog(LOG_DUMP,"PE Section Headers:");
	for(size_t i=0;i<len;i++){
		xlog(LOG_DUMP,"  Section %zu:",i+1);
		pe_print_section("    ",&sect[i]);
	}
}
