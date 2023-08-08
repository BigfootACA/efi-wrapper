//
// Created by bigfoot on 2023/5/12.
//

#ifndef EFI_WRAPPER_HOB_LIST_H
#define EFI_WRAPPER_HOB_LIST_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_HOB_LIST_GUID {0x7739f24c,0x93d7,0x11d4,{0x9a,0x3a,0x0,0x90,0x27,0x3f,0xc1,0x4d}}
#define EFI_HOB_HANDOFF_TABLE_VERSION 0x0009
typedef enum efi_hob_type{
	efi_hob_type_handoff             = 0x0001,
	efi_hob_memory_allocation   = 0x0002,
	efi_hob_resource_descriptor = 0x0003,
	efi_hob_guid_extension      = 0x0004,
	efi_hob_fv                  = 0x0005,
	efi_hob_cpu                 = 0x0006,
	efi_hob_memory_pool         = 0x0007,
	efi_hob_fv2                 = 0x0009,
	efi_hob_load_peim_unused    = 0x000A,
	efi_hob_uefi_capsule        = 0x000B,
	efi_hob_fv3                 = 0x000C,
	efi_hob_unused              = 0xFFFE,
	efi_hob_list_end            = 0xFFFF,
} cdecl_attr_packed efi_hob_type;
static_assert(sizeof(efi_hob_type)==2,"efi_hob_type size mismatch");
typedef enum efi_boot_mode{
	boot_with_full_cfg           = 0x00,
	boot_with_min_cfg            = 0x01,
	boot_assuming_no_cfg_changes = 0x02,
	boot_with_full_cfg_plus_diag = 0x03,
	boot_with_def_settings       = 0x04,
	boot_on_s4_resume            = 0x05,
	boot_on_s5_resume            = 0x06,
	boot_with_mfg_mode_settings  = 0x07,
	boot_on_s2_resume            = 0x10,
	boot_on_s3_resume            = 0x11,
	boot_on_flash_update         = 0x12,
	boot_in_recovery_mode        = 0x20,
	boot_mode_max                = UINT32_MAX,
}cdecl_attr_packed efi_boot_mode;
static_assert(sizeof(efi_boot_mode)==4,"efi_boot_mode size mismatch");
typedef struct efi_hob_generic_header{
	efi_hob_type type:16;
	uint16_t length;
	uint32_t reserved;
}cdecl_attr_packed efi_hob_generic_header;
static_assert(sizeof(efi_hob_generic_header)==8,"efi_hob_generic_header size mismatch");
typedef struct efi_hob_handoff_info_table{
	efi_hob_generic_header header;
	uint32_t               version;
	efi_boot_mode          boot_mode;
	efi_address            mem_top;
	efi_address            mem_bottom;
	efi_address            free_mem_top;
	efi_address            free_mem_bottom;
	efi_address            end_of_hob;
}cdecl_attr_packed efi_hob_handoff_info_table;
#if WORD_SIZE == 8
static_assert(sizeof(efi_hob_handoff_info_table)==56,"efi_hob_handoff_info_table size mismatch");
#elif WORD_SIZE == 4
static_assert(sizeof(efi_hob_handoff_info_table)==36,"efi_hob_handoff_info_table size mismatch");
#else
#error unknown word size
#endif
typedef struct efi_hob_guid_type{
	efi_hob_generic_header header;
	efi_guid               name;
}cdecl_attr_packed efi_hob_guid_type;
static_assert(sizeof(efi_hob_guid_type)==24,"efi_hob_handoff_info_table size mismatch");
extern const efi_guid gEfiHobListGuid;
#endif //EFI_WRAPPER_HOB_LIST_H
