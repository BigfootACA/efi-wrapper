//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_DNS6_H
#define EFI_WRAPPER_DNS6_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_DNS6_SERVICE_BINDING_PROTOCOL_GUID {0x7f1647c8,0xb76e,0x44b2,{0xa5,0x65,0xf7,0x0f,0xf1,0x9c,0xd1,0x9e}}
#define EFI_DNS6_PROTOCOL_GUID                 {0xca37bc1f,0xa327,0x4ae9,{0x82,0x8a,0x8c,0x40,0xd8,0x50,0x6a,0x17}}
typedef struct efi_dns6_protocol efi_dns6_protocol;
typedef struct efi_dns6_config_data{
	bool             enable_cache;
	uint8_t          protocol;
	efi_ipv6_address station_ip;
	uint16_t         local_port;
	uint32_t         server_count;
	efi_ipv6_address *server_list;
	uint32_t         retry_count;
	uint32_t         retry_interval;
}efi_dns6_config_data;
typedef struct efi_dns6_cache_entry{
	char16           *hostname;
	efi_ipv6_address *address;
	uint32_t         timeout;
}efi_dns6_cache_entry;
typedef struct efi_dns6_mode_data{
	efi_dns6_config_data data;
	uint32_t             server_count;
	efi_ipv6_address     *server_list;
	uint32_t             cache_count;
	efi_dns6_cache_entry *cache_list;
}efi_dns6_mode_data;
typedef struct dns6_host_to_addr_data{
	uint32_t         ip_count;
	efi_ipv6_address *ip_list;
}dns6_host_to_addr_data;
typedef struct dns6_addr_to_host_data{
	char16 *hostname;
}dns6_addr_to_host_data;
typedef struct dns6_resource_record{
	char8    *q_name;
	uint16_t q_type;
	uint16_t q_class;
	uint32_t ttl;
	uint16_t data_len;
	char8    *rdata;
}dns6_resource_record;

typedef struct dns6_general_lookup_data{
	uintn_t              rr_count;
	dns6_resource_record *rr_list;
}dns6_general_lookup_data;
typedef struct efi_dns6_completion_token{
	efi_event  event;
	efi_status status;
	uint32_t   retry_count;
	uint32_t   retry_interval;
	union{
		dns6_host_to_addr_data   *h2a;
		dns6_addr_to_host_data   *a2h;
		dns6_general_lookup_data *g_lookup;
	}rsp;
}efi_dns6_completion_token;
typedef efi_status(efiapi*efi_dns6_get_mode_data)(
	efi_dns6_protocol  *this,
	efi_dns6_mode_data *data
);
typedef efi_status(efiapi*efi_dns6_configure)(
	efi_dns6_protocol    *this,
	efi_dns6_config_data *data
);
typedef efi_status(efiapi*efi_dns6_host_name_to_ip)(
	efi_dns6_protocol         *this,
	char16                    *hostname,
	efi_dns6_completion_token *token
);
typedef efi_status(efiapi*efi_dns6_ip_to_host_name)(
	efi_dns6_protocol         *this,
	efi_ipv6_address          address,
	efi_dns6_completion_token *token
);
typedef efi_status(efiapi*efi_dns6_general_lookup)(
	efi_dns6_protocol         *this,
	char8                     *q_name,
	uint16_t                  q_type,
	uint16_t                  q_class,
	efi_dns6_completion_token *token
);
typedef efi_status(efiapi*efi_dns6_update_dns_cache)(
	efi_dns6_protocol    *this,
	bool                 flag,
	bool                 override,
	efi_dns6_cache_entry entry
);
typedef efi_status(efiapi*efi_dns6_poll)(
	efi_dns6_protocol *this
);
typedef efi_status(efiapi*efi_dns6_cancel)(
	efi_dns6_protocol         *this,
	efi_dns6_completion_token *token
);
struct efi_dns6_protocol{
	efi_dns6_get_mode_data    get_mode_data;
	efi_dns6_configure        configure;
	efi_dns6_host_name_to_ip  hostname_to_ip;
	efi_dns6_ip_to_host_name  ip_to_hostname;
	efi_dns6_general_lookup   general_lookup;
	efi_dns6_update_dns_cache update_dns_cache;
	efi_dns6_poll             poll;
	efi_dns6_cancel           cancel;
};
extern const efi_guid gEfiDns6ServiceBindingProtocolGuid;
extern const efi_guid gEfiDns6ProtocolGuid;
#endif
