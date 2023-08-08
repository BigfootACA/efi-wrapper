#include"efi/efi_context.h"

const efi_system_table efi_system_table_template={
	.header={
		.sign        = EFI_SYSTEM_TABLE_SIGNATURE,
		.revision    = EFI_SYSTEM_TABLE_REVISION,
		.header_size = sizeof(efi_system_table),
		.crc32       = 0,
		.reserved    = 0,
	},
	.firmware_vendor   = (char16*)L"EFI Wrapper for Linux",
	.firmware_revision = 0x10000,
	.con_stdin_handle  = NULL,
	.con_stdin         = NULL,
	.con_stdout_handle = NULL,
	.con_stdout        = NULL,
	.con_stderr_handle = NULL,
	.con_stderr        = NULL,
	.runtime_services  = NULL,
	.boot_services     = NULL,
	.entries           = 0,
	.cfg_table         = NULL,
};

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_system_table*st;
	if(!ctx||!ins||!ins->cust_data)
		return efi_invalid_parameter;
	if(!ctx->bs||!ctx->rt)return efi_not_ready;
	if(ctx->st)return efi_already_started;
	st=ins->cust_data;
	st->boot_services=ctx->bs;
	st->runtime_services=ctx->rt;
	efi_table_calc_sum(st,sizeof(efi_system_table));
	ctx->st=ins->cust_data;
	return efi_success;
}
static const efi_service efi_system_table_service={
	.guid      = &gEfiWrapperSystemTableGuid,
	.name      = "SystemTable",
	.depends   = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperRuntimeServiceTableGuid,
		NULL
	},
	.start     = svc_start,
	.cust_len  = sizeof(efi_system_table),
	.cust_init = &efi_system_table_template,
};

DECLARE_EFI_SERVICE(efi_system_table_service)
