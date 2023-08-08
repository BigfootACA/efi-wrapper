//
// Created by bigfoot on 2023/5/11.
//

#ifndef EFI_WRAPPER_DEVICE_PATH_UTILS_H
#define EFI_WRAPPER_DEVICE_PATH_UTILS_H
#include"device_path.h"
#define EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID {0x0379be4e,0xd706,0x437d,{0xb0,0x37,0xed,0xb8,0x2f,0xb7,0x72,0xa4}}
typedef uintn_t(efiapi*efi_device_path_utils_get_device_path_size)(
	const efi_device_path_protocol*dp
);
typedef efi_device_path_protocol*(efiapi*efi_device_path_utils_dup_device_path)(
	const efi_device_path_protocol*dp
);
typedef efi_device_path_protocol*(efiapi*efi_device_path_utils_append_path)(
	const efi_device_path_protocol*src1,
	const efi_device_path_protocol*src2
);
typedef efi_device_path_protocol*(efiapi*efi_device_path_utils_append_node)(
	const efi_device_path_protocol*dp,
	const efi_device_path_protocol*node
);
typedef efi_device_path_protocol*(efiapi*efi_device_path_utils_append_instance)(
	const efi_device_path_protocol*dp,
	const efi_device_path_protocol*instance
);
typedef efi_device_path_protocol*(efiapi*efi_device_path_utils_get_next_instance)(
	efi_device_path_protocol **instance,
	uintn_t                  *size
);
typedef efi_device_path_protocol*(efiapi*efi_device_path_utils_create_node)(
	uint8_t  type,
	uint8_t  sub_type,
	uint16_t length
);
typedef bool(efiapi*efi_device_path_utils_is_multi_instance)(
	const efi_device_path_protocol*dp
);
typedef struct efi_device_path_utilities_protocol{
	efi_device_path_utils_get_device_path_size get_device_path_size;
	efi_device_path_utils_dup_device_path      duplicate_device_path;
	efi_device_path_utils_append_path          append_device_path;
	efi_device_path_utils_append_node          append_device_node;
	efi_device_path_utils_append_instance      append_device_path_instance;
	efi_device_path_utils_get_next_instance    get_next_device_path_instance;
	efi_device_path_utils_is_multi_instance    is_device_path_multi_instance;
	efi_device_path_utils_create_node          create_device_node;
}efi_device_path_utilities_protocol;
extern const efi_guid gEfiDevicePathUtilitiesProtocolGuid;
#endif //EFI_WRAPPER_DEVICE_PATH_UTILS_H
