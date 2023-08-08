//
// Created by bigfoot on 2023/5/11.
//
//
#ifndef EFI_WRAPPER_EFI_HII_H
#define EFI_WRAPPER_EFI_HII_H
#include"efi_types.h"
#include"efi_status.h"
typedef void*efi_hii_handle;
typedef char16*efi_string;
typedef uint16_t efi_image_id;
typedef uint16_t efi_question_id;
typedef uint16_t efi_string_id;
typedef uint16_t efi_form_id;
typedef uint16_t efi_varstore_id;
typedef uint16_t efi_animation_id;
typedef uint16_t efi_default_id;
typedef uint32_t efi_hii_font_style;
#define EFI_HII_PACKAGE_TYPE_ALL             0x00
#define EFI_HII_PACKAGE_TYPE_GUID            0x01
#define EFI_HII_PACKAGE_FORMS                0x02
#define EFI_HII_PACKAGE_STRINGS              0x04
#define EFI_HII_PACKAGE_FONTS                0x05
#define EFI_HII_PACKAGE_IMAGES               0x06
#define EFI_HII_PACKAGE_SIMPLE_FONTS         0x07
#define EFI_HII_PACKAGE_DEVICE_PATH          0x08
#define EFI_HII_PACKAGE_KEYBOARD_LAYOUT      0x09
#define EFI_HII_PACKAGE_ANIMATIONS           0x0A
#define EFI_HII_PACKAGE_END                  0xDF
#define EFI_HII_PACKAGE_TYPE_SYSTEM_BEGIN    0xE0
#define EFI_HII_PACKAGE_TYPE_SYSTEM_END      0xFF
#define EFI_GLYPH_NON_SPACING                0x01
#define EFI_GLYPH_WIDE                       0x02
#define EFI_GLYPH_HEIGHT                     19
#define EFI_GLYPH_WIDTH                      8
#define EFI_HII_FONT_STYLE_NORMAL            0x00000000
#define EFI_HII_FONT_STYLE_BOLD              0x00000001
#define EFI_HII_FONT_STYLE_ITALIC            0x00000002
#define EFI_HII_FONT_STYLE_EMBOSS            0x00010000
#define EFI_HII_FONT_STYLE_OUTLINE           0x00020000
#define EFI_HII_FONT_STYLE_SHADOW            0x00040000
#define EFI_HII_FONT_STYLE_UNDERLINE         0x00080000
#define EFI_HII_FONT_STYLE_DBL_UNDER         0x00100000
#define EFI_HII_GIBT_END                     0x00
#define EFI_HII_GIBT_GLYPH                   0x10
#define EFI_HII_GIBT_GLYPHS                  0x11
#define EFI_HII_GIBT_GLYPH_DEFAULT           0x12
#define EFI_HII_GIBT_GLYPHS_DEFAULT          0x13
#define EFI_HII_GIBT_GLYPH_VARIABILITY       0x14
#define EFI_HII_GIBT_DUPLICATE               0x20
#define EFI_HII_GIBT_SKIP2                   0x21
#define EFI_HII_GIBT_SKIP1                   0x22
#define EFI_HII_GIBT_DEFAULTS                0x23
#define EFI_HII_GIBT_EXT1                    0x30
#define EFI_HII_GIBT_EXT2                    0x31
#define EFI_HII_GIBT_EXT4                    0x32
#define EFI_HII_SIBT_END                     0x00
#define EFI_HII_SIBT_STRING_SCSU             0x10
#define EFI_HII_SIBT_STRING_SCSU_FONT        0x11
#define EFI_HII_SIBT_STRINGS_SCSU            0x12
#define EFI_HII_SIBT_STRINGS_SCSU_FONT       0x13
#define EFI_HII_SIBT_STRING_UCS2             0x14
#define EFI_HII_SIBT_STRING_UCS2_FONT        0x15
#define EFI_HII_SIBT_STRINGS_UCS2            0x16
#define EFI_HII_SIBT_STRINGS_UCS2_FONT       0x17
#define EFI_HII_SIBT_DUPLICATE               0x20
#define EFI_HII_SIBT_SKIP2                   0x21
#define EFI_HII_SIBT_SKIP1                   0x22
#define EFI_HII_SIBT_EXT1                    0x30
#define EFI_HII_SIBT_EXT2                    0x31
#define EFI_HII_SIBT_EXT4                    0x32
#define EFI_HII_SIBT_FONT                    0x40
#define EFI_HII_IIBT_END                     0x00
#define EFI_HII_IIBT_IMAGE_1BIT              0x10
#define EFI_HII_IIBT_IMAGE_1BIT_TRANS        0x11
#define EFI_HII_IIBT_IMAGE_4BIT              0x12
#define EFI_HII_IIBT_IMAGE_4BIT_TRANS        0x13
#define EFI_HII_IIBT_IMAGE_8BIT              0x14
#define EFI_HII_IIBT_IMAGE_8BIT_TRANS        0x15
#define EFI_HII_IIBT_IMAGE_24BIT             0x16
#define EFI_HII_IIBT_IMAGE_24BIT_TRANS       0x17
#define EFI_HII_IIBT_IMAGE_JPEG              0x18
#define EFI_HII_IIBT_IMAGE_PNG               0x19
#define EFI_HII_IIBT_DUPLICATE               0x20
#define EFI_HII_IIBT_SKIP2                   0x21
#define EFI_HII_IIBT_SKIP1                   0x22
#define EFI_HII_IIBT_EXT1                    0x30
#define EFI_HII_IIBT_EXT2                    0x31
#define EFI_HII_IIBT_EXT4                    0x32
#define EFI_AFFECTED_BY_STANDARD_SHIFT       0x0001
#define EFI_AFFECTED_BY_CAPS_LOCK            0x0002
#define EFI_AFFECTED_BY_NUM_LOCK             0x0004
#define EFI_NULL_MODIFIER                    0x0000
#define EFI_LEFT_CONTROL_MODIFIER            0x0001
#define EFI_RIGHT_CONTROL_MODIFIER           0x0002
#define EFI_LEFT_ALT_MODIFIER                0x0003
#define EFI_RIGHT_ALT_MODIFIER               0x0004
#define EFI_ALT_GR_MODIFIER                  0x0005
#define EFI_INSERT_MODIFIER                  0x0006
#define EFI_DELETE_MODIFIER                  0x0007
#define EFI_PAGE_DOWN_MODIFIER               0x0008
#define EFI_PAGE_UP_MODIFIER                 0x0009
#define EFI_HOME_MODIFIER                    0x000A
#define EFI_END_MODIFIER                     0x000B
#define EFI_LEFT_SHIFT_MODIFIER              0x000C
#define EFI_RIGHT_SHIFT_MODIFIER             0x000D
#define EFI_CAPS_LOCK_MODIFIER               0x000E
#define EFI_NUM_LOCK_MODIFIER                0x000F
#define EFI_LEFT_ARROW_MODIFIER              0x0010
#define EFI_RIGHT_ARROW_MODIFIER             0x0011
#define EFI_DOWN_ARROW_MODIFIER              0x0012
#define EFI_UP_ARROW_MODIFIER                0x0013
#define EFI_NS_KEY_MODIFIER                  0x0014
#define EFI_NS_KEY_DEPENDENCY_MODIFIER       0x0015
#define EFI_FUNCTION_KEY_ONE_MODIFIER        0x0016
#define EFI_FUNCTION_KEY_TWO_MODIFIER        0x0017
#define EFI_FUNCTION_KEY_THREE_MODIFIER      0x0018
#define EFI_FUNCTION_KEY_FOUR_MODIFIER       0x0019
#define EFI_FUNCTION_KEY_FIVE_MODIFIER       0x001A
#define EFI_FUNCTION_KEY_SIX_MODIFIER        0x001B
#define EFI_FUNCTION_KEY_SEVEN_MODIFIER      0x001C
#define EFI_FUNCTION_KEY_EIGHT_MODIFIER      0x001D
#define EFI_FUNCTION_KEY_NINE_MODIFIER       0x001E
#define EFI_FUNCTION_KEY_TEN_MODIFIER        0x001F
#define EFI_FUNCTION_KEY_ELEVEN_MODIFIER     0x0020
#define EFI_FUNCTION_KEY_TWELVE_MODIFIER     0x0021
#define EFI_PRINT_MODIFIER                   0x0022
#define EFI_SYS_REQUEST_MODIFIER             0x0023
#define EFI_SCROLL_LOCK_MODIFIER             0x0024
#define EFI_PAUSE_MODIFIER                   0x0025
#define EFI_BREAK_MODIFIER                   0x0026
#define EFI_LEFT_LOGO_MODIFIER               0x0027
#define EFI_RIGHT_LOGO_MODIFIER              0x0028
#define EFI_MENU_MODIFIER                    0x0029
#define UEFI_CONFIG_LANG   "x-UEFI"
#define UEFI_CONFIG_LANG_2 "x-i-UEFI"
#pragma pack(1)
typedef struct efi_hii_package_list_header{
	efi_guid guid;
	uint32_t length;
}cdecl_attr_packed efi_hii_package_list_header;
static_assert(sizeof(efi_hii_package_list_header)==20,"efi_hii_package_list_header size mismatch");
typedef struct efi_hii_package_header{
	uint32_t length:24;
	uint32_t type:8;
}cdecl_attr_packed efi_hii_package_header;
static_assert(sizeof(efi_hii_package_header)==4,"efi_hii_package_header size mismatch");
typedef struct efi_narrow_glyph{
	char16 weight;
	uint8_t attrs;
	uint8_t glyph_col1[EFI_GLYPH_HEIGHT];
}cdecl_attr_packed efi_narrow_glyph;
static_assert(sizeof(efi_narrow_glyph)==22,"efi_narrow_glyph size mismatch");
typedef struct efi_wide_glyph{
	char16 weight;
	uint8_t attrs;
	uint8_t glyph_col1[EFI_GLYPH_HEIGHT];
	uint8_t glyph_col2[EFI_GLYPH_HEIGHT];
	uint8_t pad[3];
}cdecl_attr_packed efi_wide_glyph;
static_assert(sizeof(efi_wide_glyph)==44,"efi_wide_glyph size mismatch");
typedef struct efi_hii_simple_font_package_hdr{
	efi_hii_package_header header;
	uint16_t               narrow_glyphs_cnt;
	uint16_t               wide_glyphs_cnt;
}cdecl_attr_packed efi_hii_simple_font_package_hdr;
static_assert(sizeof(efi_hii_simple_font_package_hdr)==8,"efi_hii_simple_font_package_hdr size mismatch");
typedef struct efi_hii_glyph_info{
	uint16_t width;
	uint16_t height;
	int16_t  offset_x;
	int16_t  offset_y;
	int16_t  advance_x;
}cdecl_attr_packed efi_hii_glyph_info;
static_assert(sizeof(efi_hii_glyph_info)==10,"efi_hii_glyph_info size mismatch");
typedef struct efi_hii_font_package_hdr{
	efi_hii_package_header header;
	uint32_t               hdr_size;
	uint32_t               glyph_block_offset;
	efi_hii_glyph_info     cell;
	efi_hii_font_style     font_style;
	char16                 font_family[0];
}cdecl_attr_packed efi_hii_font_package_hdr;
static_assert(sizeof(efi_hii_font_package_hdr)==26,"efi_hii_font_package_hdr size mismatch");
typedef struct efi_hii_glyph_block{
	uint8_t type;
}cdecl_attr_packed efi_hii_glyph_block;
static_assert(sizeof(efi_hii_glyph_block)==1,"efi_hii_glyph_block size mismatch");
typedef struct efi_hii_gibt_defaults_block{
	efi_hii_glyph_block header;
	efi_hii_glyph_info  cell;
}cdecl_attr_packed efi_hii_gibt_defaults_block;
static_assert(sizeof(efi_hii_gibt_defaults_block)==11,"efi_hii_gibt_defaults_block size mismatch");
typedef struct efi_hii_gibt_duplicate_block{
	efi_hii_glyph_block header;
	char16              value;
}cdecl_attr_packed efi_hii_gibt_duplicate_block;
static_assert(sizeof(efi_hii_gibt_duplicate_block)==3,"efi_hii_gibt_duplicate_block size mismatch");
typedef struct efi_glyph_gibt_end_block{
	efi_hii_glyph_block header;
}cdecl_attr_packed efi_glyph_gibt_end_block;
static_assert(sizeof(efi_glyph_gibt_end_block)==1,"efi_glyph_gibt_end_block size mismatch");
typedef struct efi_hii_gibt_ext1_block{
	efi_hii_glyph_block header;
	uint8_t             type;
	uint8_t             length;
}cdecl_attr_packed efi_hii_gibt_ext1_block;
static_assert(sizeof(efi_hii_gibt_ext1_block)==3,"efi_hii_gibt_ext1_block size mismatch");
typedef struct efi_hii_gibt_ext2_block{
	efi_hii_glyph_block header;
	uint8_t             type;
	uint16_t            length;
}cdecl_attr_packed efi_hii_gibt_ext2_block;
static_assert(sizeof(efi_hii_gibt_ext2_block)==4,"efi_hii_gibt_ext2_block size mismatch");
typedef struct efi_hii_gibt_ext4_block{
	efi_hii_glyph_block header;
	uint8_t             type;
	uint32_t            length;
}cdecl_attr_packed efi_hii_gibt_ext4_block;
static_assert(sizeof(efi_hii_gibt_ext4_block)==6,"efi_hii_gibt_ext4_block size mismatch");
typedef struct efi_hii_gibt_glyph_block{
	efi_hii_glyph_block header;
	efi_hii_glyph_info  cell;
	uint8_t             bitmap[0];
}cdecl_attr_packed efi_hii_gibt_glyph_block;
static_assert(sizeof(efi_hii_gibt_glyph_block)==11,"efi_hii_gibt_glyph_block size mismatch");
typedef struct efi_hii_gibt_glyphs_block{
	efi_hii_glyph_block header;
	efi_hii_glyph_info  cell;
	uint16_t            count;
	uint8_t             bitmap[0];
}cdecl_attr_packed efi_hii_gibt_glyphs_block;
static_assert(sizeof(efi_hii_gibt_glyphs_block)==13,"efi_hii_gibt_glyphs_block size mismatch");
typedef struct efi_hii_gibt_glyph_default_block{
	efi_hii_glyph_block header;
	uint8_t             bitmap[0];
}cdecl_attr_packed efi_hii_gibt_glyph_default_block;
static_assert(sizeof(efi_hii_gibt_glyph_default_block)==1,"efi_hii_gibt_glyph_default_block size mismatch");
typedef struct efi_hii_gibt_glyphs_default_block{
	efi_hii_glyph_block header;
	uint16_t            count;
	uint8_t             bitmap[0];
}cdecl_attr_packed efi_hii_gibt_glyphs_default_block;
static_assert(sizeof(efi_hii_gibt_glyphs_default_block)==3,"efi_hii_gibt_glyphs_default_block size mismatch");
typedef struct efi_hii_gibt_variability_block{
	efi_hii_glyph_block header;
	efi_hii_glyph_info  cell;
	uint8_t             glyph_pack_in_bits;
	uint8_t             bitmap[0];
}cdecl_attr_packed efi_hii_gibt_variability_block;
static_assert(sizeof(efi_hii_gibt_variability_block)==12,"efi_hii_gibt_variability_block size mismatch");
typedef struct efi_hii_gibt_skip1_block{
	efi_hii_glyph_block header;
	uint8_t             count;
}cdecl_attr_packed efi_hii_gibt_skip1_block;
static_assert(sizeof(efi_hii_gibt_skip1_block)==2,"efi_hii_gibt_skip1_block size mismatch");
typedef struct efi_hii_gibt_skip2_block{
	efi_hii_glyph_block header;
	uint16_t            count;
}cdecl_attr_packed efi_hii_gibt_skip2_block;
static_assert(sizeof(efi_hii_gibt_skip2_block)==3,"efi_hii_gibt_skip2_block size mismatch");
typedef struct efi_hii_device_path_package_hdr{
	efi_hii_package_header header;
}cdecl_attr_packed efi_hii_device_path_package_hdr;
static_assert(sizeof(efi_hii_device_path_package_hdr)==4,"efi_hii_device_path_package_hdr size mismatch");
typedef struct efi_hii_guid_package_hdr{
	efi_hii_package_header header;
	efi_guid               guid;
}cdecl_attr_packed efi_hii_guid_package_hdr;
static_assert(sizeof(efi_hii_guid_package_hdr)==20,"efi_hii_guid_package_hdr size mismatch");
typedef struct efi_hii_string_package_hdr{
	efi_hii_package_header  header;
	uint32_t                hdr_size;
	uint32_t                string_info_offset;
	char16                  language_window[16];
	efi_string_id           language_name;
	char8                   language[0];
}cdecl_attr_packed efi_hii_string_package_hdr;
static_assert(sizeof(efi_hii_string_package_hdr)==46,"efi_hii_string_package_hdr size mismatch");
typedef struct efi_hii_string_block{
	uint8_t type;
}cdecl_attr_packed efi_hii_string_block;
static_assert(sizeof(efi_hii_string_block)==1,"efi_hii_string_block size mismatch");
typedef struct efi_hii_sibt_duplicate_block{
	efi_hii_string_block header;
	efi_string_id        string_id;
}cdecl_attr_packed efi_hii_sibt_duplicate_block;
static_assert(sizeof(efi_hii_sibt_duplicate_block)==3,"efi_hii_sibt_duplicate_block size mismatch");
typedef struct efi_hii_sibt_end_block{
	efi_hii_string_block header;
}cdecl_attr_packed efi_hii_sibt_end_block;
static_assert(sizeof(efi_hii_sibt_end_block)==1,"efi_hii_sibt_end_block size mismatch");
typedef struct efi_hii_sibt_ext1_block{
	efi_hii_string_block header;
	uint8_t              type;
	uint8_t              length;
}cdecl_attr_packed efi_hii_sibt_ext1_block;
static_assert(sizeof(efi_hii_sibt_ext1_block)==3,"efi_hii_sibt_ext1_block size mismatch");
typedef struct efi_hii_sibt_ext2_block{
	efi_hii_string_block header;
	uint8_t              type;
	uint16_t             length;
}cdecl_attr_packed efi_hii_sibt_ext2_block;
static_assert(sizeof(efi_hii_sibt_ext2_block)==4,"efi_hii_sibt_ext2_block size mismatch");
typedef struct efi_hii_sibt_ext4_block{
	efi_hii_string_block header;
	uint8_t              type;
	uint32_t             length;
}cdecl_attr_packed efi_hii_sibt_ext4_block;
static_assert(sizeof(efi_hii_sibt_ext4_block)==6,"efi_hii_sibt_ext4_block size mismatch");
typedef struct efi_hii_sibt_font_block{
	efi_hii_sibt_ext2_block header;
	uint8_t                 font_id;
	uint16_t                font_size;
	efi_hii_font_style      font_style;
	char16                  font_name[0];
}cdecl_attr_packed efi_hii_sibt_font_block;
static_assert(sizeof(efi_hii_sibt_font_block)==11,"efi_hii_sibt_font_block size mismatch");
typedef struct efi_hii_sibt_skip1_block{
	efi_hii_string_block header;
	uint8_t              count;
}cdecl_attr_packed efi_hii_sibt_skip1_block;
static_assert(sizeof(efi_hii_sibt_skip1_block)==2,"efi_hii_sibt_skip1_block size mismatch");
typedef struct efi_hii_sibt_skip2_block{
	efi_hii_string_block header;
	uint16_t             count;
}cdecl_attr_packed efi_hii_sibt_skip2_block;
static_assert(sizeof(efi_hii_sibt_skip2_block)==3,"efi_hii_sibt_skip2_block size mismatch");
typedef struct efi_hii_sibt_string_scsu_block{
	efi_hii_string_block header;
	uint8_t              text[0];
}cdecl_attr_packed efi_hii_sibt_string_scsu_block;
static_assert(sizeof(efi_hii_sibt_string_scsu_block)==1,"efi_hii_sibt_string_scsu_block size mismatch");
typedef struct efi_hii_sibt_string_scsu_font_block{
	efi_hii_string_block header;
	uint8_t              font_id;
	uint8_t              text[0];
}cdecl_attr_packed efi_hii_sibt_string_scsu_font_block;
static_assert(sizeof(efi_hii_sibt_string_scsu_font_block)==2,"efi_hii_sibt_string_scsu_font_block size mismatch");
typedef struct efi_hii_sibt_strings_scsu_block{
	efi_hii_string_block header;
	uint16_t             count;
	uint8_t              text[0];
}cdecl_attr_packed efi_hii_sibt_strings_scsu_block;
static_assert(sizeof(efi_hii_sibt_strings_scsu_block)==3,"efi_hii_sibt_strings_scsu_block size mismatch");
typedef struct efi_hii_sibt_strings_scsu_font_block{
	efi_hii_string_block header;
	uint8_t              font_id;
	uint16_t             count;
	uint8_t              text[0];
}cdecl_attr_packed efi_hii_sibt_strings_scsu_font_block;
static_assert(sizeof(efi_hii_sibt_strings_scsu_font_block)==4,"efi_hii_sibt_strings_scsu_font_block size mismatch");
typedef struct efi_hii_sibt_string_ucs2_block{
	efi_hii_string_block header;
	char16               text[0];
}cdecl_attr_packed efi_hii_sibt_string_ucs2_block;
static_assert(sizeof(efi_hii_sibt_string_ucs2_block)==1,"efi_hii_sibt_string_ucs2_block size mismatch");
typedef struct efi_hii_sibt_string_ucs2_font_block{
	efi_hii_string_block header;
	uint8_t              font_id;
	char16               text[0];
}cdecl_attr_packed efi_hii_sibt_string_ucs2_font_block;
static_assert(sizeof(efi_hii_sibt_string_ucs2_font_block)==2,"efi_hii_sibt_string_ucs2_font_block size mismatch");
typedef struct efi_hii_sibt_strings_ucs2_block{
	efi_hii_string_block  header;
	uint16_t              count;
	char16                text[0];
}cdecl_attr_packed efi_hii_sibt_strings_ucs2_block;
static_assert(sizeof(efi_hii_sibt_strings_ucs2_block)==3,"efi_hii_sibt_strings_ucs2_block size mismatch");
typedef struct efi_hii_sibt_strings_ucs2_font_block{
	efi_hii_string_block header;
	uint8_t              font_id;
	uint16_t             count;
	char16               text[0];
}cdecl_attr_packed efi_hii_sibt_strings_ucs2_font_block;
static_assert(sizeof(efi_hii_sibt_strings_ucs2_font_block)==4,"efi_hii_sibt_strings_ucs2_font_block size mismatch");
typedef struct efi_hii_image_package_hdr{
	efi_hii_package_header header;
	uint32_t               image_info_offset;
	uint32_t               palette_info_offset;
}cdecl_attr_packed efi_hii_image_package_hdr;
static_assert(sizeof(efi_hii_image_package_hdr)==12,"efi_hii_image_package_hdr size mismatch");
typedef struct efi_hii_image_block{
	uint8_t type;
}cdecl_attr_packed efi_hii_image_block;
static_assert(sizeof(efi_hii_image_block)==1,"efi_hii_image_block size mismatch");
typedef struct efi_hii_iibt_end_block{
	efi_hii_image_block header;
}cdecl_attr_packed efi_hii_iibt_end_block;
static_assert(sizeof(efi_hii_iibt_end_block)==1,"efi_hii_iibt_end_block size mismatch");
typedef struct efi_hii_iibt_ext1_block{
	efi_hii_image_block header;
	uint8_t             type;
	uint8_t             length;
}cdecl_attr_packed efi_hii_iibt_ext1_block;
static_assert(sizeof(efi_hii_iibt_ext1_block)==3,"efi_hii_iibt_ext1_block size mismatch");
typedef struct efi_hii_iibt_ext2_block{
	efi_hii_image_block header;
	uint8_t             type;
	uint16_t            length;
}cdecl_attr_packed efi_hii_iibt_ext2_block;
static_assert(sizeof(efi_hii_iibt_ext2_block)==4,"efi_hii_iibt_ext2_block size mismatch");
typedef struct efi_hii_iibt_ext4_block{
	efi_hii_image_block header;
	uint8_t             type;
	uint32_t            length;
}cdecl_attr_packed efi_hii_iibt_ext4_block;
static_assert(sizeof(efi_hii_iibt_ext4_block)==6,"efi_hii_iibt_ext4_block size mismatch");
typedef struct efi_hii_iibt_image_1bit_base{
	uint16_t width;
	uint16_t height;
	uint8_t  data[0];
}cdecl_attr_packed efi_hii_iibt_image_1bit_base;
static_assert(sizeof(efi_hii_iibt_image_1bit_base)==4,"efi_hii_iibt_image_1bit_base size mismatch");
typedef struct efi_hii_iibt_image_1bit_block{
	efi_hii_image_block          header;
	uint8_t                      index;
	efi_hii_iibt_image_1bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_1bit_block;
static_assert(sizeof(efi_hii_iibt_image_1bit_block)==6,"efi_hii_iibt_image_1bit_block size mismatch");
typedef struct efi_hii_iibt_image_1bit_trans_block{
	efi_hii_image_block          header;
	uint8_t                      index;
	efi_hii_iibt_image_1bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_1bit_trans_block;
static_assert(sizeof(efi_hii_iibt_image_1bit_trans_block)==6,"efi_hii_iibt_image_1bit_trans_block size mismatch");
typedef struct efi_hii_rgb_pixel{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
}cdecl_attr_packed efi_hii_rgb_pixel;
static_assert(sizeof(efi_hii_rgb_pixel)==3,"efi_hii_rgb_pixel size mismatch");
typedef struct efi_hii_iibt_image_24bit_base{
	uint16_t          width;
	uint16_t          height;
	efi_hii_rgb_pixel bitmap[0];
}cdecl_attr_packed efi_hii_iibt_image_24bit_base;
static_assert(sizeof(efi_hii_iibt_image_24bit_base)==4,"efi_hii_iibt_image_24bit_base size mismatch");
typedef struct efi_hii_iibt_image_24bit_block{
	efi_hii_image_block           header;
	efi_hii_iibt_image_24bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_24bit_block;
static_assert(sizeof(efi_hii_iibt_image_24bit_block)==5,"efi_hii_iibt_image_24bit_block size mismatch");
typedef struct efi_hii_iibt_image_24bit_trans_block{
	efi_hii_image_block           header;
	efi_hii_iibt_image_24bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_24bit_trans_block;
static_assert(sizeof(efi_hii_iibt_image_24bit_trans_block)==5,"efi_hii_iibt_image_24bit_trans_block size mismatch");
typedef struct efi_hii_iibt_image_4bit_base{
	uint16_t width;
	uint16_t height;
	uint8_t  data[0];
}cdecl_attr_packed efi_hii_iibt_image_4bit_base;
static_assert(sizeof(efi_hii_iibt_image_4bit_base)==4,"efi_hii_iibt_image_4bit_base size mismatch");
typedef struct efi_hii_iibt_image_4bit_block{
	efi_hii_image_block          header;
	uint8_t                      index;
	efi_hii_iibt_image_4bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_4bit_block;
static_assert(sizeof(efi_hii_iibt_image_4bit_block)==6,"efi_hii_iibt_image_4bit_block size mismatch");
typedef struct efi_hii_iibt_image_4bit_trans_block{
	efi_hii_image_block          header;
	uint8_t                      index;
	efi_hii_iibt_image_4bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_4bit_trans_block;
static_assert(sizeof(efi_hii_iibt_image_4bit_trans_block)==6,"efi_hii_iibt_image_4bit_trans_block size mismatch");
typedef struct efi_hii_iibt_image_8bit_base{
	uint16_t width;
	uint16_t height;
	uint8_t  data[0];
}cdecl_attr_packed efi_hii_iibt_image_8bit_base;
static_assert(sizeof(efi_hii_iibt_image_8bit_base)==4,"efi_hii_iibt_image_8bit_base size mismatch");
typedef struct efi_hii_iibt_image_8bit_palette_block{
	efi_hii_image_block          header;
	uint8_t                      index;
	efi_hii_iibt_image_8bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_8bit_block;
static_assert(sizeof(efi_hii_iibt_image_8bit_block)==6,"efi_hii_iibt_image_8bit_block size mismatch");
typedef struct efi_hii_iibt_image_8bit_trans_block{
	efi_hii_image_block          header;
	uint8_t                      index;
	efi_hii_iibt_image_8bit_base bitmap;
}cdecl_attr_packed efi_hii_iibt_image_8bit_tran_block;
static_assert(sizeof(efi_hii_iibt_image_8bit_tran_block)==6,"efi_hii_iibt_image_8bit_tran_block size mismatch");
typedef struct efi_hii_iibt_duplicate_block{
	efi_hii_image_block header;
	efi_image_id        imageid;
}cdecl_attr_packed efi_hii_iibt_duplicate_block;
static_assert(sizeof(efi_hii_iibt_duplicate_block)==3,"efi_hii_iibt_duplicate_block size mismatch");
typedef struct efi_hii_iibt_jpeg_block{
	efi_hii_image_block header;
	uint32_t            size;
	uint8_t             data[0];
}cdecl_attr_packed efi_hii_iibt_jpeg_block;
static_assert(sizeof(efi_hii_iibt_jpeg_block)==5,"efi_hii_iibt_jpeg_block size mismatch");
typedef struct efi_hii_iibt_png_block{
	efi_hii_image_block header;
	uint32_t            size;
	uint8_t             data[0];
}cdecl_attr_packed efi_hii_iibt_png_block;
static_assert(sizeof(efi_hii_iibt_png_block)==5,"efi_hii_iibt_png_block size mismatch");
typedef struct efi_hii_iibt_skip1_block{
	efi_hii_image_block header;
	uint8_t             count;
}cdecl_attr_packed efi_hii_iibt_skip1_block;
static_assert(sizeof(efi_hii_iibt_skip1_block)==2,"efi_hii_iibt_skip1_block size mismatch");
typedef struct efi_hii_iibt_skip2_block{
	efi_hii_image_block header;
	uint16_t            count;
}cdecl_attr_packed efi_hii_iibt_skip2_block;
static_assert(sizeof(efi_hii_iibt_skip2_block)==3,"efi_hii_iibt_skip2_block size mismatch");
typedef struct efi_hii_image_palette_info_header{
	uint16_t count;
}cdecl_attr_packed efi_hii_image_palette_info_header;
static_assert(sizeof(efi_hii_image_palette_info_header)==2,"efi_hii_image_palette_info_header size mismatch");
typedef struct efi_hii_image_palette_info{
	uint16_t          size;
	efi_hii_rgb_pixel value[0];
}cdecl_attr_packed efi_hii_image_palette_info;
static_assert(sizeof(efi_hii_image_palette_info)==2,"efi_hii_image_palette_info size mismatch");
typedef enum efi_key{
	efi_key_left_ctrl,
	efi_key_a0,
	efi_key_left_alt,
	efi_key_space_bar,
	efi_key_a2,
	efi_key_a3,
	efi_key_a4,
	efi_key_right_ctrl,
	efi_key_left_arrow,
	efi_key_down_arrow,
	efi_key_right_arrow,
	efi_key_zero,
	efi_key_period,
	efi_key_enter,
	efi_key_lshift,
	efi_key_b0,
	efi_key_b1,
	efi_key_b2,
	efi_key_b3,
	efi_key_b4,
	efi_key_b5,
	efi_key_b6,
	efi_key_b7,
	efi_key_b8,
	efi_key_b9,
	efi_key_b10,
	efi_key_rshift,
	efi_key_up_arrow,
	efi_key_one,
	efi_key_two,
	efi_key_three,
	efi_key_caps_lock,
	efi_key_c1,
	efi_key_c2,
	efi_key_c3,
	efi_key_c4,
	efi_key_c5,
	efi_key_c6,
	efi_key_c7,
	efi_key_c8,
	efi_key_c9,
	efi_key_c10,
	efi_key_c11,
	efi_key_c12,
	efi_key_four,
	efi_key_five,
	efi_key_six,
	efi_key_plus,
	efi_key_tab,
	efi_key_d1,
	efi_key_d2,
	efi_key_d3,
	efi_key_d4,
	efi_key_d5,
	efi_key_d6,
	efi_key_d7,
	efi_key_d8,
	efi_key_d9,
	efi_key_d10,
	efi_key_d11,
	efi_key_d12,
	efi_key_d13,
	efi_key_del,
	efi_key_end,
	efi_key_page_dn,
	efi_key_seven,
	efi_key_eight,
	efi_key_nine,
	efi_key_e0,
	efi_key_e1,
	efi_key_e2,
	efi_key_e3,
	efi_key_e4,
	efi_key_e5,
	efi_key_e6,
	efi_key_e7,
	efi_key_e8,
	efi_key_e9,
	efi_key_e10,
	efi_key_e11,
	efi_key_e12,
	efi_key_backspace,
	efi_key_ins,
	efi_key_home,
	efi_key_page_up,
	efi_key_num_lck,
	efi_key_slash,
	efi_key_asterisk,
	efi_key_minus,
	efi_key_esc,
	efi_key_f1,
	efi_key_f2,
	efi_key_f3,
	efi_key_f4,
	efi_key_f5,
	efi_key_f6,
	efi_key_f7,
	efi_key_f8,
	efi_key_f9,
	efi_key_f10,
	efi_key_f11,
	efi_key_f12,
	efi_key_print,
	efi_key_scr_lck,
	efi_key_pause,
	efi_key_max = UINT32_MAX,
}cdecl_attr_packed efi_key;
static_assert(sizeof(efi_key)==4,"efi_key size mismatch");
typedef struct efi_key_descriptor{
	efi_key  key;
	char16   unicode;
	char16   shifted;
	char16   alt_gr;
	char16   shifted_alt_gr;
	uint16_t modifier;
	uint16_t affected_attr;
}cdecl_attr_packed efi_key_descriptor;
static_assert(sizeof(efi_key_descriptor)==16,"efi_key_descriptor size mismatch");
typedef struct efi_hii_keyboard_layout{
	uint16_t           length;
	efi_guid           guid;
	uint32_t           offset;
	uint8_t            count;
	efi_key_descriptor descriptors[];
}cdecl_attr_packed efi_hii_keyboard_layout;
static_assert(sizeof(efi_hii_keyboard_layout)==23,"efi_hii_keyboard_layout size mismatch");
typedef struct efi_hii_keyboard_package_hdr{
	efi_hii_package_header header;
	uint16_t               count;
}cdecl_attr_packed efi_hii_keyboard_package_hdr;
static_assert(sizeof(efi_hii_keyboard_package_hdr)==6,"efi_hii_keyboard_package_hdr size mismatch");
#pragma pack()
#endif //EFI_WRAPPER_EFI_HII_H
