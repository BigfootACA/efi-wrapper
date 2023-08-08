//
// Created by bigfoot on 2023/5/12.
//

#include"efi/efi_context.h"
#include"efi/protocols/hii_config_routing.h"

static efiapi efi_status efi_hii_extract_config_impl(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      request,
	efi_string                            *progress,
	efi_string                            *results
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,request),
		call_arg(ptr,progress),
		call_arg(ptr,results),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_export_config_impl(
	const efi_hii_config_routing_protocol *proto,
	efi_string                            *results
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,results),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_route_config_impl(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      cfg,
	efi_string                            *progress
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,cfg),
		call_arg(ptr,progress),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_block_to_config_impl(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      request,
	const uint8_t                         *block,
	const uintn_t                         size,
	efi_string                            *config,
	efi_string                            *progress
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,request),
		call_arg(ptr,block),
		call_arg(unsigned,size),
		call_arg(ptr,config),
		call_arg(ptr,progress),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_config_to_block_impl(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      resp,
	uint8_t                               *block,
	uintn_t                               *size,
	efi_string                            *progress
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,resp),
		call_arg(ptr,block),
		call_arg(unsigned,size),
		call_arg(ptr,progress),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_alt_cfg_impl(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      resp,
	const efi_guid                        *guid,
	const efi_string                      name,
	const efi_device_path_protocol        *device_path,
	const uint16_t                        *alt_cfg_id,
	efi_string                            *alt_cfg_resp
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,resp),
		call_arg(guid,guid),
		call_arg(str16,name),
		call_arg(device_path,device_path),
		call_arg(ptr,alt_cfg_id),
		call_arg(ptr,alt_cfg_resp),
		NULL
	);
	log_return(efi_unsupported);
}

static efi_hii_config_routing_protocol cfg_routing_protocol={
	.extract_config  = efi_hii_extract_config_impl,
	.export_config   = efi_hii_export_config_impl,
	.route_config    = efi_hii_route_config_impl,
	.block_to_config = efi_hii_block_to_config_impl,
	.config_to_block = efi_hii_config_to_block_impl,
	.get_alt_config  = efi_hii_get_alt_cfg_impl,
};

efi_status efi_new_hii_cfg_routing(efi_handle*handle){
	if(!efi_current_ctx)return efi_not_ready;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiHiiConfigRoutingProtocolGuid,&cfg_routing_protocol,
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
	return efi_new_hii_cfg_routing(&ins->handle);
}

static const efi_service efi_hii_cfg_routing_service={
	.guid    = &gEfiHiiConfigRoutingProtocolGuid,
	.name    = "HiiConfigRoutingProtocol",
	.depends = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start   = svc_start,
	.disabled=true,
};

DECLARE_EFI_SERVICE(efi_hii_cfg_routing_service)
