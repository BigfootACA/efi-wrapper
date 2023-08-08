#ifndef EFI_WRAPPER_LOAD_FILE_H
#define EFI_WRAPPER_LOAD_FILE_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"device_path.h"
#define EFI_LOAD_FILE_PROTOCOL_GUID {0x56ec3091,0x954c,0x11d2,{0x8e,0x3f,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
typedef struct efi_load_file_protocol efi_load_file_protocol;
typedef efi_load_file_protocol efi_load_file_interface;
typedef efi_status(efiapi*efi_load_file)(
	efi_load_file_protocol   *this,
	efi_device_path_protocol *path,
	bool                     boot_policy,
	uintn_t                  *size,
	void                     *buffer
);
struct efi_load_file_protocol{
	efi_load_file load;
};
extern efi_guid gEfiLoadFileProtocolGuid;
#endif
