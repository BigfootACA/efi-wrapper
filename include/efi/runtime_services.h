//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_RUNTIME_SERVICES_H
#define EFI_WRAPPER_RUNTIME_SERVICES_H
#include "efi_types.h"

#define EFI_RUNTIME_SERVICES_SIGNATURE  SIGNATURE_64 ('R','U','N','T','S','E','R','V')
#define EFI_RUNTIME_SERVICES_REVISION   EFI_SPECIFICATION_VERSION
#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET          0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE         0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET                0x00040000
typedef enum efi_reset_type{
	RESET_COLD                 = 0x00,
	RESET_WARM                 = 0x01,
	RESET_SHUTDOWN             = 0x02,
	RESET_PLATFORM_SPECIFIC    = 0x03,
}efi_reset_type;
typedef struct efi_time_capabilities{
	uint32_t    resolution;
	uint32_t    accuracy;
	bool        sets_to_zero;
}efi_time_capabilities;
typedef struct efi_capsule_block_descriptor{
	uint64_t length;
	union {
		efi_address data_block;
		efi_address continuation_pointer;
	}unions;
}efi_capsule_block_descriptor;
typedef struct efi_capsule_header{
	efi_guid guid;
	uint32_t header_size;
	uint32_t flags;
	uint32_t image_size;
}efi_capsule_header;
typedef struct efi_capsule_table{
	uint32_t capsule_array_number;
	void*    capsule_ptr[1];
}efi_capsule_table;
typedef efiapi efi_status(*efi_get_time)(
	efi_time              *time,
	efi_time_capabilities *caps
);
typedef efiapi efi_status(*efi_set_time)(efi_time*time);
typedef efiapi efi_status(*efi_get_wakeup_time)(
	bool     *enabled,
	bool     *pending,
	efi_time *time
);
typedef efiapi efi_status(*efi_set_wakeup_time)(
	bool     enable,
	efi_time *time
);
typedef efiapi efi_status(*efi_set_virtual_address_map)(
	uintn_t                map_size,
	uintn_t                desc_size,
	uint32_t               desc_ver,
	efi_memory_descriptor  *virt_map
);
typedef efiapi efi_status(*efi_convert_pointer)(
	uintn_t debug_disposition,
	void    **address
);
typedef efiapi efi_status(*efi_get_variable)(
	const char16   *variable_name,
	const efi_guid *vendor_guid,
	uint32_t       *attributes,
	uintn_t        *data_size,
	void           *data
);
typedef efiapi efi_status(*efi_get_next_variable_name)(
	uintn_t  *variable_name_size,
	char16   *variable_name,
	const efi_guid *vendor_guid
);
typedef efiapi efi_status(*efi_set_variable)(
	const char16   *variable_name,
	const efi_guid *vendor_guid,
	uint32_t       attributes,
	uintn_t        data_size,
	void           *data
);
typedef efiapi void(*efi_reset_system)(
	efi_reset_type type,
	efi_status     status,
	uintn_t        size,
	void           *data
);
typedef efiapi efi_status(*efi_get_next_high_mono_count)(uint32_t*count);
typedef efiapi efi_status(*efi_update_capsule)(
	efi_capsule_header**header_array,
	uintn_t count,
	efi_address scatter_gather_list
);
typedef efiapi efi_status(*efi_query_capsule_capabilities)(
	efi_capsule_header **header_array,
	uintn_t            count,
	uint64_t           *max_size,
	efi_reset_type     *reset_type
);
typedef efiapi efi_status(*efi_query_variable_info)(
	uint32_t attributes,
	uint64_t *max_var_storage,
	uint64_t *remain_var_storage,
	uint64_t *max_var_size
);
typedef struct efi_runtime_services{
	  efi_table_header                header;
	  efi_get_time                    get_time;
	  efi_set_time                    set_time;
	  efi_get_wakeup_time             get_wakeup_time;
	  efi_set_wakeup_time             set_wakeup_time;
	  efi_set_virtual_address_map     set_va_map;
	  efi_convert_pointer             convert_pointer;
	  efi_get_variable                get_var;
	  efi_get_next_variable_name      get_next_var_name;
	  efi_set_variable                set_var;
	  efi_get_next_high_mono_count    get_next_high_mono_count;
	  efi_reset_system                reset_system;
	  efi_update_capsule              update_capsule;
	  efi_query_capsule_capabilities  query_capsule_caps;
	  efi_query_variable_info         query_var_info;
}efi_runtime_services;
#endif //EFI_WRAPPER_RUNTIME_SERVICES_H
