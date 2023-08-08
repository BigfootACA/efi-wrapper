//
// Created by bigfoot on 2023/5/12.
//

#ifndef EFI_WRAPPER_MEM_TYPE_INFO_H
#define EFI_WRAPPER_MEM_TYPE_INFO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_MEMORY_TYPE_INFORMATION_GUID {0x4c19049f,0x4137,0x4dd3,{0x9c,0x10,0x8b,0x97,0xa8,0x3f,0xfd,0xfa}}
#define EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME L"MemoryTypeInformation"
typedef struct efi_memory_type_information{
	uint32_t type;
	uint32_t pages;
}efi_memory_type_information;
extern const efi_guid gEfiMemoryTypeInformationGuid;
#endif //EFI_WRAPPER_MEM_TYPE_INFO_H
