#ifndef EFI_WRAPPER_LOAD_FILE2_H
#define EFI_WRAPPER_LOAD_FILE2_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"device_path.h"
#define EFI_LOAD_FILE2_PROTOCOL_GUID {0x4006c0c1,0xfcb3,0x403e,{0x99,0x6d,0x4a,0x6c,0x87,0x24,0xe0,0x6d}}
typedef struct efi_load_file2_protocol efi_load_file2_protocol;
typedef efi_status(efiapi*efi_load_file2)(
	efi_load_file2_protocol  *this,
	efi_device_path_protocol *path,
	bool                     boot_policy,
	uintn_t                  *size,
	void                     *buffer
);
struct efi_load_file2_protocol{
	efi_load_file2 load;
};
extern efi_guid gEfiLoadFile2ProtocolGuid;
#endif
