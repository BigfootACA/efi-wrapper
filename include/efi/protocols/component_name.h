//
// Created by bigfoot on 2023/5/16.
//

#ifndef EFI_WRAPPER_COMPONENT_NAME_H
#define EFI_WRAPPER_COMPONENT_NAME_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_COMPONENT_NAME_PROTOCOL_GUID {0x107a772c,0xd5e1,0x11d4,{0x9a,0x46,0x00,0x90,0x27,0x3f,0xc1,0x4d}}
#define EFI_COMPONENT_NAME2_PROTOCOL_GUID {0x6a7a5cff,0xe8d9,0x4f70,{0xba,0xda,0x75,0xab,0x30,0x25,0xce,0x14}}
typedef struct efi_component_name_protocol efi_component_name_protocol;
typedef efi_component_name_protocol efi_component_name2_protocol;
typedef efi_status(efiapi*efi_component_name_get_driver_name)(
	efi_component_name_protocol *proto,
	char8                       *lang,
	char16                      **name
);
typedef efi_status(efiapi*efi_component_name_get_controller_name)(
	efi_component_name_protocol *proto,
	efi_handle                  controller,
	efi_handle                  child,
	char8                       *lang,
	char16                      **name
);
struct efi_component_name_protocol {
	efi_component_name_get_driver_name      get_driver_name;
	efi_component_name_get_controller_name  get_controller_name;
	char8                                   *supported_languages;
};
extern const efi_guid gEfiComponentNameProtocolGuid;
extern const efi_guid gEfiComponentName2ProtocolGuid;
#endif //EFI_WRAPPER_COMPONENT_NAME_H
