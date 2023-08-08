//
// Created by bigfoot on 2023/5/9.
//

#ifndef EFI_WRAPPER_DRIVER_BINDING_H
#define EFI_WRAPPER_DRIVER_BINDING_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"device_path.h"
#define EFI_DRIVER_BINDING_PROTOCOL_GUID {0x18a031ab,0xb443,0x4d1a,{0xa5,0xc0,0x0c,0x09,0x26,0x1e,0x9f,0x71}}
typedef struct efi_driver_binding_protocol efi_driver_binding_protocol;
typedef efi_status(efiapi*efi_driver_binding_supported)(
	efi_driver_binding_protocol *proto,
	efi_handle                  controller,
	efi_device_path_protocol    *remaining
);
typedef efi_status(efiapi*efi_driver_binding_start)(
	efi_driver_binding_protocol *proto,
	efi_handle                  controller,
	efi_device_path_protocol    *remaining
);
typedef efi_status(efiapi*efi_driver_binding_stop)(
	efi_driver_binding_protocol *proto,
	efi_handle                  controller,
	uintn_t                     children,
	efi_handle                  *buffer
);
struct efi_driver_binding_protocol{
	efi_driver_binding_supported supported;
	efi_driver_binding_start     start;
	efi_driver_binding_stop      stop;
	uint32_t                     version;
	efi_handle                   image;
	efi_handle                   driver_binding;
};
extern const efi_guid gEfiDriverBindingProtocolGuid;
#endif //EFI_WRAPPER_DRIVER_BINDING_H
