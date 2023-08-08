//
// Created by bigfoot on 2023/5/11.
//

#ifndef EFI_WRAPPER_HII_IMAGE_H
#define EFI_WRAPPER_HII_IMAGE_H
#include"graphics_output.h"
#include"../efi_hii.h"
#define EFI_HII_IMAGE_PROTOCOL_GUID {0x31a6406a,0x6bdf,0x4e46,{0xb2,0xa2,0xeb,0xaa,0x89,0xc4,0x09,0x20}}
typedef struct efi_hii_image_protocol efi_hii_image_protocol;
#define EFI_IMAGE_TRANSPARENT 0x00000001
typedef struct efi_image_input{
	uint32_t flags;
	uint16_t width;
	uint16_t height;
	efi_graphics_output_blt_pixel*bitmap;
}efi_image_input;
typedef efi_status(efiapi*efi_hii_new_image)(
	const efi_hii_image_protocol *proto,
	efi_hii_handle               pkg_list,
	efi_image_id                 *id,
	const efi_image_input        *image
);
typedef efi_status(efiapi*efi_hii_get_image)(
	const  efi_hii_image_protocol  *proto,
	efi_hii_handle                 pkg_list,
	efi_image_id                   id,
	efi_image_input                *image
);
typedef efi_status(efiapi*efi_hii_set_image)(
	const efi_hii_image_protocol  *proto,
	efi_hii_handle                pkg_list,
	efi_image_id                  id,
	const efi_image_input         *image
);
typedef uint32_t efi_hii_draw_flags;
#define EFI_HII_DRAW_FLAG_CLIP          0x00000001
#define EFI_HII_DRAW_FLAG_TRANSPARENT   0x00000030
#define EFI_HII_DRAW_FLAG_DEFAULT       0x00000000
#define EFI_HII_DRAW_FLAG_FORCE_TRANS   0x00000010
#define EFI_HII_DRAW_FLAG_FORCE_OPAQUE  0x00000020
#define EFI_HII_DIRECT_TO_SCREEN        0x00000080
typedef struct efi_image_output{
	uint16_t width;
	uint16_t height;
	union{
		efi_graphics_output_blt_pixel*bitmap;
		efi_graphics_output_protocol*screen;
	}image;
}efi_image_output;
typedef efi_status(efiapi*efi_hii_draw_image)(
	const efi_hii_image_protocol *proto,
	efi_hii_draw_flags           flags,
	const efi_image_input        *image,
	efi_image_output             **blt,
	uintn_t                      blt_x,
	uintn_t                      blt_y
);
typedef efi_status(efiapi*efi_hii_draw_image_id)(
	const efi_hii_image_protocol *proto,
	efi_hii_draw_flags           flags,
	efi_hii_handle               pkg_list,
	efi_image_id                 id,
	efi_image_output             **blt,
	uintn_t                      blt_x,
	uintn_t                      blt_y
);
struct efi_hii_image_protocol{
	efi_hii_new_image     new_image;
	efi_hii_get_image     get_image;
	efi_hii_set_image     set_image;
	efi_hii_draw_image    draw_image;
	efi_hii_draw_image_id draw_image_id;
};
extern const efi_guid gEfiHiiImageProtocolGuid;
#endif //EFI_WRAPPER_HII_IMAGE_H
