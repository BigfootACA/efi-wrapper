#include"efi/efi_context.h"
#include"../tables/internal.h"

const efi_runtime_services efi_runtime_services_template={
	.header={
		.sign        = EFI_RUNTIME_SERVICES_SIGNATURE,
		.revision    = EFI_RUNTIME_SERVICES_REVISION,
		.header_size = sizeof(efi_runtime_services),
		.crc32       = 0,
		.reserved    = 0,
	},
	.get_time                 = efi_get_time_impl,
	.set_time                 = efi_set_time_impl,
	.get_wakeup_time          = efi_get_wakeup_time_impl,
	.set_wakeup_time          = efi_set_wakeup_time_impl,
	.set_va_map               = efi_set_va_map_impl,
	.convert_pointer          = efi_convert_pointer_impl,
	.get_var                  = efi_get_var_impl,
	.get_next_var_name        = efi_get_next_var_name_impl,
	.set_var                  = efi_set_var_impl,
	.get_next_high_mono_count = efi_get_next_high_mono_count_impl,
	.reset_system             = efi_reset_system_impl,
	.update_capsule           = efi_update_capsule_impl,
	.query_capsule_caps       = efi_query_capsule_caps_impl,
	.query_var_info           = efi_query_var_info_impl,
};

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_runtime_services*rt;
	if(!ctx||!ins||!ins->cust_data)
		return efi_invalid_parameter;
	if(ctx->rt)return efi_already_started;
	rt=ins->cust_data;
	efi_table_calc_sum(rt,sizeof(efi_runtime_services));
	ctx->rt=rt;
	return efi_success;
}

static const efi_service efi_runtime_service={
	.guid      = &gEfiWrapperRuntimeServiceTableGuid,
	.name      = "RuntimeService",
	.start     = svc_start,
	.cust_len  = sizeof(efi_runtime_services),
	.cust_init = &efi_runtime_services_template,
};

DECLARE_EFI_SERVICE(efi_runtime_service)
