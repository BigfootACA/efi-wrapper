#ifndef EFI_WRAPPER_SECURITY2_H
#define EFI_WRAPPER_SECURITY2_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"device_path.h"
#define EFI_SECURITY2_ARCH_PROTOCOL_GUID {0x94ab2f58,0x1438,0x4ef1,{0x91,0x52,0x18,0x94,0x1a,0x3a,0x0e,0x68}}
typedef struct efi_security2_arch_protocol efi_security2_arch_protocol;
typedef efi_status(efiapi*efi_security2_file_authentication)(
	const efi_security2_arch_protocol*this,
	const efi_device_path_protocol*file,
	void*buffer,
	uintn_t size,
	bool boot_policy
);
struct efi_security2_arch_protocol{
	efi_security2_file_authentication file_auth;
};
extern efi_guid gEfiSecurity2ArchProtocolGuid;
#endif
