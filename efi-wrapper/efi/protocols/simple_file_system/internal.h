#ifndef EFI_WRAPPER_INTERNAL_H
#define EFI_WRAPPER_INTERNAL_H
#include<stdio.h>
#include<errno.h>
#include<wchar.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"defines.h"
#include"efi/efi_spec.h"
#include"efi/efi_string.h"
#include"efi/efi_context.h"
#include"efi/device_path.h"
#include"efi/guids/file_info.h"
#include"efi/protocols/component_name.h"
#include"efi/protocols/simple_filesystem.h"
typedef struct efi_simple_file_system_protocol_private{
	efi_simple_file_system_protocol protocol;
	efi_guid magic;
}efi_simple_file_system_protocol_private;
typedef struct efi_file_protocol_private{
	efi_file_protocol protocol;
	efi_guid magic;
}efi_file_protocol_private;
#endif //EFI_WRAPPER_INTERNAL_H
