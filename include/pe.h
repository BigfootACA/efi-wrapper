#ifndef EFI_WRAPPER_PE_H
#define EFI_WRAPPER_PE_H
#include<assert.h>
#include<stdint.h>
#include<stddef.h>
#include<stdbool.h>
#include<defines.h>

typedef enum pe_machine_type{
	MACHINE_I386      = 0x014c,
	MACHINE_R3000     = 0x0162,
	MACHINE_R4000     = 0x0166,
	MACHINE_R10000    = 0x0168,
	MACHINE_WCEMIPSV2 = 0x0169,
	MACHINE_ALPHA     = 0x0184,
	MACHINE_SH3       = 0x01a2,
	MACHINE_SH3DSP    = 0x01a3,
	MACHINE_SH3E      = 0x01a4,
	MACHINE_SH4       = 0x01a6,
	MACHINE_SH5       = 0x01a8,
	MACHINE_ARM       = 0x01c0,
	MACHINE_ARMV7     = 0x01c4,
	MACHINE_ARMNT     = 0x01c4,
	MACHINE_ARM64     = 0xaa64,
	MACHINE_THUMB     = 0x01c2,
	MACHINE_AM33      = 0x01d3,
	MACHINE_POWERPC   = 0x01F0,
	MACHINE_POWERPCFP = 0x01f1,
	MACHINE_IA64      = 0x0200,
	MACHINE_MIPS16    = 0x0266,
	MACHINE_ALPHA64   = 0x0284,
	MACHINE_MIPSFPU   = 0x0366,
	MACHINE_MIPSFPU16 = 0x0466,
	MACHINE_AXP64     = MACHINE_ALPHA64,
	MACHINE_TRICORE   = 0x0520,
	MACHINE_CEF       = 0x0CEF,
	MACHINE_EBC       = 0x0EBC,
	MACHINE_AMD64     = 0x8664,
	MACHINE_M32R      = 0x9041,
	MACHINE_CEE       = 0xc0ee,
}pe_machine_type;

typedef enum pe_flag{
	FLAG_RELOCS_STRIPPED         = 0x0001,
	FLAG_EXECUTABLE_IMAGE        = 0x0002,
	FLAG_LINE_NUMS_STRIPPED      = 0x0004,
	FLAG_LOCAL_SYMS_STRIPPED     = 0x0008,
	FLAG_AGGRESIVE_WS_TRIM       = 0x0010,
	FLAG_LARGE_ADDRESS_AWARE     = 0x0020,
	FLAG_BYTES_REVERSED_LO       = 0x0080,
	FLAG_32BIT_MACHINE           = 0x0100,
	FLAG_DEBUG_STRIPPED          = 0x0200,
	FLAG_REMOVABLE_RUN_FROM_SWAP = 0x0400,
	FLAG_NET_RUN_FROM_SWAP       = 0x0800,
	FLAG_SYSTEM                  = 0x1000,
	FLAG_DLL                     = 0x2000,
	FLAG_UP_SYSTEM_ONLY          = 0x4000,
	FLAG_BYTES_REVERSED_HI       = 0x8000,
}pe_flag;

typedef enum pe_subsystem{
	SUBSYSTEM_NATIVE                   = 1,
	SUBSYSTEM_WINDOWS_GUI              = 2,
	SUBSYSTEM_WINDOWS_CUI              = 3,
	SUBSYSTEM_OS2_CUI                  = 5,
	SUBSYSTEM_POSIX_CUI                = 7,
	SUBSYSTEM_NATIVE_WINDOWS           = 8,
	SUBSYSTEM_WINDOWS_CE_GUI           = 9,
	SUBSYSTEM_EFI_APPLICATION          = 10,
	SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  = 11,
	SUBSYSTEM_EFI_RUNTIME_DRIVER       = 12,
	SUBSYSTEM_EFI_ROM                  = 13,
	SUBSYSTEM_XBOX                     = 14,
	SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16,
}pe_subsystem;

typedef enum pe_section_flag{
	SCN_TYPE_NO_PAD            = 0x00000008,
		SCN_CNT_CODE               = 0x00000020,
	SCN_CNT_INITIALIZED_DATA   = 0x00000040,
	SCN_CNT_UNINITIALIZED_DATA = 0x00000080,
	SCN_LNK_OTHER              = 0x00000100,
	SCN_LNK_INFO               = 0x00000200,
	SCN_LNK_REMOVE             = 0x00000800,
	SCN_LNK_COMDAT             = 0x00001000,
	SCN_NO_DEFER_SPEC_EXC      = 0x00004000,
	SCN_GPREL                  = 0x00008000,
	SCN_MEM_PURGEABLE          = 0x00020000,
	SCN_MEM_LOCKED             = 0x00040000,
	SCN_MEM_PRELOAD            = 0x00080000,
	SCN_ALIGN_1BYTES           = 0x00100000,
	SCN_ALIGN_2BYTES           = 0x00200000,
	SCN_ALIGN_4BYTES           = 0x00300000,
	SCN_ALIGN_8BYTES           = 0x00400000,
	SCN_ALIGN_16BYTES          = 0x00500000,
	SCN_ALIGN_32BYTES          = 0x00600000,
	SCN_ALIGN_64BYTES          = 0x00700000,
	SCN_ALIGN_128BYTES         = 0x00800000,
	SCN_ALIGN_256BYTES         = 0x00900000,
	SCN_ALIGN_512BYTES         = 0x00A00000,
	SCN_ALIGN_1024BYTES        = 0x00B00000,
	SCN_ALIGN_2048BYTES        = 0x00C00000,
	SCN_ALIGN_4096BYTES        = 0x00D00000,
	SCN_ALIGN_8192BYTES        = 0x00E00000,
	SCN_ALIGN_MASK             = 0x00F00000,
	SCN_LNK_NRELOC_OVFL        = 0x01000000,
	SCN_MEM_DISCARDABLE        = 0x02000000,
	SCN_MEM_NOT_CACHED         = 0x04000000,
	SCN_MEM_NOT_PAGED          = 0x08000000,
	SCN_MEM_SHARED             = 0x10000000,
	SCN_MEM_EXECUTE            = 0x20000000,
	SCN_MEM_READ               = 0x40000000,
	SCN_MEM_WRITE              = 0x80000000,
}pe_section_flag;

typedef enum pe_symbol_storage_class{
	CLASS_NULL             = 0x0000,
	CLASS_AUTOMATIC        = 0x0001,
	CLASS_EXTERNAL         = 0x0002,
	CLASS_STATIC           = 0x0003,
	CLASS_REGISTER         = 0x0004,
	CLASS_EXTERNAL_DEF     = 0x0005,
	CLASS_LABEL            = 0x0006,
	CLASS_UNDEFINED_LABEL  = 0x0007,
	CLASS_MEMBER_OF_STRUCT = 0x0008,
	CLASS_ARGUMENT         = 0x0009,
	CLASS_STRUCT_TAG       = 0x000A,
	CLASS_MEMBER_OF_UNION  = 0x000B,
	CLASS_UNION_TAG        = 0x000C,
	CLASS_TYPE_DEFINITION  = 0x000D,
	CLASS_UNDEFINED_STATIC = 0x000E,
	CLASS_ENUM_TAG         = 0x000F,
	CLASS_MEMBER_OF_ENUM   = 0x0010,
	CLASS_REGISTER_PARAM   = 0x0011,
	CLASS_BIT_FIELD        = 0x0012,
	CLASS_FAR_EXTERNAL     = 0x0044,
	CLASS_BLOCK            = 0x0064,
	CLASS_FUNCTION         = 0x0065,
	CLASS_END_OF_STRUCT    = 0x0066,
	CLASS_FILE             = 0x0067,
	CLASS_SECTION          = 0x0068,
	CLASS_WEAK_EXTERNAL    = 0x0069,
	CLASS_CLR_TOKEN        = 0x006B,
}pe_symbol_storage;
typedef enum pe_relocation_type{
	REL_ABSOLUTE       = 0,
	REL_HIGH           = 1,
	REL_LOW            = 2,
	REL_HIGHLOW        = 3,
	REL_HIGHADJ        = 4,
	REL_JMPADDR        = 5,
	REL_SECTION        = 6,
	REL_REL            = 7,
	REL_RISCV_LOW12S   = 8,
	REL_JMPADDR16      = 9,
	REL_DIR64          = 10,
	REL_HIGH3ADJ       = 11,
}pe_relocation_type;

#define PE_MAGIC 0x4550
#define PE32_MAGIC 0x10B
#define PE64_MAGIC 0x20B

typedef struct pe_file_header{
	union{
		char str[4];
		uint32_t u32;
	}magic;
	pe_machine_type machine:16;
	uint16_t sections_cnt;
	uint32_t timestamp;
	uint32_t symbol_table;
	uint32_t symbols_cnt;
	uint16_t opt_hdr_size;
	uint16_t flags;
}cdecl_attr_packed pe_file_header;
static_assert(sizeof(pe_file_header)==24,"pe_file_header size mismatch");
typedef struct pe_data_dir{
	uint32_t virt_addr;
	uint32_t size;
}cdecl_attr_packed pe_data_dir;
static_assert(sizeof(pe_data_dir)==8,"pe_data_dir size mismatch");
typedef struct pe_opt_header_common{
	uint16_t magic;
	uint8_t linker_major;
	uint8_t linker_minor;
	uint32_t code_size;
	uint32_t init_data_size;
	uint32_t uninit_data_size;
	uint32_t entrypoint;
	uint32_t code_base;
}cdecl_attr_packed pe_opt_header_common;
static_assert(sizeof(pe_opt_header_common)==24,"pe_opt_header_common size mismatch");
typedef struct pe_opt_header_pe32{
	uint32_t data_base;
	uint32_t image_base;
	uint32_t section_align;
	uint32_t file_align;
	uint16_t os_major;
	uint16_t os_minor;
	uint16_t image_major;
	uint16_t image_minor;
	uint16_t subsystem_major;
	uint16_t subsystem_minor;
	uint32_t win32_version;
	uint32_t image_size;
	uint32_t headers_size;
	uint32_t checksum;
	pe_subsystem subsystem:16;
	uint16_t dll_flags;
	uint32_t stack_reserve_size;
	uint32_t stack_commit_size;
	uint32_t heap_reserve_size;
	uint32_t heap_commit_size;
	uint32_t loader_flags;
	uint32_t data_dir_size;
	pe_data_dir data_dir[16];
}cdecl_attr_packed pe_opt_header_pe32;
static_assert(sizeof(pe_opt_header_pe32)==200,"pe_opt_header_pe32 size mismatch");
typedef struct pe_opt_header_pe64{
	uint64_t image_base;
	uint32_t section_align;
	uint32_t file_align;
	uint16_t os_major;
	uint16_t os_minor;
	uint16_t image_major;
	uint16_t image_minor;
	uint16_t subsystem_major;
	uint16_t subsystem_minor;
	uint32_t win32_version;
	uint32_t image_size;
	uint32_t headers_size;
	uint32_t checksum;
	pe_subsystem subsystem:16;
	uint16_t dll_flags;
	uint64_t stack_reserve_size;
	uint64_t stack_commit_size;
	uint64_t heap_reserve_size;
	uint64_t heap_commit_size;
	uint32_t loader_flags;
	uint32_t data_dir_size;
	pe_data_dir data_dir[16];
}cdecl_attr_packed pe_opt_header_pe64;
static_assert(sizeof(pe_opt_header_pe64)==216,"pe_opt_header_pe64 size mismatch");
typedef struct pe_opt_header{
	pe_opt_header_common common;
	union{
		pe_opt_header_pe32 pe32;
		pe_opt_header_pe64 pe64;
	}cdecl_attr_packed;
}cdecl_attr_packed pe_opt_header;
static_assert(sizeof(pe_opt_header)==240,"pe_opt_header size mismatch");
typedef struct pe_section_header{
	char name[8];
	union{
		uint32_t physical_address;
		uint32_t virtual_size;
	}cdecl_attr_packed misc;
	uint32_t virtual_address;
	uint32_t raw_data_size;
	uint32_t raw_data_ptr;
	uint32_t relocations_ptr;
	uint32_t line_numbers_ptr;
	uint16_t relocations_cnt;
	uint16_t line_numbers_cnt;
	uint32_t flags;
}cdecl_attr_packed pe_section_header;
static_assert(sizeof(pe_section_header)==40,"pe_section_header size mismatch");
typedef struct pe_symbol{
	union{
		char short_name[8];
		struct{
			uint32_t short_num;
			uint32_t long_num;
		}cdecl_attr_packed name;
		uint32_t long_name[2];
	}cdecl_attr_packed name;
	uint32_t value;
	uint16_t section;
	uint16_t type;
	uint8_t storage;
	uint8_t aux_symbols;
}cdecl_attr_packed pe_symbol;
static_assert(sizeof(pe_symbol)==18,"pe_symbol size mismatch");
typedef struct pe_relocation_base{
	uint32_t virtual_address;
	uint32_t block_size;
}cdecl_attr_packed pe_relocation_base;
static_assert(sizeof(pe_relocation_base)==8,"pe_relocation_base size mismatch");
typedef struct pe_relocation{
	uint16_t offset:12;
	pe_relocation_type type:4;
}cdecl_attr_packed pe_relocation;
static_assert(sizeof(pe_relocation)==2,"pe_relocation size mismatch");
extern bool pe_check_machine(pe_machine_type type);
extern bool pe_hdr_check_machine(pe_file_header*hdr);
extern bool pe_hdr_check_magic(pe_file_header*hdr);
extern bool pe_check_flag(pe_flag flags,pe_flag flag);
extern bool pe_hdr_check_flag(pe_file_header*hdr,pe_flag flag);
extern bool pe_sect_check_flag(pe_section_flag flags,pe_section_flag flag);
extern bool pe_hdr_sect_check_flag(pe_section_header*sect,pe_section_flag flag);
extern const char*pe_flag_to_string(pe_flag flag,const char*def);
extern const char*pe_machine_type_to_string(pe_machine_type machine,const char*def);
extern const char*pe_subsystem_to_string(pe_subsystem subsystem,const char*def);
extern const char*pe_section_flag_to_string(pe_section_flag subsystem,const char*def);
extern void pe_print_file_header(pe_file_header*hdr);
extern void pe_print_opt_header(pe_file_header*fh,pe_opt_header*hdr);
extern void pe_print_section_header(pe_section_header*sect);
extern void pe_print_section_headers(pe_section_header*sect,size_t len);
#endif //EFI_WRAPPER_PE_H
