//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_SIMPLE_FILESYSTEM_H
#define EFI_WRAPPER_SIMPLE_FILESYSTEM_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID {0x964e5b22,0x6459,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_WRAPPER_POSIX_FD_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID {0x94c37a50,0xd365,0x48db,{0x0b,0xa7,0x11,0x55,0x12,0xf4,0xa8,0x50}}
typedef struct efi_simple_file_system_protocol efi_simple_file_system_protocol;
typedef struct efi_file_protocol efi_file_protocol;
typedef struct efi_file_protocol*efi_file_handle;
typedef efi_simple_file_system_protocol efi_file_io_interface;
typedef efi_status(efiapi*efi_simple_file_system_protocol_open_volume)(
	efi_simple_file_system_protocol*proto,
	efi_file_protocol**root
);
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION1  0x00010000
#define EFI_FILE_IO_INTERFACE_REVISION EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION1
struct efi_simple_file_system_protocol{
	uint64_t revision;
	efi_simple_file_system_protocol_open_volume open_volume;
};
typedef efi_status(efiapi*efi_file_open)(
	efi_file_protocol*proto,
	efi_file_protocol**handle,
	char16*file,
	uint64_t mode,
	uint64_t attrs
);
#define EFI_FILE_MODE_READ    0x0000000000000001ULL
#define EFI_FILE_MODE_WRITE   0x0000000000000002ULL
#define EFI_FILE_MODE_CREATE  0x8000000000000000ULL
#define EFI_FILE_READ_ONLY    0x0000000000000001ULL
#define EFI_FILE_HIDDEN       0x0000000000000002ULL
#define EFI_FILE_SYSTEM       0x0000000000000004ULL
#define EFI_FILE_RESERVED     0x0000000000000008ULL
#define EFI_FILE_DIRECTORY    0x0000000000000010ULL
#define EFI_FILE_ARCHIVE      0x0000000000000020ULL
#define EFI_FILE_VALID_ATTR   0x0000000000000037ULL
typedef efi_status(efiapi*efi_file_close)(
	efi_file_protocol*proto
);
typedef efi_status(efiapi*efi_file_delete)(
	efi_file_protocol*proto
);
typedef efi_status(efiapi*efi_file_read)(
	efi_file_protocol*proto,
	uintn_t*size,
	void*buffer
);
typedef efi_status(efiapi*efi_file_write)(
	efi_file_protocol*proto,
	uintn_t*size,
	void*buffer
);
typedef efi_status(efiapi*efi_file_set_position)(
	efi_file_protocol*proto,
	uint64_t pos
);
typedef efi_status(efiapi*efi_file_get_position)(
	efi_file_protocol*proto,
	uint64_t*pos
);
typedef efi_status(efiapi*efi_file_get_info)(
	efi_file_protocol *proto,
	const efi_guid    *type,
	uintn_t           *size,
	void              *buffer
);
typedef efi_status(efiapi*efi_file_set_info)(
	efi_file_protocol *proto,
	const efi_guid    *type,
	uintn_t           size,
	void              *buffer
);
typedef efi_status(efiapi*efi_file_flush)(
	efi_file_protocol*proto
);
typedef struct efi_file_io_token{
	efi_event  event;
	efi_status status;
	uintn_t    size;
	void       *buffer;
}efi_file_io_token;
typedef efi_status(efiapi*efi_file_open_ex)(
	efi_file_protocol *proto,
	efi_file_protocol **handle,
	char16            *file,
	uint64_t          mode,
	uint64_t          attrs,
	efi_file_io_token *token
);
typedef efi_status(efiapi*efi_file_read_ex)(
	efi_file_protocol*proto,
	efi_file_io_token*token
);
typedef efi_status(efiapi*efi_file_write_ex)(
	efi_file_protocol*proto,
	efi_file_io_token*token
);
typedef efi_status(efiapi*efi_file_flush_ex)(
	efi_file_protocol*proto,
	efi_file_io_token*token
);
#define EFI_FILE_PROTOCOL_REVISION1       0x00010000
#define EFI_FILE_PROTOCOL_REVISION2       0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2
#define EFI_FILE_REVISION EFI_FILE_PROTOCOL_REVISION
struct efi_file_protocol{
	uint64_t              revision;
	efi_file_open         open;
	efi_file_close        close;
	efi_file_delete       delete;
	efi_file_read         read;
	efi_file_write        write;
	efi_file_get_position get_pos;
	efi_file_set_position set_pos;
	efi_file_get_info     get_info;
	efi_file_set_info     set_info;
	efi_file_flush        flush;
	efi_file_open_ex      open_ex;
	efi_file_read_ex      read_ex;
	efi_file_write_ex     write_ex;
	efi_file_flush_ex     flush_ex;
};
extern const efi_guid gEfiSimpleFileSystemProtocolGuid;
extern const efi_guid gEfiWrapperPosixFDSimpleFileSystemProtocolGuid;
#endif //EFI_WRAPPER_SIMPLE_FILESYSTEM_H
