//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_SYSTEM_TABLE_H
#define EFI_WRAPPER_SYSTEM_TABLE_H
#include "efi_types.h"
#include "runtime_services.h"
#include "boot_services.h"
#include "protocols/simple_text_input.h"
#include "protocols/simple_text_output.h"

#define EFI_SYSTEM_TABLE_SIGNATURE      SIGNATURE_64 ('I','B','I',' ','S','Y','S','T')
#define EFI_SYSTEM_TABLE_REVISION       EFI_SPECIFICATION_VERSION
typedef struct efi_configuration_table{
	efi_guid guid;
	void     *table;
}efi_configuration_table;
typedef struct efi_system_table{
	efi_table_header                  header;
	char16                            *firmware_vendor;
	uint32_t                          firmware_revision;
	efi_handle                        con_stdin_handle;
	efi_simple_text_input_protocol    *con_stdin;
	efi_handle                        con_stdout_handle;
	efi_simple_text_output_protocol   *con_stdout;
	efi_handle                        con_stderr_handle;
	efi_simple_text_output_protocol   *con_stderr;
	efi_runtime_services              *runtime_services;
	efi_boot_services                 *boot_services;
	uintn_t                           entries;
	efi_configuration_table           *cfg_table;
}efi_system_table;
#endif //EFI_WRAPPER_SYSTEM_TABLE_H
