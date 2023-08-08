#ifndef EFI_WRAPPER_INTERNAL_H
#define EFI_WRAPPER_INTERNAL_H
#include<errno.h>
#include<wchar.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/poll.h>
#include<sys/mman.h>
#include"defines.h"
#include"efi/efi_spec.h"
#include"efi/efi_string.h"
#include"efi/efi_context.h"
#include"efi/device_path.h"
#include"efi/protocols/component_name.h"
#include"efi/protocols/simple_text_output.h"
#include"efi/protocols/simple_text_input.h"
#include"efi/protocols/simple_text_input_ex.h"
typedef struct efi_simple_text_output_protocol_private{
	efi_simple_text_output_protocol protocol;
	efi_guid magic;
}efi_simple_text_output_protocol_private;
typedef struct efi_simple_text_input_protocol_private{
	efi_simple_text_input_protocol protocol;
	efi_guid magic;
}efi_simple_text_input_protocol_private;
typedef struct efi_simple_text_input_ex_protocol_private{
	efi_simple_text_input_ex_protocol protocol;
	efi_guid magic;
}efi_simple_text_input_ex_protocol_private;
typedef struct efi_input_key_escape_map{
	char escape[12];
	efi_input_key map;
}efi_input_key_escape_map;
extern efi_input_key_escape_map escape_map[];
#endif //EFI_WRAPPER_INTERNAL_H
