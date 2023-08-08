//
// Created by bigfoot on 2023/5/7.
//

#ifndef EFI_WRAPPER_LOADED_IMAGE_H
#define EFI_WRAPPER_LOADED_IMAGE_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"../system_table.h"
#include"device_path.h"
#define EFI_LOADED_IMAGE_PROTOCOL_GUID             {0x5b1b31a1,0x9562,0x11d2,{0x8e,0x3f,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID {0xbc62157e,0x3e33,0x4fec,{0x99,0x20,0x2d,0x3b,0x36,0xd7,0x50,0xdf}}
#define EFI_LOADED_IMAGE_PROTOCOL_REVISION1 0x1000
#define EFI_LOADED_IMAGE_INFORMATION_REVISION EFI_LOADED_IMAGE_PROTOCOL_REVISION1
typedef struct efi_loaded_image_protocol{
	uint32_t                 revision;
	efi_handle               parent_handle;
	efi_system_table         *system_table;
	efi_handle               device_handle;
	efi_device_path_protocol *file_path;
	void                     *reserved;
	uint32_t                 load_options_size;
	void                     *load_options;
	void                     *image_base;
	uint64_t                 image_size;
	efi_memory_type          image_code_type;
	efi_memory_type          image_data_type;
	efi_image_unload         unload;
}efi_loaded_image_protocol;
typedef efi_loaded_image_protocol efi_loaded_image;
extern const efi_guid gEfiLoadedImageProtocolGuid;
extern const efi_guid gEfiLoadedImageDevicePathProtocolGuid;
#endif //EFI_WRAPPER_LOADED_IMAGE_H
