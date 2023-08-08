//
// Created by bigfoot on 2023/5/12.
//

#ifndef EFI_WRAPPER_UNICODE_COLLATION_H
#define EFI_WRAPPER_UNICODE_COLLATION_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_UNICODE_COLLATION_PROTOCOL_GUID  {0x1d85cd7f,0xf43d,0x11d2,{0x9a,0x0c,0x00,0x90,0x27,0x3f,0xc1,0x4d}}
#define EFI_UNICODE_COLLATION_PROTOCOL2_GUID {0xa4c751fc,0x23ae,0x4c3e,{0x92,0xe9,0x49,0x64,0xcf,0x63,0xf3,0x49}}
typedef struct efi_unicode_collation_protocol efi_unicode_collation_protocol;
typedef efi_unicode_collation_protocol unicode_collation_interface;
#define EFI_UNICODE_BYTE_ORDER_MARK (CHAR16)(0xfeff)
typedef intn_t(efiapi*efi_unicode_collation_stricoll)(
	efi_unicode_collation_protocol *proto,
	char16                         *str1,
	char16                         *str2
);
typedef bool(efiapi*efi_unicode_collation_metaimatch)(
	efi_unicode_collation_protocol *proto,
	char16                         *string,
	char16                         *pattern
);
typedef void(efiapi*efi_unicode_collation_strlwr)(
	efi_unicode_collation_protocol *proto,
	char16                         *str
);
typedef void(efiapi*efi_unicode_collation_strupr)(
	efi_unicode_collation_protocol *proto,
	char16                         *str
);
typedef void(efiapi*efi_unicode_collation_fattostr)(
	efi_unicode_collation_protocol *proto,
	uintn_t                        size,
	char8                          *fat,
	char16                         *string
);
typedef bool(efiapi*efi_unicode_collation_strtofat)(
	efi_unicode_collation_protocol *proto,
	char16                         *string,
	uintn_t                        size,
	char8                          *fat
);
struct efi_unicode_collation_protocol{
	efi_unicode_collation_stricoll   stricoll;
	efi_unicode_collation_metaimatch metaimatch;
	efi_unicode_collation_strlwr     strtolower;
	efi_unicode_collation_strupr     strtoupper;
	efi_unicode_collation_fattostr   fattostr;
	efi_unicode_collation_strtofat   strtofat;
	char*supported_languages;
};
extern const efi_guid gEfiUnicodeCollationProtocolGuid;
extern const efi_guid gEfiUnicodeCollation2ProtocolGuid;
#endif //EFI_WRAPPER_UNICODE_COLLATION_H
