//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_ARP_H
#define EFI_WRAPPER_ARP_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_ARP_SERVICE_BINDING_PROTOCOL_GUID {0xf44c00ee,0x1f2c,0x4a00,{0xaa,0x09,0x1c,0x9f,0x3e,0x08,0x00,0xa3}}
#define EFI_ARP_PROTOCOL_GUID                 {0xf4b427bb,0xba21,0x4f16,{0xbc,0x4e,0x43,0xe4,0x16,0xab,0x61,0x9c}}
typedef struct efi_arp_protocol efi_arp_protocol;
typedef struct efi_arp_find_data{
	uint32_t size;
	bool     deny_flag;
	bool     static_flag;
	uint16_t hw_addr_type;
	uint16_t sw_addr_type;
	uint8_t  hw_addr_length;
	uint8_t  sw_addr_length;
}efi_arp_find_data;
typedef struct efi_arp_config_data{
	uint16_t sw_addr_type;
	uint8_t  sw_addr_length;
	void     *station_addr;
	uint32_t entry_timeout;
	uint32_t retry_count;
	uint32_t retry_timeout;
}efi_arp_config_data;
typedef efi_status(efiapi*efi_arp_configure)(
	efi_arp_protocol    *this,
	efi_arp_config_data *config
);
typedef efi_status(efiapi*efi_arp_add)(
	efi_arp_protocol *this,
	bool             deny_flag,
	void             *target_sw_addr,
	void             *target_hw_addr,
	uint32_t         timeout,
	bool             overwrite
);
typedef efi_status(efiapi*efi_arp_find)(
	efi_arp_protocol  *this,
	bool              by_sw_addr,
	void              *addr_buf,
	uint32_t          *entry_len,
	uint32_t          *entry_cnt,
	efi_arp_find_data **entries,
	bool              refresh
);
typedef efi_status(efiapi*efi_arp_delete)(
	efi_arp_protocol *This,
	bool             by_sw_addr,
	void             *addr_buf
);
typedef efi_status(efiapi*efi_arp_flush)(
	efi_arp_protocol *this
);
typedef efi_status(efiapi*efi_arp_request)(
	efi_arp_protocol *this,
	void             *target_sw_addr,
	efi_event        resolved_event,
	void             *target_hw_addr
);
typedef efi_status(efiapi*efi_arp_cancel)(
	efi_arp_protocol *This,
	void             *target_sw_addr,
	efi_event        resolved_event
);
struct efi_arp_protocol{
	efi_arp_configure configure;
	efi_arp_add       add;
	efi_arp_find      find;
	efi_arp_delete    delete;
	efi_arp_flush     flush;
	efi_arp_request   request;
	efi_arp_cancel    cancel;
};
extern const efi_guid gEfiArpServiceBindingProtocolGuid;
extern const efi_guid gEfiArpProtocolGuid;
#endif
