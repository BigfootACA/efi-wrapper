#include"efi/efi_context.h"
#include"../tables/internal.h"

const efi_dxe_services efi_dxe_services_template={
	.header={
		.sign        = EFI_DXE_SERVICES_SIGNATURE,
		.revision    = EFI_DXE_SERVICES_REVISION,
		.header_size = sizeof(efi_dxe_services),
		.crc32       = 0,
		.reserved    = 0,
	},
	.add_mem_space      = efi_add_mem_space_impl,
	.alloc_mem_space    = efi_alloc_mem_space_impl,
	.free_mem_space     = efi_free_mem_space_impl,
	.remove_mem_space   = efi_remove_mem_space_impl,
	.get_mem_space_desc = efi_get_mem_space_desc_impl,
	.set_mem_space_attrs= efi_set_mem_space_attrs_impl,
	.get_mem_space_map  = efi_get_mem_space_map_impl,
	.add_io_space       = efi_add_io_space_impl,
	.alloc_io_space     = efi_alloc_io_space_impl,
	.free_io_space      = efi_free_io_space_impl,
	.remove_io_space    = efi_remove_io_space_impl,
	.get_io_space_desc  = efi_get_io_space_desc_impl,
	.get_io_space_map   = efi_get_io_space_map_impl,
	.dispatch           = efi_dispatch_impl,
	.schedule           = efi_schedule_impl,
	.trust              = efi_trust_impl,
	.proc_fv            = efi_proc_fv_impl,
	.set_mem_space_caps = efi_set_mem_space_caps_impl,
};

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st;
	efi_dxe_services*ds;
	if(!ctx||!ins||!ins->cust_data)
		return efi_invalid_parameter;
	if(!ctx->bs||!ctx->st)return efi_not_ready;
	if(ctx->ds)return efi_already_started;
	ds=ins->cust_data;
	efi_table_calc_sum(ds,sizeof(efi_dxe_services));
	st=ctx->bs->install_cfg_table(&gEfiDxeServicesTableGuid,ds);
	if(efi_error(st))return st;
	ctx->ds=ds;
	return efi_success;
}

static const efi_service efi_dxe_service={
	.guid      = &gEfiDxeServicesTableGuid,
	.name      = "DxeServices",
	.depends   = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start     = svc_start,
	.cust_len  = sizeof(efi_dxe_services),
	.cust_init = &efi_dxe_services_template,
};

DECLARE_EFI_SERVICE(efi_dxe_service)
