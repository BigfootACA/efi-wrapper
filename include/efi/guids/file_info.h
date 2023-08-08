//
// Created by bigfoot on 2023/5/12.
//

#ifndef EFI_WRAPPER_FILE_INFO_H
#define EFI_WRAPPER_FILE_INFO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_FILE_INFO_ID {0x09576e92,0x6d3f,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_FILE_SYSTEM_INFO_ID {0x09576e93,0x6d3f,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID {0xdb47d7d3,0xfe81,0x11d3,{0x9a,0x35,0x00,0x90,0x27,0x3f,0xc1,0x4d}}
typedef struct efi_file_info{
	uint64_t size;
	uint64_t file_size;
	uint64_t physical_size;
	efi_time ctime;
	efi_time atime;
	efi_time mtime;
	uint64_t attr;
	char16 file_name[0];
}efi_file_info;
typedef struct efi_file_system_info{
	uint64_t size;
	bool read_only;
	uint64_t vol_size;
	uint64_t free_space;
	uint32_t block_size;
	char16 vol_label[0];
}efi_file_system_info;
extern const efi_guid gEfiFileInfoGuid;
extern const efi_guid gEfiFileSystemInfoGuid;
extern const efi_guid gEfiFileSystemVolumeLabelInfoIdGuid;
#endif //EFI_WRAPPER_FILE_INFO_H
