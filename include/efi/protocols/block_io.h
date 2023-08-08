//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_BLOCK_IO_H
#define EFI_WRAPPER_BLOCK_IO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_BLOCK_IO_PROTOCOL_GUID  {0x964e5b21,0x6459,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_BLOCK_IO2_PROTOCOL_GUID {0xa77b2472,0xe282,0x4e9f,{0xa2,0x45,0xc2,0xc0,0xe2,0x7b,0xbc,0xc1}}
typedef struct efi_block_io_protocol efi_block_io_protocol;
typedef struct efi_block_io2_protocol efi_block_io2_protocol;
typedef efi_block_io_protocol efi_block_io;
typedef struct efi_block_io2_token{
	efi_event event;
	efi_status status;
}efi_block_io2_token;
typedef efi_status(efiapi*efi_block_reset)(
	efi_block_io_protocol*proto,
	bool ext_verify
);
typedef efi_status(efiapi*efi_block_read)(
	efi_block_io_protocol *proto,
	uint32_t              Media_Id,
	efi_lba               lba,
	uintn_t               size,
	void                  *buffer
);
typedef efi_status(efiapi*efi_block_write)(
	efi_block_io_protocol *proto,
	uint32_t              media_id,
	efi_lba               lba,
	uintn_t               size,
	void                  *buffer
);
typedef efi_status(efiapi*efi_block_flush)(
	efi_block_io_protocol  *proto
);
typedef struct efi_block_io_media{
	uint32_t media_id;
	bool     removable_media;
	bool     media_present;
	bool     logical_partition;
	bool     read_only;
	bool     write_caching;
	uint32_t block_size;
	uint32_t io_align;
	efi_lba  last_block;
	efi_lba  lowest_aligned_lba;
	uint32_t logical_blocks_per_physical_block;
	uint32_t optimal_transfer_length_granularity;
}efi_block_io_media;
#define EFI_BLOCK_IO_PROTOCOL_REVISION1 0x00010000
#define EFI_BLOCK_IO_PROTOCOL_REVISION2 0x00020001
#define EFI_BLOCK_IO_PROTOCOL_REVISION3 0x0002001F
#define EFI_BLOCK_IO_INTERFACE_REVISION EFI_BLOCK_IO_PROTOCOL_REVISION1
typedef efi_status(efiapi*efi_block_reset_ex)(
	efi_block_io2_protocol*proto,
	bool ext_verify
);
typedef efi_status(efiapi*efi_block_read_ex)(
	efi_block_io2_protocol *proto,
	uint32_t               media_id,
	efi_lba                lba,
	efi_block_io2_token    *token,
	uintn_t                size,
	void                   *buffer
);
typedef efi_status(efiapi*efi_block_write_ex)(
	efi_block_io2_protocol *proto,
	uint32_t               media_id,
	efi_lba                lba,
	efi_block_io2_token    *token,
	uintn_t                size,
	void                   *buffer
);
typedef efi_status(efiapi*efi_block_flush_ex)(
	efi_block_io2_protocol*proto,
	efi_block_io2_token*token
);
struct efi_block_io_protocol{
	uint64_t            revision;
	efi_block_io_media  *media;
	efi_block_reset     reset;
	efi_block_read      read_blocks;
	efi_block_write     write_blocks;
	efi_block_flush     flush_blocks;
};
struct efi_block_io2_protocol{
	efi_block_io_media *media;
	efi_block_reset_ex reset;
	efi_block_read_ex  read_blocks_ex;
	efi_block_write_ex write_blocks_ex;
	efi_block_flush_ex flush_blocks_ex;
};
extern const efi_guid gEfiBlockIoProtocolGuid;
extern const efi_guid gEfiBlockIo2ProtocolGuid;
#endif //EFI_WRAPPER_BLOCK_IO_H
