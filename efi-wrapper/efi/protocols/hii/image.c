//
// Created by bigfoot on 2023/5/11.
//

#include"efi/efi_context.h"
#include"efi/protocols/hii_image.h"

static efiapi efi_status efi_hii_new_image_impl(
	const efi_hii_image_protocol *proto,
	efi_hii_handle               pkg_list,
	efi_image_id                 *id,
	const efi_image_input        *image
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(ptr,image),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_image_impl(
	const efi_hii_image_protocol *proto,
	efi_hii_handle               pkg_list,
	efi_image_id                 id,
	efi_image_input              *image
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(ptr,image),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_set_image_impl(
	const efi_hii_image_protocol *proto,
	efi_hii_handle               pkg_list,
	efi_image_id                 id,
	const efi_image_input        *image
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(ptr,image),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_draw_image_impl(
	const efi_hii_image_protocol *proto,
	efi_hii_draw_flags           flags,
	const efi_image_input        *image,
	efi_image_output             **blt,
	uintn_t                      blt_x,
	uintn_t                      blt_y
){
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,flags),
		call_arg(ptr,image),
		call_arg(ptr,blt),
		call_arg(unsigned,blt_x),
		call_arg(unsigned,blt_y),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_draw_image_id_impl(
	const efi_hii_image_protocol *proto,
	efi_hii_draw_flags           flags,
	efi_hii_handle               pkg_list,
	efi_image_id                 id,
	efi_image_output             **blt,
	uintn_t                      blt_x,
	uintn_t                      blt_y
){
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,flags),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(ptr,blt),
		call_arg(unsigned,blt_x),
		call_arg(unsigned,blt_y),
		NULL
	);
	log_return(efi_unsupported);
}

static efi_hii_image_protocol image_protocol={
	.new_image     = efi_hii_new_image_impl,
	.get_image     = efi_hii_get_image_impl,
	.set_image     = efi_hii_set_image_impl,
	.draw_image    = efi_hii_draw_image_impl,
	.draw_image_id = efi_hii_draw_image_id_impl,
};

efi_status efi_new_hii_image(efi_handle*handle){
	if(!efi_current_ctx)return efi_not_ready;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiHiiImageProtocolGuid,&image_protocol,
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
	return efi_new_hii_image(&ins->handle);
}
static const efi_service efi_hii_image_service={
	.guid    = &gEfiHiiImageProtocolGuid,
	.name    = "HiiImageProtocol",
	.depends = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start   = svc_start,
	.disabled=true,
};

DECLARE_EFI_SERVICE(efi_hii_image_service)
