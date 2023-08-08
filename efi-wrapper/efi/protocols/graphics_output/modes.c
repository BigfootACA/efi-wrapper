#include"internal.h"
efi_graphics_output_mode_information efi_gop_modes[]={
	{ 0, 1024, 768,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1024 },
	{ 0, 640,  480,  PIXEL_RGBA8888, {0, 0, 0, 0}, 640  },
	{ 0, 800,  480,  PIXEL_RGBA8888, {0, 0, 0, 0}, 800  },
	{ 0, 800,  600,  PIXEL_RGBA8888, {0, 0, 0, 0}, 800  },
	{ 0, 832,  624,  PIXEL_RGBA8888, {0, 0, 0, 0}, 832  },
	{ 0, 960,  640,  PIXEL_RGBA8888, {0, 0, 0, 0}, 960  },
	{ 0, 1024, 600,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1024 },
	{ 0, 1152, 864,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1152 },
	{ 0, 1152, 870,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1152 },
	{ 0, 1280, 720,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1280 },
	{ 0, 1280, 760,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1280 },
	{ 0, 1280, 768,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1280 },
	{ 0, 1280, 800,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1280 },
	{ 0, 1280, 960,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1280 },
	{ 0, 1280, 1024, PIXEL_RGBA8888, {0, 0, 0, 0}, 1280 },
	{ 0, 1360, 768,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1360 },
	{ 0, 1366, 768,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1366 },
	{ 0, 1400, 1050, PIXEL_RGBA8888, {0, 0, 0, 0}, 1400 },
	{ 0, 1440, 900,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1440 },
	{ 0, 1600, 900,  PIXEL_RGBA8888, {0, 0, 0, 0}, 1600 },
	{ 0, 1600, 1200, PIXEL_RGBA8888, {0, 0, 0, 0}, 1600 },
	{ 0, 1680, 1050, PIXEL_RGBA8888, {0, 0, 0, 0}, 1680 },
	{ 0, 1920, 1080, PIXEL_RGBA8888, {0, 0, 0, 0}, 1920 },
	{ 0, 1920, 1200, PIXEL_RGBA8888, {0, 0, 0, 0}, 1920 },
	{ 0, 1920, 1440, PIXEL_RGBA8888, {0, 0, 0, 0}, 1920 },
	{ 0, 2000, 2000, PIXEL_RGBA8888, {0, 0, 0, 0}, 2000 },
	{ 0, 2048, 1536, PIXEL_RGBA8888, {0, 0, 0, 0}, 2048 },
	{ 0, 2048, 2048, PIXEL_RGBA8888, {0, 0, 0, 0}, 2048 },
	{ 0, 2560, 1440, PIXEL_RGBA8888, {0, 0, 0, 0}, 2560 },
	{ 0, 2560, 1600, PIXEL_RGBA8888, {0, 0, 0, 0}, 2560 },
	{ 0, 2560, 2048, PIXEL_RGBA8888, {0, 0, 0, 0}, 2560 },
	{ 0, 2800, 2100, PIXEL_RGBA8888, {0, 0, 0, 0}, 2800 },
	{ 0, 3200, 2400, PIXEL_RGBA8888, {0, 0, 0, 0}, 3200 },
	{ 0, 3840, 2160, PIXEL_RGBA8888, {0, 0, 0, 0}, 3840 },
	{ 0, 4096, 2160, PIXEL_RGBA8888, {0, 0, 0, 0}, 4096 },
	{ 0, 7680, 4320, PIXEL_RGBA8888, {0, 0, 0, 0}, 7680 },
	{ 0, 8192, 4320, PIXEL_RGBA8888, {0, 0, 0, 0}, 8192 },
};

const efi_graphics_output_protocol_mode efi_gop_mode={
	.max_mode = (sizeof(efi_gop_modes)/sizeof(efi_graphics_output_mode_information)),
	.mode     = 0,
	.info     = (efi_graphics_output_mode_information*)efi_gop_modes,
	.size     = sizeof(efi_gop_modes),
	.fb_base  = {.ptr=NULL},
	.fb_size  = 0,
};