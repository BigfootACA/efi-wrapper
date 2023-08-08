//
// Created by bigfoot on 2023/5/12.
//

#include"efi/efi_string.h"
#include"efi/efi_context.h"
#include"efi/protocols/unicode_collation.h"

static efiapi intn_t efi_unicode_collation_stricoll_impl(
	efi_unicode_collation_protocol *proto,
	char16                         *str1,
	char16                         *str2
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,str1),
		call_arg(str16,str2),
		NULL
	);
	log_return_signed(intn_t,-1);
}

static efiapi bool efi_unicode_collation_metaimatch_impl(
	efi_unicode_collation_protocol *proto,
	char16                         *string,
	char16                         *pattern
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,string),
		call_arg(str16,pattern),
		NULL
	);
	log_return_bool(false);
}

static efiapi void efi_unicode_collation_strlwr_impl(
	efi_unicode_collation_protocol *proto,
	char16                         *str
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,str),
		NULL
	);
	log_return_void();
}

static efiapi void efi_unicode_collation_strupr_impl(
	efi_unicode_collation_protocol *proto,
	char16                         *str
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,str),
		NULL
	);
	log_return_void();
}

static efiapi void efi_unicode_collation_fattostr_impl(
	efi_unicode_collation_protocol *proto,
	uintn_t                        size,
	char8                          *fat,
	char16                         *string
){
	log_call(
		call_arg(ptr,proto),
		call_arg(unsigned,size),
		call_arg(str8,fat),
		call_arg(str16,string),
		NULL
	);
	log_return_void();
}

static efiapi bool efi_unicode_collation_strtofat_impl(
	efi_unicode_collation_protocol *proto,
	char16                         *string,
	uintn_t                        size,
	char8                          *fat
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,string),
		call_arg(unsigned,size),
		call_arg(str8,fat),
		NULL
	);
	log_return_bool(false);
}

static efi_unicode_collation_protocol unicode_protocol={
	.stricoll   = efi_unicode_collation_stricoll_impl,
	.metaimatch = efi_unicode_collation_metaimatch_impl,
	.strtolower = efi_unicode_collation_strlwr_impl,
	.strtoupper = efi_unicode_collation_strupr_impl,
	.fattostr   = efi_unicode_collation_fattostr_impl,
	.strtofat   = efi_unicode_collation_strtofat_impl,
	.supported_languages = "eng",
};

static efi_unicode_collation_protocol unicode2_protocol={
	.stricoll   = efi_unicode_collation_stricoll_impl,
	.metaimatch = efi_unicode_collation_metaimatch_impl,
	.strtolower = efi_unicode_collation_strlwr_impl,
	.strtoupper = efi_unicode_collation_strupr_impl,
	.fattostr   = efi_unicode_collation_fattostr_impl,
	.strtofat   = efi_unicode_collation_strtofat_impl,
	.supported_languages = "en",
};

efi_status efi_new_unicode_collation(efi_handle*handle){
	if(!efi_current_ctx)return efi_not_ready;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiUnicodeCollationProtocolGuid,&unicode2_protocol,
		&gEfiUnicodeCollation2ProtocolGuid,&unicode_protocol,
		NULL
	);
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)return efi_invalid_parameter;
	return efi_new_unicode_collation(&ins->handle);
}

static const efi_service efi_unicode_collation_service={
	.guid     = &gEfiUnicodeCollationProtocolGuid,
	.name     = "UnicodeCollationProtocol",
	.provides = {
		&gEfiUnicodeCollationProtocolGuid,
		&gEfiUnicodeCollation2ProtocolGuid,
		NULL
	},
	.depends  = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start    = svc_start,
	.disabled = true,
};

DECLARE_EFI_SERVICE(efi_unicode_collation_service)
