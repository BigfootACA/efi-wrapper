//
// Created by bigfoot on 2023/5/11.
//

#include"efi/device_path.h"
#include"efi/efi_context.h"
#include"efi/protocols/device_path_utils.h"

static efiapi uintn_t get_device_path_size_impl(
	const efi_device_path*dp
){
	log_call(
		call_arg(device_path,dp),
		NULL
	);
	log_return_unsigned(uintn_t,efi_device_path_get_size(dp));
}

static efiapi efi_device_path*duplicate_device_path_impl(
	const efi_device_path*dp
){
	log_call(
		call_arg(device_path,dp),
		NULL
	);
	if(!efi_current_ctx)log_return_ptr(NULL);
	log_return_ptr(efi_device_path_dup(
		efi_current_ctx->mem[efi_bs_data].pool,dp
	));
}

static efiapi efi_device_path*append_device_path_impl(
	const efi_device_path*src1,
	const efi_device_path*src2
){
	log_call(
		call_arg(device_path,src1),
		call_arg(device_path,src2),
		NULL
	);
	if(!efi_current_ctx)log_return_ptr(NULL);
	log_return_ptr(efi_device_path_append(
		efi_current_ctx->mem[efi_bs_data].pool,
		src1,src2
	));
}

static efiapi efi_device_path*append_device_node_impl(
	const efi_device_path*dp,
	const efi_device_path*node
){
	log_call(
		call_arg(device_path,dp),
		call_arg(device_path,node),
		NULL
	);
	if(!efi_current_ctx)log_return_ptr(NULL);
	log_return_ptr(efi_device_path_node_append(
		efi_current_ctx->mem[efi_bs_data].pool,dp,node
	));
}

static efiapi efi_device_path*append_device_path_instance_impl(
	const efi_device_path*dp,
	const efi_device_path*instance
){
	log_call(
		call_arg(device_path,dp),
		call_arg(device_path,instance),
		NULL
	);
	if(!efi_current_ctx)log_return_ptr(NULL);
	log_return_ptr(efi_device_path_instance_append(
		efi_current_ctx->mem[efi_bs_data].pool,
		dp,instance
	));
}

static efiapi efi_device_path*get_next_device_path_instance_impl(
	efi_device_path**instance,
	size_t*size
){
	log_call(
		call_arg(ptr,instance),
		call_arg(ptr,size),
		NULL
	);
	if(!efi_current_ctx)log_return_ptr(NULL);
	log_return_ptr(efi_device_path_instance_next(
		efi_current_ctx->mem[efi_bs_data].pool,
		instance,size
	));
}

static efiapi bool is_device_path_multi_instance_impl(
	const efi_device_path*dp
){
	log_call(
		call_arg(device_path,dp),
		NULL
	);
	log_return_bool(efi_device_path_is_multi_instance(dp));
}

static efiapi efi_device_path*create_device_node_impl(
	uint8_t type,
	uint8_t sub_type,
	uint16_t length
){
	log_call(
		call_arg(unsigned,type),
		call_arg(unsigned,sub_type),
		call_arg(unsigned,length),
		NULL
	);
	if(!efi_current_ctx)log_return_ptr(NULL);
	log_return_ptr(efi_device_path_create_node(
		efi_current_ctx->mem[efi_bs_data].pool,
		type,sub_type,length
	));
}

static efi_device_path_utilities_protocol device_path_utils={
	.get_device_path_size          = get_device_path_size_impl,
	.duplicate_device_path         = duplicate_device_path_impl,
	.append_device_path            = append_device_path_impl,
	.append_device_node            = append_device_node_impl,
	.append_device_path_instance   = append_device_path_instance_impl,
	.get_next_device_path_instance = get_next_device_path_instance_impl,
	.is_device_path_multi_instance = is_device_path_multi_instance_impl,
	.create_device_node            = create_device_node_impl,
};

efi_status efi_new_device_path_utils(efi_run_context*ctx,efi_handle*handle){
	if(!ctx)return efi_not_ready;
	return ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiDevicePathUtilitiesProtocolGuid,&device_path_utils,
		NULL
	);
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)
		return efi_invalid_parameter;
	return efi_new_device_path_utils(ctx,&ins->handle);
}

static const efi_service efi_device_path_utils_service={
	.guid    = &gEfiDevicePathUtilitiesProtocolGuid,
	.name    = "DevicePathUtilitiesProtocol",
	.depends = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start   = svc_start,
};

DECLARE_EFI_SERVICE(efi_device_path_utils_service)
