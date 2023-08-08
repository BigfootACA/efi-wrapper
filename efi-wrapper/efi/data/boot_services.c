#include"efi/efi_context.h"
#include"../tables/internal.h"

const efi_boot_services efi_boot_services_template={
	.header={
		.sign        = EFI_BOOT_SERVICES_SIGNATURE,
		.revision    = EFI_BOOT_SERVICES_REVISION,
		.header_size = sizeof(efi_boot_services),
		.crc32       = 0,
		.reserved    = 0,
	},
	.raise_tpl                           = efi_raise_tpl_impl,
	.restore_tpl                         = efi_restore_tpl_impl,
	.alloc_pages                         = efi_allocate_pages_impl,
	.free_pages                          = efi_free_pages_impl,
	.get_mem_map                         = efi_get_memory_map_impl,
	.allocate_pool                       = efi_allocate_pool_impl,
	.free_pool                           = efi_free_pool_impl,
	.create_event                        = efi_create_event_impl,
	.set_timer                           = efi_set_timer_impl,
	.wait_for_event                      = efi_wait_for_event_impl,
	.signal_event                        = efi_signal_event_impl,
	.close_event                         = efi_close_event_impl,
	.check_event                         = efi_check_event_impl,
	.install_proto_interface             = efi_install_protocol_interface_impl,
	.reinstall_proto_interface           = efi_reinstall_protocol_interface_impl,
	.uninstall_proto_interface           = efi_uninstall_protocol_interface_impl,
	.handle_proto                        = efi_handle_protocol_impl,
	.reserved                            = NULL,
	.register_proto_notify               = efi_register_protocol_notify_impl,
	.locate_handle                       = efi_locate_handle_impl,
	.locate_device_path                  = efi_locate_device_path_impl,
	.install_cfg_table                   = efi_install_configuration_table_impl,
	.load_image                          = efi_image_load_impl,
	.start_image                         = efi_image_start_impl,
	.exit                                = efi_exit_impl,
	.unload_image                        = efi_image_unload_impl,
	.exit_boot_services                  = efi_exit_boot_services_impl,
	.get_next_mono_count                 = efi_get_next_monotonic_count_impl,
	.stall                               = efi_stall_impl,
	.set_watchdog_timer                  = efi_set_watchdog_timer_impl,
	.connect_controller                  = efi_connect_controller_impl,
	.disconnect_controller               = efi_disconnect_controller_impl,
	.open_proto                          = efi_open_protocol_impl,
	.close_proto                         = efi_close_protocol_impl,
	.open_proto_info                     = efi_open_protocol_information_impl,
	.proto_per_handle                    = efi_protocols_per_handle_impl,
	.locate_handle_buffer                = efi_locate_handle_buffer_impl,
	.locate_proto                        = efi_locate_protocol_impl,
	.install_multiple_proto_interfaces   = efi_install_multiple_protocol_interfaces_impl,
	.uninstall_multiple_proto_interfaces = efi_uninstall_multiple_protocol_interfaces_impl,
	.calculate_crc32                     = efi_calculate_crc32_impl,
	.copy_mem                            = efi_copy_mem_impl,
	.set_mem                             = efi_set_mem_impl,
	.create_event_ex                     = efi_create_event_ex_impl,
};

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_boot_services*bs;
	if(!ctx||!ins||!ins->cust_data)
		return efi_invalid_parameter;
	if(ctx->bs)return efi_already_started;
	bs=ins->cust_data;
	efi_table_calc_sum(bs,sizeof(efi_boot_services));
	ctx->bs=bs;
	return efi_success;
}

static const efi_service efi_boot_service={
	.guid      = &gEfiWrapperBootServiceTableGuid,
	.name      = "BootServices",
	.start     = svc_start,
	.cust_len  = sizeof(efi_boot_services),
	.cust_init = &efi_boot_services_template,
};

DECLARE_EFI_SERVICE(efi_boot_service)
