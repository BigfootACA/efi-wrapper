//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_SIMPLE_TEXT_INPUT_H
#define EFI_WRAPPER_SIMPLE_TEXT_INPUT_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID {0x387477c1,0x69c7,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_WRAPPER_POSIX_FD_SIMPLE_TEXT_IN_PROTOCOL_GUID {0xbb9b58c5,0xad30,0xabeb,{0x6d,0xf9,0x6a,0x4c,0x44,0x4f,0xcb,0x54}}
#define CHAR_NULL             0x0000
#define CHAR_BACKSPACE        0x0008
#define CHAR_TAB              0x0009
#define CHAR_LINEFEED         0x000A
#define CHAR_CARRIAGE_RETURN  0x000D
#define SCAN_NULL             0x0000
#define SCAN_UP               0x0001
#define SCAN_DOWN             0x0002
#define SCAN_RIGHT            0x0003
#define SCAN_LEFT             0x0004
#define SCAN_HOME             0x0005
#define SCAN_END              0x0006
#define SCAN_INSERT           0x0007
#define SCAN_DELETE           0x0008
#define SCAN_PAGE_UP          0x0009
#define SCAN_PAGE_DOWN        0x000A
#define SCAN_F1               0x000B
#define SCAN_F2               0x000C
#define SCAN_F3               0x000D
#define SCAN_F4               0x000E
#define SCAN_F5               0x000F
#define SCAN_F6               0x0010
#define SCAN_F7               0x0011
#define SCAN_F8               0x0012
#define SCAN_F9               0x0013
#define SCAN_F10              0x0014
#define SCAN_ESC              0x0017
typedef struct efi_simple_text_input_protocol efi_simple_text_input_protocol;
typedef struct efi_input_key{
	uint16_t scan_code;
	char16   unicode_char;
}efi_input_key;
typedef efiapi efi_status(*efi_input_reset)(
	efi_simple_text_input_protocol*proto,
	bool ext_verify
);
typedef efiapi efi_status(*efi_input_read_key)(
	efi_simple_text_input_protocol*proto,
	efi_input_key*key
);
struct efi_simple_text_input_protocol{
	efi_input_reset     reset;
	efi_input_read_key  read_key_stroke;
	efi_event           wait_for_key;
};
extern const efi_guid gEfiSimpleTextInProtocolGuid;
extern const efi_guid gEfiWrapperPosixFDSimpleTextInProtocolGuid;
#endif //EFI_WRAPPER_SIMPLE_TEXT_INPUT_H
