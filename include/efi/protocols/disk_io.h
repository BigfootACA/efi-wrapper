//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_DISK_IO_H
#define EFI_WRAPPER_DISK_IO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_DISK_IO_PROTOCOL_GUID  {0xce345171,0xba0b,0x11d2,{0x8e,0x4f,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_DISK_IO2_PROTOCOL_GUID {0x151c8eae,0x7f2c,0x472c,{0x9e,0x54,0x98,0x28,0x19,0x4f,0x6a,0x88}}
typedef struct efi_disk_io2_protocol efi_disk_io2_protocol;
typedef struct efi_disk_io_protocol efi_disk_io_protocol;
typedef efi_disk_io_protocol efi_disk_io;
typedef struct efi_disk_io2_token{
	efi_event  event;
	efi_status status;
}efi_disk_io2_token;
typedef efi_status(efiapi*efi_disk_read)(
	efi_disk_io_protocol *proto,
	uint32_t             media_id,
	uint64_t             offset,
	uintn_t              size,
	void                 *buffer
);
typedef efi_status(efiapi*efi_disk_write)(
	efi_disk_io_protocol *proto,
	uint32_t             media_id,
	uint64_t             offset,
	uintn_t              size,
	void                 *buffer
);

typedef efi_status(efiapi*efi_disk_cancel_ex)(
	efi_disk_io2_protocol*proto
);
typedef efi_status(efiapi*efi_disk_read_ex)(
	efi_disk_io2_protocol *proto,
	uint32_t              media_id,
	uint64_t              offset,
	efi_disk_io2_token    *token,
	uintn_t               size,
	void                  *buffer
);
typedef efi_status(efiapi*efi_disk_write_ex)(
	efi_disk_io2_protocol *proto,
	uint32_t              media_id,
	uint64_t              offset,
	efi_disk_io2_token    *token,
	uintn_t               size,
	void                  *buffer
);
typedef efi_status(efiapi*efi_disk_flush_ex)(
	efi_disk_io2_protocol*proto,
	efi_disk_io2_token*token
);
#define EFI_DISK_IO_PROTOCOL_REVISION1 0x00010000
#define EFI_DISK_IO_INTERFACE_REVISION EFI_DISK_IO_PROTOCOL_REVISION1
#define EFI_DISK_IO2_PROTOCOL_REVISION 0x00020000
struct efi_disk_io_protocol{
	uint64_t        revision;
	efi_disk_read   read_disk;
	efi_disk_write  write_disk;
};
struct efi_disk_io2_protocol{
	uint64_t           revision;
	efi_disk_cancel_ex cancel;
	efi_disk_read_ex   read_disk_ex;
	efi_disk_write_ex  write_disk_ex;
	efi_disk_flush_ex  flush_disk_ex;
};
extern const efi_guid gEfiDiskIoProtocolGuid;
extern const efi_guid gEfiDiskIo2ProtocolGuid;
#endif //EFI_WRAPPER_DISK_IO_H
