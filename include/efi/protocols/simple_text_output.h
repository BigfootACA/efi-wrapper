//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_SIMPLE_TEXT_OUTPUT_H
#define EFI_WRAPPER_SIMPLE_TEXT_OUTPUT_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID {0x387477c2,0x69c7,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_WRAPPER_POSIX_FD_SIMPLE_TEXT_OUT_PROTOCOL_GUID {0x44cd6c5d,0x4c28,0xe056,{0x32,0x89,0x52,0xcd,0xea,0xf4,0x37,0xa5}}
#define BOXDRAW_HORIZONTAL                  0x2500
#define BOXDRAW_VERTICAL                    0x2502
#define BOXDRAW_DOWN_RIGHT                  0x250c
#define BOXDRAW_DOWN_LEFT                   0x2510
#define BOXDRAW_UP_RIGHT                    0x2514
#define BOXDRAW_UP_LEFT                     0x2518
#define BOXDRAW_VERTICAL_RIGHT              0x251c
#define BOXDRAW_VERTICAL_LEFT               0x2524
#define BOXDRAW_DOWN_HORIZONTAL             0x252c
#define BOXDRAW_UP_HORIZONTAL               0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL         0x253c
#define BOXDRAW_DOUBLE_HORIZONTAL           0x2550
#define BOXDRAW_DOUBLE_VERTICAL             0x2551
#define BOXDRAW_DOWN_RIGHT_DOUBLE           0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT           0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT           0x2554
#define BOXDRAW_DOWN_LEFT_DOUBLE            0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT            0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT            0x2557
#define BOXDRAW_UP_RIGHT_DOUBLE             0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT             0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT             0x255a
#define BOXDRAW_UP_LEFT_DOUBLE              0x255b
#define BOXDRAW_UP_DOUBLE_LEFT              0x255c
#define BOXDRAW_DOUBLE_UP_LEFT              0x255d
#define BOXDRAW_VERTICAL_RIGHT_DOUBLE       0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT       0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT       0x2560
#define BOXDRAW_VERTICAL_LEFT_DOUBLE        0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT        0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT        0x2563
#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE      0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL      0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL      0x2566
#define BOXDRAW_UP_HORIZONTAL_DOUBLE        0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL        0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL        0x2569
#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE  0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL  0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL  0x256c
#define BLOCKELEMENT_FULL_BLOCK             0x2588
#define BLOCKELEMENT_LIGHT_SHADE            0x2591
#define GEOMETRICSHAPE_UP_TRIANGLE          0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE       0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE        0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE        0x25c4
#define ARROW_LEFT                          0x2190
#define ARROW_UP                            0x2191
#define ARROW_RIGHT                         0x2192
#define ARROW_DOWN                          0x2193
#define EFI_BLACK                           0x00
#define EFI_BLUE                            0x01
#define EFI_GREEN                           0x02
#define EFI_CYAN                            (EFI_BLUE|EFI_GREEN)
#define EFI_RED                             0x04
#define EFI_MAGENTA                         (EFI_BLUE|EFI_RED)
#define EFI_BROWN                           (EFI_GREEN|EFI_RED)
#define EFI_LIGHTGRAY                       (EFI_BLUE|EFI_GREEN|EFI_RED)
#define EFI_BRIGHT                          0x08
#define EFI_DARKGRAY                        (EFI_BLACK|EFI_BRIGHT)
#define EFI_LIGHTBLUE                       (EFI_BLUE|EFI_BRIGHT)
#define EFI_LIGHTGREEN                      (EFI_GREEN|EFI_BRIGHT)
#define EFI_LIGHTCYAN                       (EFI_CYAN|EFI_BRIGHT)
#define EFI_LIGHTRED                        (EFI_RED|EFI_BRIGHT)
#define EFI_LIGHTMAGENTA                    (EFI_MAGENTA|EFI_BRIGHT)
#define EFI_YELLOW                          (EFI_BROWN|EFI_BRIGHT)
#define EFI_WHITE                           (EFI_BLUE|EFI_GREEN|EFI_RED|EFI_BRIGHT)
#define EFI_TEXT_ATTR(fg,bg)                ((fg)|((bg)<<4))
#define EFI_BACKGROUND_BLACK                0x00
#define EFI_BACKGROUND_BLUE                 0x10
#define EFI_BACKGROUND_GREEN                0x20
#define EFI_BACKGROUND_CYAN                 (EFI_BACKGROUND_BLUE|EFI_BACKGROUND_GREEN)
#define EFI_BACKGROUND_RED                  0x40
#define EFI_BACKGROUND_MAGENTA              (EFI_BACKGROUND_BLUE|EFI_BACKGROUND_RED)
#define EFI_BACKGROUND_BROWN                (EFI_BACKGROUND_GREEN|EFI_BACKGROUND_RED)
#define EFI_BACKGROUND_LIGHTGRAY            (EFI_BACKGROUND_BLUE|EFI_BACKGROUND_GREEN|EFI_BACKGROUND_RED)
#define EFI_WIDE_ATTRIBUTE                  0x80

typedef struct efi_simple_text_output_protocol efi_simple_text_output_protocol;
typedef efiapi efi_status(*efi_text_reset)(
	efi_simple_text_output_protocol*proto,
	bool ext_verify
);
typedef efiapi efi_status(*efi_text_string)(
	efi_simple_text_output_protocol*proto,
	char16*string
);
typedef efiapi efi_status(*efi_text_test_string)(
	efi_simple_text_output_protocol*proto,
	char16*string
);
typedef efiapi efi_status(*efi_text_query_mode)(
	efi_simple_text_output_protocol*proto,
	uintn_t mode,
	uintn_t*cols,
	uintn_t*rows
);
typedef efiapi efi_status(*efi_text_set_mode)(
	efi_simple_text_output_protocol*proto,
	uintn_t mode
);
typedef efiapi efi_status(*efi_text_set_attribute)(
	efi_simple_text_output_protocol*proto,
	uintn_t attribute
);
typedef efiapi efi_status(*efi_text_clear_screen)(
	efi_simple_text_output_protocol*proto
);
typedef efiapi efi_status(*efi_text_set_cursor_position)(
	efi_simple_text_output_protocol*proto,
	uintn_t col,
	uintn_t row
);
typedef efiapi efi_status(*efi_text_enable_cursor)(
	efi_simple_text_output_protocol*proto,
	bool visible
);
typedef struct efi_simple_text_output_mode{
	int32_t max_mode;
	int32_t mode;
	union{
		int32_t num:32;
		struct{
			int16_t fg:4;
			int16_t bg:4;
			int32_t _r:24;
		}data;
	}attribute;
	int32_t cursor_col;
	int32_t cursor_row;
	bool cursor_visible;
}efi_simple_text_output_mode;
struct efi_simple_text_output_protocol{
	efi_text_reset                reset;
	efi_text_string               output_string;
	efi_text_test_string          test_string;
	efi_text_query_mode           query_mode;
	efi_text_set_mode             set_mode;
	efi_text_set_attribute        set_attribute;
	efi_text_clear_screen         clear_screen;
	efi_text_set_cursor_position  set_cursor_position;
	efi_text_enable_cursor        enable_cursor;
	efi_simple_text_output_mode   *mode;
};
extern const efi_guid gEfiSimpleTextOutProtocolGuid;
extern const efi_guid gEfiWrapperPosixFDSimpleTextOutProtocolGuid;
#endif //EFI_WRAPPER_SIMPLE_TEXT_OUTPUT_H
