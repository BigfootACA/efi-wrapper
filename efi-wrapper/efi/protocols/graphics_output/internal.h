#ifndef EFI_WRAPPER_INTERNAL_H
#define EFI_WRAPPER_INTERNAL_H
#include<errno.h>
#include<wchar.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/poll.h>
#include<sys/mman.h>
#include"defines.h"
#include"efi/efi_spec.h"
#include"efi/efi_string.h"
#include"efi/efi_context.h"
#include"efi/device_path.h"
#include"efi/protocols/graphics_output.h"
#include"efi/protocols/component_name.h"

typedef struct fb_cfg fb_cfg;
typedef struct efi_graphics_output_protocol_private{
	efi_graphics_output_protocol protocol;
	efi_guid magic;
	mem_pool*pool;
	efi_graphics_output_protocol_mode mode;
	efi_graphics_output_mode_information*cur;
	size_t size;
	void*buffer;
	fb_cfg*cfg;
	bool reallocate;
}efi_graphics_output_protocol_private;
extern efiapi efi_status efi_gop_query_mode(
	efi_graphics_output_protocol          *proto,
	uint32_t                              num,
	uintn_t                               *size,
	efi_graphics_output_mode_information  **info
);
extern efi_status efi_update_size(
	efi_graphics_output_protocol_private*info,
	efi_graphics_output_mode_information*oi,
	efi_graphics_output_mode_information*ni,
	void*buffer
);
extern efi_status fb_blt(
	fb_cfg*cfg,
	efi_graphics_output_blt_pixel*buffer,
	efi_graphics_output_blt_operation oper,
	uintn_t src_x,uintn_t src_y,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height,
	uintn_t delta
);
extern efi_status efiapi fb_blt_setup(
	void*buffer,
	efi_graphics_output_mode_information*info,
	fb_cfg*cfg,uintn_t*size
);
extern efi_graphics_output_mode_information efi_gop_modes[];
extern const efi_graphics_output_protocol_mode efi_gop_mode;
#endif //EFI_WRAPPER_INTERNAL_H
