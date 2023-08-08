//
// Created by bigfoot on 2023/5/11.
//

#include"efi/efi_context.h"
#include"efi/protocols/hii_string.h"

static efiapi efi_status efi_hii_new_string_impl(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	efi_string_id                 *id,
	const char8                   *lang,
	const char16                  *lang_name,
	const efi_string              string,
	const efi_font_info           *font_info
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(str8,lang),
		call_arg(str16,lang_name),
		call_arg(str16,string),
		call_arg(ptr,font_info),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_string_impl(
	const efi_hii_string_protocol *proto,
	const char8                   *lang,
	efi_hii_handle                pkg_list,
	efi_string_id                 id,
	efi_string                    string,
	uintn_t                       *size,
	efi_font_info                 **font_info
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str8,lang),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(str16,string),
		call_arg(ptr,size),
		call_arg(ptr,font_info),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_set_string_impl(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	efi_string_id                 id,
	const char8                   *lang,
	efi_string                    string,
	const efi_font_info           *font_info
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(ptr,id),
		call_arg(str8,lang),
		call_arg(str16,string),
		call_arg(ptr,font_info),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_languages_impl(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	char8                         *langs,
	uintn_t                       *lang_size
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(str8,langs),
		call_arg(ptr,lang_size),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status efi_hii_get_2nd_languages_impl(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	const char8                   *pri_lang,
	char8                         *sec_langs,
	uintn_t                       *sec_lang_size
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,pkg_list),
		call_arg(str8,pri_lang),
		call_arg(ptr,sec_langs),
		call_arg(ptr,sec_lang_size),
		NULL
	);
	log_return(efi_unsupported);
}

static efi_hii_string_protocol string_protocol={
	.new_string              = efi_hii_new_string_impl,
	.get_string              = efi_hii_get_string_impl,
	.set_string              = efi_hii_set_string_impl,
	.get_languages           = efi_hii_get_languages_impl,
	.get_secondary_languages = efi_hii_get_2nd_languages_impl,
};

efi_status efi_new_hii_string(efi_handle*handle){
	if(!efi_current_ctx)return efi_not_ready;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiHiiStringProtocolGuid,&string_protocol,
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
	return efi_new_hii_string(&ins->handle);
}

static const efi_service efi_hii_string_service={
	.guid    = &gEfiHiiStringProtocolGuid,
	.name    = "HiiStringProtocol",
	.depends = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start   = svc_start,
	.disabled=true,
};

DECLARE_EFI_SERVICE(efi_hii_string_service)
