#ifndef EFI_WRAPPER_IP_V4_CONFIG2_H
#define EFI_WRAPPER_IP_V4_CONFIG2_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"ip_v4.h"
#define EFI_IP4_CONFIG2_PROTOCOL_GUID {0x5b446ed1,0xe30b,0x4faa,{0x87,0x1a,0x36,0x54,0xec,0xa3,0x60,0x80}}
typedef struct efi_ip4_config2_protocol efi_ip4_config2_protocol;
typedef enum efi_ip4_config2_data_type{
	ip4cfg2_type_int_info,
	ip4cfg2_type_policy,
	ip4cfg2_type_man_addr,
	ip4cfg2_type_gateway,
	ip4cfg2_type_dns,
	ip4cfg2_type_max
}efi_ip4_config2_data_type;
#define EFI_IP4_CONFIG2_INTERFACE_INFO_NAME_SIZE 32
typedef struct{
	char16              name[EFI_IP4_CONFIG2_INTERFACE_INFO_NAME_SIZE];
	uint8_t             if_type;
	uint32_t            hw_addr_size;
	efi_mac_address     hw_addr;
	efi_ipv4_address    station_addr;
	efi_ipv4_address    subnet_mask;
	uint32_t            route_table_size;
	efi_ip4_route_table *route_table;
}efi_ip4_config2_interface_info;
typedef enum efi_ip4_config2_policy{
	ip4cfg2_policy_static,
	ip4cfg2_policy_dhcp,
	ip4cfg2_policy_max
}efi_ip4_config2_policy;
typedef struct efi_ip4_config2_manual_address{
	efi_ipv4_address addr;
	efi_ipv4_address subnet_mask;
}efi_ip4_config2_manual_address;
typedef efi_status(efiapi*efi_ip4_config2_set_data)(
	efi_ip4_config2_protocol   *this,
	efi_ip4_config2_data_type  type,
	uintn_t                    size,
	void                       *data
);
typedef efi_status(efiapi*efi_ip4_config2_get_data)(
	efi_ip4_config2_protocol  *this,
	efi_ip4_config2_data_type type,
	uintn_t                   *size,
	void                      *data
);
typedef efi_status(efiapi*efi_ip4_config2_register_notify)(
	efi_ip4_config2_protocol  *this,
	efi_ip4_config2_data_type type,
	efi_event                 event
);
typedef efi_status(efiapi*efi_ip4_config2_unregister_notify)(
	efi_ip4_config2_protocol  *this,
	efi_ip4_config2_data_type type,
	efi_event                 event
);
struct efi_ip4_config2_protocol{
	efi_ip4_config2_set_data          set_data;
	efi_ip4_config2_get_data          get_data;
	efi_ip4_config2_register_notify   register_data_notify;
	efi_ip4_config2_unregister_notify unregister_data_notify;
};
extern const efi_guid gEfiIp4Config2ProtocolGuid;
#endif
