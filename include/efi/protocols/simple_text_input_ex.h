//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_SIMPLE_TEXT_INPUT_EX_H
#define EFI_WRAPPER_SIMPLE_TEXT_INPUT_EX_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"simple_text_input.h"
#define EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID {0xdd9e7534,0x7762,0x4698,{0x8c,0x14,0xf5,0x85,0x17,0xa6,0x25,0xaa}}
typedef struct efi_simple_text_input_ex_protocol efi_simple_text_input_ex_protocol;
typedef uint8_t efi_key_toggle_state;
typedef struct efi_key_state{
	uint32_t             shift;
	efi_key_toggle_state toggle;
}efi_key_state;
typedef struct efi_key_data{
	efi_input_key key;
	efi_key_state state;
}efi_key_data;
#define EFI_SHIFT_STATE_VALID     0x80000000
#define EFI_RIGHT_SHIFT_PRESSED   0x00000001
#define EFI_LEFT_SHIFT_PRESSED    0x00000002
#define EFI_RIGHT_CONTROL_PRESSED 0x00000004
#define EFI_LEFT_CONTROL_PRESSED  0x00000008
#define EFI_RIGHT_ALT_PRESSED     0x00000010
#define EFI_LEFT_ALT_PRESSED      0x00000020
#define EFI_RIGHT_LOGO_PRESSED    0x00000040
#define EFI_LEFT_LOGO_PRESSED     0x00000080
#define EFI_MENU_KEY_PRESSED      0x00000100
#define EFI_SYS_REQ_PRESSED       0x00000200
#define EFI_TOGGLE_STATE_VALID    0x80
#define EFI_KEY_STATE_EXPOSED     0x40
#define EFI_SCROLL_LOCK_ACTIVE    0x01
#define EFI_NUM_LOCK_ACTIVE       0x02
#define EFI_CAPS_LOCK_ACTIVE      0x04
#define SCAN_F11                  0x0015
#define SCAN_F12                  0x0016
#define SCAN_PAUSE                0x0048
#define SCAN_F13                  0x0068
#define SCAN_F14                  0x0069
#define SCAN_F15                  0x006A
#define SCAN_F16                  0x006B
#define SCAN_F17                  0x006C
#define SCAN_F18                  0x006D
#define SCAN_F19                  0x006E
#define SCAN_F20                  0x006F
#define SCAN_F21                  0x0070
#define SCAN_F22                  0x0071
#define SCAN_F23                  0x0072
#define SCAN_F24                  0x0073
#define SCAN_MUTE                 0x007F
#define SCAN_VOLUME_UP            0x0080
#define SCAN_VOLUME_DOWN          0x0081
#define SCAN_BRIGHTNESS_UP        0x0100
#define SCAN_BRIGHTNESS_DOWN      0x0101
#define SCAN_SUSPEND              0x0102
#define SCAN_HIBERNATE            0x0103
#define SCAN_TOGGLE_DISPLAY       0x0104
#define SCAN_RECOVERY             0x0105
#define SCAN_EJECT                0x0106
typedef efi_status(efiapi*efi_input_reset_ex)(
	efi_simple_text_input_ex_protocol*proto,
	bool ext_verify
);
typedef efi_status(efiapi*efi_input_read_key_ex)(
	efi_simple_text_input_ex_protocol*proto,
	efi_key_data*data
);
typedef efi_status(efiapi*efi_set_state)(
	efi_simple_text_input_ex_protocol*proto,
	efi_key_toggle_state*state
);
typedef efi_status(efiapi*efi_key_notify_function)(
	efi_key_data*data
);
typedef efi_status(efiapi*efi_register_keystroke_notify)(
	efi_simple_text_input_ex_protocol*proto,
	efi_key_data*data,
	efi_key_notify_function func,
	void**handle
);
typedef efi_status(efiapi*efi_unregister_keystroke_notify)(
	efi_simple_text_input_ex_protocol*proto,
	void*handle
);
struct efi_simple_text_input_ex_protocol{
	efi_input_reset_ex              reset;
	efi_input_read_key_ex           read_key_stroke_ex;
	efi_event                       wait_for_key_ex;
	efi_set_state                   set_state;
	efi_register_keystroke_notify   register_key_notify;
	efi_unregister_keystroke_notify unregister_key_notify;
};
extern const efi_guid gEfiSimpleTextInputExProtocolGuid;
#endif //EFI_WRAPPER_SIMPLE_TEXT_INPUT_EX_H
