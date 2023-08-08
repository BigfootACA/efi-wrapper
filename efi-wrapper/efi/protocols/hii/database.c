//
// Created by bigfoot on 2023/5/11.
//

#include"efi/efi_context.h"
#include"efi/protocols/hii_database.h"

static efiapi efi_status efi_hii_database_notify_impl(
	uint8_t                      type,
	const efi_guid               *guid,
	const efi_hii_package_header *package,
	efi_hii_handle               handle,
	efi_hii_database_notify_type notify
){
	log_call(
		call_arg(hex,type),
		call_arg(guid,guid),
		call_arg(ptr,package),
		call_arg(ptr,handle),
		call_arg(hex,notify),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_new_pack_impl(
	const efi_hii_database_protocol   *proto,
	const efi_hii_package_list_header *list,
	efi_handle                        driver,
	efi_hii_handle                    *handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		call_arg(handle,driver),
		call_arg(ptr,list),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_remove_pack_impl(
	const efi_hii_database_protocol *proto,
	efi_hii_handle                  handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_update_pack_impl(
	const efi_hii_database_protocol   *proto,
	efi_hii_handle                    handle,
	const efi_hii_package_list_header *list
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		call_arg(ptr,list),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_list_packs_impl(
	const efi_hii_database_protocol *proto,
	uint8_t                         type,
	const efi_guid                  *guid,
	uintn_t                         *length,
	efi_hii_handle                  *handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,type),
		call_arg(guid,guid),
		call_arg(ptr,length),
		call_arg(ptr,handle),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_export_packs_impl(
	const efi_hii_database_protocol *proto,
	efi_hii_handle                  handle,
	uintn_t                         *size,
	efi_hii_package_list_header     *buffer
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		call_arg(ptr,size),
		call_arg(ptr,buffer),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_register_notify_impl(
	const efi_hii_database_protocol *proto,
	uint8_t                         type,
	const efi_guid                  *guid,
	efi_hii_database_notify         func,
	efi_hii_database_notify_type    notify,
	efi_handle                      *handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,type),
		call_arg(guid,guid),
		call_arg(ptr,func),
		call_arg(hex,notify),
		call_arg(ptr,handle),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_unregister_notify_impl(
	const efi_hii_database_protocol *proto,
	efi_handle                       handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(handle,handle),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_find_keyboard_layouts_impl(
	const efi_hii_database_protocol *proto,
	uint16_t                        *length,
	efi_guid                        *buffer
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,length),
		call_arg(ptr,buffer),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_keyboard_layout_impl(
	const efi_hii_database_protocol *proto,
	const efi_guid                  *guid,
	uint16_t                        *length,
	efi_hii_keyboard_layout         *layout
){
	log_call(
		call_arg(ptr,proto),
		call_arg(guid,guid),
		call_arg(ptr,length),
		call_arg(ptr,layout),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_set_keyboard_layout_impl(
	const efi_hii_database_protocol *proto,
	const efi_guid                  *guid
){
	log_call(
		call_arg(ptr,proto),
		call_arg(guid,guid),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_database_get_pack_handle_impl(
	const efi_hii_database_protocol *proto,
	efi_hii_handle                  list,
	efi_handle                      *driver
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,list),
		call_arg(ptr,driver),
		NULL
	);
	log_return(efi_unsupported);
}

static efi_hii_database_protocol db_protocol={
	.new_package_list          = efi_hii_database_new_pack_impl,
	.remove_package_list       = efi_hii_database_remove_pack_impl,
	.update_package_list       = efi_hii_database_update_pack_impl,
	.list_package_lists        = efi_hii_database_list_packs_impl,
	.export_package_lists      = efi_hii_database_export_packs_impl,
	.register_package_notify   = efi_hii_database_register_notify_impl,
	.unregister_package_notify = efi_hii_database_unregister_notify_impl,
	.find_keyboard_layouts     = efi_hii_find_keyboard_layouts_impl,
	.get_keyboard_layout       = efi_hii_get_keyboard_layout_impl,
	.set_keyboard_layout       = efi_hii_set_keyboard_layout_impl,
	.get_package_list_handle   = efi_hii_database_get_pack_handle_impl,
};

efi_status efi_new_hii_database(efi_handle*handle){
	if(!efi_current_ctx)return efi_not_ready;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiHiiDatabaseProtocolGuid,&db_protocol,
		NULL
	);
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	return efi_new_hii_database(&ins->handle);
}

static const efi_service efi_hii_database_service={
	.guid    = &gEfiHiiDatabaseProtocolGuid,
	.name    = "HiiDatabaseProtocol",
	.depends = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start   = svc_start,
	.disabled=true,
};

DECLARE_EFI_SERVICE(efi_hii_database_service)
