//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_DNS4_H
#define EFI_WRAPPER_DNS4_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_DNS4_SERVICE_BINDING_PROTOCOL_GUID {0xb625b186,0xe063,0x44f7,{0x89,0x05,0x6a,0x74,0xdc,0x6f,0x52,0xb4}}
#define EFI_DNS4_PROTOCOL_GUID                 {0xae3d28cc,0xe05b,0x4fa1,{0xa0,0x11,0x7e,0xb5,0x5a,0x3f,0x14,0x01}}
typedef struct efi_dns4_protocol efi_dns4_protocol;
typedef struct efi_dns4_config_data{
	uintn_t          server_count;
	efi_ipv4_address *server_list;
	bool             use_default_setting;
	bool             enable_cache;
	uint8_t          protocol;
	efi_ipv4_address station_ip;
	efi_ipv4_address subnet_mask;
	uint16_t         local_port;
	uint32_t         retry_count;
	uint32_t         retry_interval;
}efi_dns4_config_data;
typedef struct efi_dns4_cache_entry{
	char16           *hostname;
	efi_ipv4_address *ipaddress;
	uint32_t         timeout;
}efi_dns4_cache_entry;
typedef struct efi_dns4_mode_data{
	efi_dns4_config_data config;
	uint32_t             server_count;
	efi_ipv4_address     *server_list;
	uint32_t             cache_count;
	efi_dns4_cache_entry *cache_list;
}efi_dns4_mode_data;
typedef struct dns_host_to_addr_data{
	uint32_t         ip_count;
	efi_ipv4_address *ip_list;
}dns_host_to_addr_data;
typedef struct dns_addr_to_host_data{
	char16 *hostname;
}dns_addr_to_host_data;
typedef struct dns_resource_record{
	char8    *q_name;
	uint16_t q_type;
	uint16_t q_class;
	uint32_t ttl;
	uint16_t data_len;
	char8    *rdata;
}dns_resource_record;
typedef struct dns_general_lookup_data{
	uintn_t             rr_count;
	dns_resource_record *rr_list;
}dns_general_lookup_data;
typedef struct efi_dns4_completion_token{
	efi_event  event;
	efi_status status;
	uint32_t   retry_count;
	uint32_t   retry_interval;
	union{
		dns_host_to_addr_data   *h2a;
		dns_addr_to_host_data   *a2h;
		dns_general_lookup_data *g_lookup;
	}rsp;
}efi_dns4_completion_token;
typedef efi_status(efiapi*efi_dns4_get_mode_data)(
	efi_dns4_protocol  *this,
	efi_dns4_mode_data *data
);
typedef efi_status(efiapi*efi_dns4_configure)(
	efi_dns4_protocol    *this,
	efi_dns4_config_data *data
);
typedef efi_status(efiapi*efi_dns4_host_name_to_ip)(
	efi_dns4_protocol         *this,
	char16                    *hostname,
	efi_dns4_completion_token *token
);
typedef efi_status(efiapi*efi_dns4_ip_to_host_name)(
	efi_dns4_protocol         *this,
	efi_ipv4_address          address,
	efi_dns4_completion_token *token
);
typedef efi_status(efiapi*efi_dns4_general_lookup)(
	efi_dns4_protocol          *this,
	char8                      *qname,
	uint16_t                   q_type,
	uint16_t                   q_class,
	efi_dns4_completion_token  *token
);
typedef efi_status(efiapi*efi_dns4_update_dns_cache)(
	efi_dns4_protocol    *this,
	bool                 flag,
	bool                 override,
	efi_dns4_cache_entry entry
);
typedef efi_status(efiapi*efi_dns4_poll)(
	efi_dns4_protocol *this
);
typedef efi_status(efiapi*efi_dns4_cancel)(
	efi_dns4_protocol         *this,
	efi_dns4_completion_token *token
);
struct efi_dns4_protocol{
	efi_dns4_get_mode_data        get_mode_data;
	efi_dns4_configure            configure;
	efi_dns4_host_name_to_ip      hostname_to_ip;
	efi_dns4_ip_to_host_name      ip_to_hostname;
	efi_dns4_general_lookup       general_lookup;
	efi_dns4_update_dns_cache     update_dns_cache;
	efi_dns4_poll                 poll;
	efi_dns4_cancel               cancel;
};
extern const efi_guid gEfiDns4ServiceBindingProtocolGuid;
extern const efi_guid gEfiDns4ProtocolGuid;
#endif
