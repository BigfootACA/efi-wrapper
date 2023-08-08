//
// Created by bigfoot on 2023/5/11.
//

#ifndef EFI_WRAPPER_HII_FONT_H
#define EFI_WRAPPER_HII_FONT_H
#include"graphics_output.h"
#include"hii_image.h"
#include"../efi_hii.h"
#define EFI_HII_FONT_PROTOCOL_GUID {0xe9ca4775,0x8657,0x47fc,{0x97,0xe7,0x7e,0xd6,0x5a,0x08,0x43,0x24}}
typedef struct efi_hii_font_protocol efi_hii_font_protocol;
typedef void*efi_font_handle;
typedef uint32_t efi_hii_out_flags;
#define EFI_HII_OUT_FLAG_CLIP         0x00000001
#define EFI_HII_OUT_FLAG_WRAP         0x00000002
#define EFI_HII_OUT_FLAG_CLIP_CLEAN_Y 0x00000004
#define EFI_HII_OUT_FLAG_CLIP_CLEAN_X 0x00000008
#define EFI_HII_OUT_FLAG_TRANSPARENT  0x00000010
#define EFI_HII_IGNORE_IF_NO_GLYPH    0x00000020
#define EFI_HII_IGNORE_LINE_BREAK     0x00000040
#define EFI_HII_DIRECT_TO_SCREEN      0x00000080
typedef struct efi_hii_row_info{
	uintn_t start_index;
	uintn_t end_index;
	uintn_t line_height;
	uintn_t line_width;
	uintn_t baseline_offset;
}efi_hii_row_info;
typedef uint32_t efi_font_info_mask;
#define EFI_FONT_INFO_SYS_FONT        0x00000001
#define EFI_FONT_INFO_SYS_SIZE        0x00000002
#define EFI_FONT_INFO_SYS_STYLE       0x00000004
#define EFI_FONT_INFO_SYS_FORE_COLOR  0x00000010
#define EFI_FONT_INFO_SYS_BACK_COLOR  0x00000020
#define EFI_FONT_INFO_RESIZE          0x00001000
#define EFI_FONT_INFO_RESTYLE         0x00002000
#define EFI_FONT_INFO_ANY_FONT        0x00010000
#define EFI_FONT_INFO_ANY_SIZE        0x00020000
#define EFI_FONT_INFO_ANY_STYLE       0x00040000
typedef struct efi_font_info{
	efi_hii_font_style style;
	uint16_t           size;
	char16             name[];
}efi_font_info;
typedef struct efi_font_display_info{
	efi_graphics_output_blt_pixel foreground_color;
	efi_graphics_output_blt_pixel background_color;
	efi_font_info_mask            font_info_mask;
	efi_font_info                 font_info;
}efi_font_display_info;
typedef efi_status(efiapi*efi_hii_string_to_image)(
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
);
typedef efi_status(efiapi*efi_hii_string_id_to_image)(
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
);
typedef efi_status(efiapi*efi_hii_get_glyph)(
	const efi_hii_font_protocol *proto,
	const char16                ch,
	const efi_font_display_info *info,
	efi_image_output            **blt,
	uintn_t                     *baseline
);
typedef efi_status(efiapi*efi_hii_get_font_info)(
	const efi_hii_font_protocol *proto,
	efi_font_handle             *handle,
	const efi_font_display_info *info_in,
	efi_font_display_info       **info_out,
	const efi_string            string
);
struct efi_hii_font_protocol{
	efi_hii_string_to_image     string_to_image;
	efi_hii_string_id_to_image  string_id_to_image;
	efi_hii_get_glyph           get_glyph;
	efi_hii_get_font_info       get_font_info;
};
extern const efi_guid gEfiHiiFontProtocolGuid;
#endif //EFI_WRAPPER_HII_FONT_H
