//
// Created by bigfoot on 2023/5/12.
//

#include"efi/efi_context.h"
#include"efi/guids/mem_type_info.h"

static efi_memory_type_information mem_type_info[efi_max_memory_type+1]={
	{ efi_reserved_memory_type, 0 },
	{ efi_loader_code,          0 },
	{ efi_loader_data,          0 },
	{ efi_bs_code,              0 },
	{ efi_bs_data,              0 },
	{ efi_rs_code,              0 },
	{ efi_rs_data,              0 },
	{ efi_conventional_memory,  0 },
	{ efi_unusable_memory,      0 },
	{ efi_acpi_reclaim_memory,  0 },
	{ efi_acpi_memory_nvs,      0 },
	{ efi_mmio,                 0 },
	{ efi_mmio_port_space,      0 },
	{ efi_pal_code,             0 },
	{ efi_persistent_memory,    0 },
	{ efi_max_memory_type,      0 }
};

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins||!ins->cust_data)
		return efi_invalid_parameter;
	return ctx->bs->install_cfg_table(
		&gEfiMemoryTypeInformationGuid,
		ins->cust_data
	);
}

static const efi_service efi_mem_type_info_service={
	.guid      = &gEfiMemoryTypeInformationGuid,
	.name      = "MemoryTypeInformation",
	.depends   = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start     = svc_start,
	.cust_len  = sizeof(mem_type_info),
	.cust_init = &mem_type_info,
};

DECLARE_EFI_SERVICE(efi_mem_type_info_service)
