#ifndef EFI_WRAPPER_PCI_IO_H
#define EFI_WRAPPER_PCI_IO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_PCI_IO_PROTOCOL_GUID {0x4cf5b200,0x68b8,0x4ca5,{0x9e,0xec,0xb2,0x3e,0x3f,0x50,0x02,0x9a}}
typedef struct efi_pci_io_protocol efi_pci_io_protocol;
typedef enum efi_pci_io_protocol_width{
	efi_pci_io_width_uint8 = 0,
	efi_pci_io_width_uint16,
	efi_pci_io_width_uint32,
	efi_pci_io_width_uint64,
	efi_pci_io_width_fifo_uint8,
	efi_pci_io_width_fifo_uint16,
	efi_pci_io_width_fifo_uint32,
	efi_pci_io_width_fifo_uint64,
	efi_pci_io_width_fill_uint8,
	efi_pci_io_width_fill_uint16,
	efi_pci_io_width_fill_uint32,
	efi_pci_io_width_fill_uint64,
	efi_pci_io_width_maximum
}efi_pci_io_protocol_width;
#define EFI_PCI_IO_PASS_THROUGH_BAR               0xff
#define EFI_PCI_IO_ATTRIBUTE_MASK                 0x077f
#define EFI_PCI_IO_ATTRIBUTE_ISA_MOTHERBOARD_IO   0x0001
#define EFI_PCI_IO_ATTRIBUTE_ISA_IO               0x0002
#define EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO       0x0004
#define EFI_PCI_IO_ATTRIBUTE_VGA_MEMORY           0x0008
#define EFI_PCI_IO_ATTRIBUTE_VGA_IO               0x0010
#define EFI_PCI_IO_ATTRIBUTE_IDE_PRIMARY_IO       0x0020
#define EFI_PCI_IO_ATTRIBUTE_IDE_SECONDARY_IO     0x0040
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE 0x0080
#define EFI_PCI_IO_ATTRIBUTE_IO                   0x0100
#define EFI_PCI_IO_ATTRIBUTE_MEMORY               0x0200
#define EFI_PCI_IO_ATTRIBUTE_BUS_MASTER           0x0400
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED        0x0800
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_DISABLE       0x1000
#define EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE      0x2000
#define EFI_PCI_IO_ATTRIBUTE_EMBEDDED_ROM         0x4000
#define EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE   0x8000
#define EFI_PCI_IO_ATTRIBUTE_ISA_IO_16            0x10000
#define EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO_16    0x20000
#define EFI_PCI_IO_ATTRIBUTE_VGA_IO_16            0x40000
#define EFI_PCI_DEVICE_ENABLE (EFI_PCI_IO_ATTRIBUTE_IO | EFI_PCI_IO_ATTRIBUTE_MEMORY | EFI_PCI_IO_ATTRIBUTE_BUS_MASTER)
#define EFI_VGA_DEVICE_ENABLE (EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO | EFI_PCI_IO_ATTRIBUTE_VGA_MEMORY | EFI_PCI_IO_ATTRIBUTE_VGA_IO | EFI_PCI_IO_ATTRIBUTE_IO)
typedef enum efi_pci_io_protocol_operation{
	efi_pci_io_oper_bus_master_read,
	efi_pci_io_oper_bus_master_write,
	efi_pci_io_oper_bus_master_buffer,
	efi_pci_io_oper_maximum
}efi_pci_io_protocol_operation;
typedef enum efi_pci_io_protocol_attribute_operation{
	efi_pci_io_attr_oper_get,
	efi_pci_io_attr_oper_set,
	efi_pci_io_attr_oper_enable,
	efi_pci_io_attr_oper_disable,
	efi_pci_io_attr_oper_supported,
	efi_pci_io_attr_oper_maximum
}efi_pci_io_protocol_attribute_operation;
typedef efi_status(efiapi*efi_pci_io_protocol_poll_io_mem)(
	efi_pci_io_protocol       *this,
	efi_pci_io_protocol_width width,
	uint8_t                   bar,
	uint64_t                  offset,
	uint64_t                  mask,
	uint64_t                  value,
	uint64_t                  delay,
	uint64_t                  *result
);
typedef efi_status(efiapi*efi_pci_io_protocol_io_mem)(
	efi_pci_io_protocol       *this,
	efi_pci_io_protocol_width width,
	uint8_t                   bar,
	uint64_t                  offset,
	uintn_t                   count,
	void                      *buffer
);
typedef efi_status(efiapi*efi_pci_io_protocol_config)(
	efi_pci_io_protocol       *this,
	efi_pci_io_protocol_width width,
	uint32_t                  offset,
	uintn_t                   count,
	void                      *buffer
);
typedef efi_status(efiapi*efi_pci_io_protocol_copy_mem)(
	efi_pci_io_protocol       *this,
	efi_pci_io_protocol_width width,
	uint8_t                   dest_bar,
	uint64_t                  dest_offset,
	uint8_t                   src_bar,
	uint64_t                  src_offset,
	uintn_t                   count
);
typedef efi_status(efiapi*efi_pci_io_protocol_map)(
	efi_pci_io_protocol            *this,
	efi_pci_io_protocol_operation  oper,
	void                           *host,
	uintn_t                        *bytes,
	efi_address                    *device,
	void                           **mapping
);
typedef efi_status(efiapi*efi_pci_io_protocol_unmap)(
	efi_pci_io_protocol *this,
	void                *mapping
);
typedef efi_status(efiapi*efi_pci_io_protocol_allocate_buffer)(
	efi_pci_io_protocol *this,
	efi_allocate_type   type,
	efi_memory_type     memory,
	uintn_t             pages,
	void                **host,
	uint64_t            attrs
);
typedef efi_status(efiapi*efi_pci_io_protocol_free_buffer)(
	efi_pci_io_protocol *this,
	uintn_t             pages,
	void                *host
);
typedef efi_status(efiapi*efi_pci_io_protocol_flush)(
	efi_pci_io_protocol*this
);
typedef efi_status(efiapi*efi_pci_io_protocol_get_location)(
	efi_pci_io_protocol *this,
	uintn_t             *segment,
	uintn_t             *bus,
	uintn_t             *device,
	uintn_t             *func
);
typedef efi_status(efiapi*efi_pci_io_protocol_attributes)(
	efi_pci_io_protocol                      This,
	efi_pci_io_protocol_attribute_operation  oper,
	uint64_t                                 attrs,
	uint64_t                                 *result
);
typedef efi_status(efiapi*efi_pci_io_protocol_get_bar_attributes)(
	efi_pci_io_protocol *this,
	uint8_t             bar,
	uint64_t            *supports,
	void                **resources
);
typedef efi_status(efiapi*efi_pci_io_protocol_set_bar_attributes)(
	efi_pci_io_protocol *this,
	uint64_t            attributes,
	uint8_t             bar,
	uint64_t            *offset,
	uint64_t            *length
);
typedef struct efi_pci_io_protocol_access{
	efi_pci_io_protocol_io_mem read;
	efi_pci_io_protocol_io_mem write;
}efi_pci_io_protocol_access;
typedef struct efi_pci_io_protocol_config_access{
	efi_pci_io_protocol_config read;
	efi_pci_io_protocol_config write;
}efi_pci_io_protocol_config_access;
struct efi_pci_io_protocol{
	efi_pci_io_protocol_poll_io_mem         poll_mem;
	efi_pci_io_protocol_poll_io_mem         poll_io;
	efi_pci_io_protocol_access              mem;
	efi_pci_io_protocol_access              io;
	efi_pci_io_protocol_config_access       pci;
	efi_pci_io_protocol_copy_mem            copy_mem;
	efi_pci_io_protocol_map                 map;
	efi_pci_io_protocol_unmap               unmap;
	efi_pci_io_protocol_allocate_buffer     alloc_buffer;
	efi_pci_io_protocol_free_buffer         free_buffer;
	efi_pci_io_protocol_flush               flush;
	efi_pci_io_protocol_get_location        get_location;
	efi_pci_io_protocol_attributes          attributes;
	efi_pci_io_protocol_get_bar_attributes  get_bar_attributes;
	efi_pci_io_protocol_set_bar_attributes  set_bar_attributes;
	uint64_t                                rom_size;
	void                                    *rom_image;
};
extern const efi_guid gEfiPciIoProtocolGuid;
#endif
