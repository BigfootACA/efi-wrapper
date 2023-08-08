//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_SERIAL_IO_H
#define EFI_WRAPPER_SERIAL_IO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_SERIAL_IO_PROTOCOL_GUID          {0xbb25cf6f,0xf1d4,0x11d2,{0x9a,0x0c,0x00,0x90,0x27,0x3f,0xc1,0xfd}}
#define EFI_SERIAL_TERMINAL_DEVICE_TYPE_GUID {0x6ad9a60f,0x5815,0x4c7c,{0x8a,0x10,0x50,0x53,0xd2,0xbf,0x7a,0x1b}}
typedef struct efi_serial_io_protocol efi_serial_io_protocol;
typedef efi_serial_io_protocol serial_io_interface;
typedef enum efi_parity_type{
	parity_default,
	parity_no,
	parity_even,
	parity_odd,
	parity_mark,
	parity_space
}efi_parity_type;
typedef enum efi_stop_bits_type{
	stop_bits_default,
	stop_bits_one,
	stop_bits_one_five,
	stop_bits_two
}efi_stop_bits_type;
#define EFI_SERIAL_CLEAR_TO_SEND                0x00000010
#define EFI_SERIAL_DATA_SET_READY               0x00000020
#define EFI_SERIAL_RING_INDICATE                0x00000040
#define EFI_SERIAL_CARRIER_DETECT               0x00000080
#define EFI_SERIAL_INPUT_BUFFER_EMPTY           0x00000100
#define EFI_SERIAL_OUTPUT_BUFFER_EMPTY          0x00000200
#define EFI_SERIAL_REQUEST_TO_SEND              0x00000002
#define EFI_SERIAL_DATA_TERMINAL_READY          0x00000001
#define EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE     0x00001000
#define EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE     0x00002000
#define EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE 0x00004000
typedef efi_status(efiapi*efi_serial_reset)(
	efi_serial_io_protocol*proto
);
typedef efi_status(efiapi*efi_serial_set_attributes)(
	efi_serial_io_protocol *proto,
	uint64_t               baud_rate,
	uint32_t               receive_fifo_depth,
	uint32_t               timeout,
	efi_parity_type        parity,
	uint8_t                data_bits,
	efi_stop_bits_type     stop_bits
);
typedef efi_status(efiapi*efi_serial_set_control_bits)(
	efi_serial_io_protocol*proto,
	uint32_t control
);
typedef efi_status(efiapi*efi_serial_get_control_bits)(
	efi_serial_io_protocol*proto,
	uint32_t*control
);
typedef efi_status(efiapi*efi_serial_write)(
	efi_serial_io_protocol *proto,
	uintn_t                *size,
	void                   *buffer
);
typedef efi_status(efiapi*efi_serial_read)(
	efi_serial_io_protocol *proto,
	uintn_t                *size,
	void                   *buffer
);
typedef struct efi_serial_io_mode{
	uint32_t control_mask;
	uint32_t timeout;
	uint64_t baud_rate;
	uint32_t receive_fifo_depth;
	uint32_t data_bits;
	uint32_t parity;
	uint32_t stop_bits;
}efi_serial_io_mode;
#define EFI_SERIAL_IO_PROTOCOL_REVISION1 0x00010000
#define EFI_SERIAL_IO_PROTOCOL_REVISION2 0x00010001
#define SERIAL_IO_INTERFACE_REVISION EFI_SERIAL_IO_PROTOCOL_REVISION1
struct efi_serial_io_protocol{
	uint32_t                    revision;
	efi_serial_reset            reset;
	efi_serial_set_attributes   set_attrs;
	efi_serial_set_control_bits set_control;
	efi_serial_get_control_bits get_control;
	efi_serial_write            write;
	efi_serial_read             read;
	efi_serial_io_mode          *mode;
	const efi_guid              *device_type_guid;
};
extern const efi_guid gEfiSerialIoProtocolGuid;
extern const efi_guid gEfiSerialTerminalDeviceTypeGuid;
#endif //EFI_WRAPPER_SERIAL_IO_H
