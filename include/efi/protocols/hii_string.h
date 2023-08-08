//
// Created by bigfoot on 2023/5/11.
//

#ifndef EFI_WRAPPER_HII_STRING_H
#define EFI_WRAPPER_HII_STRING_H
#include"hii_font.h"
#include"../efi_hii.h"
#define EFI_HII_STRING_PROTOCOL_GUID {0xfd96974,0x23aa,0x4cdc,{0xb9,0xcb,0x98,0xd1,0x77,0x50,0x32,0x2a}}
typedef struct efi_hii_string_protocol efi_hii_string_protocol;
typedef efi_status(efiapi*efi_hii_new_string)(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	efi_string_id                 *id,
	const char8                   *lang,
	const char16                  *lang_name,
	const efi_string              string,
	const efi_font_info           *font_info
);
typedef efi_status(efiapi*efi_hii_get_string)(
	const efi_hii_string_protocol *proto,
	const char8                   *lang,
	efi_hii_handle                pkg_list,
	efi_string_id                 id,
	efi_string                    string,
	uintn_t                       *size,
	efi_font_info                 **font_info
);
typedef efi_status(efiapi*efi_hii_set_string)(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	efi_string_id                 id,
	const char8                   *language,
	efi_string                    string,
	const efi_font_info           *font_info
);
typedef efi_status(efiapi*efi_hii_get_languages)(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	char8                         *langs,
	uintn_t                       *lang_size
);
typedef efi_status(efiapi*efi_hii_get_2nd_languages)(
	const efi_hii_string_protocol *proto,
	efi_hii_handle                pkg_list,
	const char8                   *pri_lang,
	char8                         *sec_langs,
	uintn_t                       *sec_lang_size
);
struct efi_hii_string_protocol{
	efi_hii_new_string        new_string;
	efi_hii_get_string        get_string;
	efi_hii_set_string        set_string;
	efi_hii_get_languages     get_languages;
	efi_hii_get_2nd_languages get_secondary_languages;
};
extern const efi_guid gEfiHiiStringProtocolGuid;
#endif //EFI_WRAPPER_HII_STRING_H
