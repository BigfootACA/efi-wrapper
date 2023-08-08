//
// Created by bigfoot on 2023/5/3.
//

#ifndef EFI_WRAPPER_GRAPHICS_OUTPUT_H
#define EFI_WRAPPER_GRAPHICS_OUTPUT_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID                {0x9042a9de,0x23dc,0x4a38,{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}
#define EFI_WRAPPER_VNC_GRAPHICS_OUTPUT_PROTOCOL_GUID    {0x1113e60d,0xcc4b,0xf4dd,{0x0e,0xf2,0x99,0xe3,0x36,0xe1,0x3c,0x40}}
#define EFI_WRAPPER_SDL2_GRAPHICS_OUTPUT_PROTOCOL_GUID   {0x3ea1e3c5,0x0eae,0xdf4f,{0xb9,0x98,0xc3,0x9c,0xf9,0xc0,0x2a,0x99}}
#define EFI_WRAPPER_MEMORY_GRAPHICS_OUTPUT_PROTOCOL_GUID {0x0cc57ad3,0x6b34,0x7a49,{0x4f,0x26,0xc3,0x2c,0x29,0xd5,0x95,0xd1}}
typedef struct efi_graphics_output_protocol efi_graphics_output_protocol;
typedef struct efi_pixel_bitmask{
	uint32_t red_mask;
	uint32_t green_mask;
	uint32_t blue_mask;
	uint32_t reserved_mask;
}efi_pixel_bitmask;
typedef enum efi_graphics_pixel_format{
	PIXEL_RGBA8888,
	PIXEL_BGRA8888,
	PIXEL_BIT_MASK,
	PIXEL_BLT_ONLY,
	PIXEL_MAX
}efi_graphics_pixel_format;
typedef struct efi_graphics_output_mode_information{
	uint32_t                   version;
	uint32_t                   width;
	uint32_t                   height;
	efi_graphics_pixel_format  format;
	efi_pixel_bitmask          mask;
	uint32_t                   scan;
}efi_graphics_output_mode_information;
typedef efi_status(efiapi*efi_graphics_output_protocol_query_mode)(
	efi_graphics_output_protocol          *proto,
	uint32_t                              num,
	uintn_t                               *size,
	efi_graphics_output_mode_information  **info
);
typedef efi_status(efiapi*efi_graphics_output_protocol_set_mode)(
	efi_graphics_output_protocol *proto,
	uint32_t                     num
);
typedef struct efi_graphics_output_blt_pixel{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t reserved;
}efi_graphics_output_blt_pixel;
typedef union efi_graphics_output_blt_pixel_union{
	efi_graphics_output_blt_pixel pixel;
	uint32_t                      raw;
}efi_graphics_output_blt_pixel_union;
typedef enum efi_graphics_output_blt_operation{
	efi_blt_video_fill,
	efi_blt_video_to_blt_buffer,
	efi_blt_buffer_to_video,
	efi_blt_video_to_video,
	efi_graphics_output_blt_operation_max
}efi_graphics_output_blt_operation;
typedef efi_status(efiapi*efi_graphics_output_protocol_blt)(
	efi_graphics_output_protocol      *proto,
	efi_graphics_output_blt_pixel     *buffer,
	efi_graphics_output_blt_operation operation,
	uintn_t                           src_x,
	uintn_t                           src_y,
	uintn_t                           dst_x,
	uintn_t                           dst_y,
	uintn_t                           width,
	uintn_t                           height,
	uintn_t                           delta
);
typedef struct efi_graphics_output_protocol_mode{
	uint32_t                             max_mode;
	uint32_t                             mode;
	efi_graphics_output_mode_information *info;
	uintn_t                              size;
	efi_address                          fb_base;
	uintn_t                              fb_size;
}efi_graphics_output_protocol_mode;
struct efi_graphics_output_protocol{
	efi_graphics_output_protocol_query_mode  query_mode;
	efi_graphics_output_protocol_set_mode    set_mode;
	efi_graphics_output_protocol_blt         blt;
	efi_graphics_output_protocol_mode        *mode;
};
extern const efi_guid gEfiGraphicsOutputProtocolGuid;
extern const efi_guid gEfiWrapperMemoryGraphicsOutputProtocolGuid;
extern const efi_guid gEfiWrapperSDL2GraphicsOutputProtocolGuid;
extern const efi_guid gEfiWrapperVNCGraphicsOutputProtocolGuid;
#endif //EFI_WRAPPER_GRAPHICS_OUTPUT_H
