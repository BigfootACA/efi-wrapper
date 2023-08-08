#ifndef EFI_WRAPPER_SECURITY_H
#define EFI_WRAPPER_SECURITY_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"device_path.h"
#define EFI_SECURITY_ARCH_PROTOCOL_GUID {0xA46423E3,0x4617,0x49f1,{0xB9,0xFF,0xD1,0xBF,0xA9,0x11,0x58,0x39}}
typedef struct efi_security_arch_protocol efi_security_arch_protocol;
typedef efi_status(efiapi*efi_security_file_authentication_state)(
	const efi_security_arch_protocol*this,
	uint32_t auth_status,
	const efi_device_path_protocol*file
);
struct efi_security_arch_protocol{
	efi_security_file_authentication_state file_auth_state;
};
extern efi_guid gEfiSecurityArchProtocolGuid;
#endif
