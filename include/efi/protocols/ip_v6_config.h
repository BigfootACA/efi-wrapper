#ifndef EFI_WRAPPER_IP_V6_CONFIG_H
#define EFI_WRAPPER_IP_V6_CONFIG_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"ip_v6.h"
#define EFI_IP6_CONFIG_PROTOCOL_GUID {0x937fe521,0x95ae,0x4d1a,{0x89,0x29,0x48,0xbc,0xd9,0x0a,0xd3,0x1a}}
typedef struct efi_ip6_config_protocol efi_ip6_config_protocol;
typedef enum efi_ip6_config_data_type{
	ip6cfg_type_int_info,
	ip6cfg_type_alt_int_id,
	ip6cfg_type_policy,
	ip6cfg_type_dup_addr_detect_transmits,
	ip6cfg_type_manual_addr,
	ip6cfg_type_gateway,
	ip6cfg_type_dns,
	ip6cfg_type_max
}efi_ip6_config_data_type;
typedef struct efi_ip6_config_interface_info{
	char16                name[32];
	uint8_t               if_type;
	uint32_t              hw_addr_size;
	efi_mac_address       hw_addr;
	uint32_t              addr_info_count;
	efi_ip6_address_info  *addr_info;
	uint32_t              route_count;
	efi_ip6_route_table   *route_table;
}efi_ip6_config_interface_info;
typedef struct efi_ip6_config_interface_id{
	uint8_t id[8];
}efi_ip6_config_interface_id;
typedef enum efi_ip6_config_policy{
	ip6cfg_policy_manual,
	ip6cfg_policy_auto
}efi_ip6_config_policy;
typedef struct efi_ip6_config_dup_addr_detect_transmits{
	uint32_t dup_addr_detect_transmits;
}efi_ip6_config_dup_addr_detect_transmits;
typedef struct efi_ip6_config_manual_address{
	efi_ipv6_address addr;
	bool             is_anycast;
	uint8_t          prefix;
}efi_ip6_config_manual_address;
typedef efi_status(efiapi*efi_ip6_config_set_data)(
	efi_ip6_config_protocol  *this,
	efi_ip6_config_data_type data_type,
	uintn_t                  data_size,
	void                     *data
);
typedef efi_status(efiapi*efi_ip6_config_get_data)(
	efi_ip6_config_protocol  *this,
	efi_ip6_config_data_type data_type,
	uintn_t                  *data_size,
	void                     *data
);
typedef efi_status(efiapi*efi_ip6_config_register_notify)(
	efi_ip6_config_protocol  *this,
	efi_ip6_config_data_type data_type,
	efi_event                event
);
typedef efi_status(efiapi*efi_ip6_config_unregister_notify)(
	efi_ip6_config_protocol  *this,
	efi_ip6_config_data_type data_type,
	efi_event                event
);
struct efi_ip6_config_protocol{
	efi_ip6_config_set_data          set_data;
	efi_ip6_config_get_data          get_data;
	efi_ip6_config_register_notify   register_data_notify;
	efi_ip6_config_unregister_notify unregister_data_notify;
};
extern const efi_guid gEfiIp6ConfigProtocolGuid;
#endif

