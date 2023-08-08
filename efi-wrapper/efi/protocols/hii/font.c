//
// Created by bigfoot on 2023/5/11.
//

#include"efi/efi_context.h"
#include"efi/protocols/hii_font.h"

static efiapi efi_status efi_hii_string_to_image_impl(
	const efi_hii_font_protocol *proto,
	efi_hii_out_flags           flags,
	const efi_string            string,
	const efi_font_display_info *info,
	efi_image_output            **blt,
	uintn_t                     blt_x,
	uintn_t                     blt_y,
	efi_hii_row_info            **row,
	uintn_t                     *row_size,
	uintn_t                     *col
){
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,flags),
		call_arg(str16,string),
		call_arg(ptr,info),
		call_arg(ptr,blt),
		call_arg(unsigned,blt_x),
		call_arg(unsigned,blt_y),
		call_arg(ptr,row),
		call_arg(ptr,row_size),
		call_arg(ptr,col),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_string_id_to_image_impl(
	const efi_hii_font_protocol *proto,
	efi_hii_out_flags           flags,
	efi_hii_handle              pkg_list,
	efi_string_id               id,
	const char8                 *language,
	const efi_font_display_info *info,
	efi_image_output            **blt,
	uintn_t                     blt_x,
	uintn_t                     blt_y,
	efi_hii_row_info            **row,
	uintn_t                     *row_size,
	uintn_t                     *col
){
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,flags),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(str8,language),
		call_arg(ptr,info),
		call_arg(ptr,blt),
		call_arg(unsigned,blt_x),
		call_arg(unsigned,blt_y),
		call_arg(ptr,row),
		call_arg(ptr,row_size),
		call_arg(ptr,col),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_glyph_impl(
	const efi_hii_font_protocol *proto,
	const char16                ch,
	const efi_font_display_info *info,
	efi_image_output            **blt,
	uintn_t                     *baseline
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,ch),
		call_arg(ptr,info),
		call_arg(ptr,blt),
		call_arg(ptr,baseline),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_font_info_impl(
	const efi_hii_font_protocol *proto,
	efi_font_handle             *handle,
	const efi_font_display_info *info_in,
	efi_font_display_info       **info_out,
	const efi_string            string
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		call_arg(ptr,info_in),
		call_arg(ptr,info_out),
		call_arg(str16,string),
		NULL
	);
	log_return(efi_unsupported);
}

static efi_hii_font_protocol font_protocol={
	.string_to_image    = efi_hii_string_to_image_impl,
	.string_id_to_image = efi_hii_string_id_to_image_impl,
	.get_glyph          = efi_hii_get_glyph_impl,
	.get_font_info      = efi_hii_get_font_info_impl,
};

efi_status efi_new_hii_font(efi_handle*handle){
	if(!efi_current_ctx)return efi_not_ready;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiHiiFontProtocolGuid,&font_protocol,
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
	return efi_new_hii_font(&ins->handle);
}

static const efi_service efi_hii_font_service={
	.guid    = &gEfiHiiFontProtocolGuid,
	.name    = "HiiFontProtocol",
	.depends = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start   = svc_start,
	.disabled=true,
};

DECLARE_EFI_SERVICE(efi_hii_font_service)
