#ifndef EFI_WRAPPER_IP_V4_H
#define EFI_WRAPPER_IP_V4_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"defines.h"
#include"managed_network.h"
#define EFI_IP4_SERVICE_BINDING_PROTOCOL_GUID {0xc51711e7,0xb4bf,0x404a,{0xbf,0xb8,0x0a,0x04,0x8e,0xf1,0xff,0xe4}}
#define EFI_IP4_PROTOCOL_GUID                 {0x41d94cd2,0x35b6,0x455a,{0x82,0x58,0xd4,0xe5,0x13,0x34,0xaa,0xdd}}
typedef struct efi_ip4_protocol efi_ip4_protocol;
typedef struct efi_ip4_address_pair{
	efi_handle       instance;
	efi_ipv4_address address;
	efi_ipv4_address subnet_mask;
}efi_ip4_address_pair;
typedef struct efi_ip4_variable_data{
	efi_handle           driver;
	uint32_t             addr_count;
	efi_ip4_address_pair addr_pairs[0];
}efi_ip4_variable_data;
typedef struct efi_ip4_config_data{
	uint8_t default_proto;
	bool accept_any_proto;
	bool accept_icmp_errors;
	bool accept_broadcast;
	bool accept_promiscuous;
	bool use_default_addr;
	efi_ipv4_address station_addr;
	efi_ipv4_address subnet_mask;
	uint8_t tos;
	uint8_t ttl;
	bool do_not_fragment;
	bool raw_data;
	uint32_t receive_timeout;
	uint32_t transmit_timeout;
}efi_ip4_config_data;
typedef struct efi_ip4_route_table{
	efi_ipv4_address subnet_addr;
	efi_ipv4_address subnet_mask;
	efi_ipv4_address nexthop;
}efi_ip4_route_table;
typedef struct efi_ip4_icmp_type{
	uint8_t type;
	uint8_t code;
}efi_ip4_icmp_type;
typedef struct efi_ip4_mode_data{
	bool                is_started;
	uint32_t            max_pkt_size;
	efi_ip4_config_data config;
	bool                is_configured;
	uint32_t            group_count;
	efi_ipv4_address    *group_table;
	uint32_t            route_count;
	efi_ip4_route_table *route_table;
	uint32_t            icmp_type_count;
	efi_ip4_icmp_type   *icmp_type_list;
}efi_ip4_mode_data;
typedef struct efi_ip4_header{
	uint8_t          hdr_len:4;
	uint8_t          version:4;
	uint8_t          tos;
	uint16_t         total_len;
	uint16_t         identification;
	uint16_t         fragmentation;
	uint8_t          ttl;
	uint8_t          protocol;
	uint16_t         checksum;
	efi_ipv4_address src;
	efi_ipv4_address dst;
}cdecl_attr_packed efi_ip4_header;
typedef struct efi_ip4_fragment_data{
	uint32_t len;
	void*buffer;
}efi_ip4_fragment_data;
typedef struct efi_ip4_receive_data{
	efi_time               timestamp;
	efi_event              recycle_signal;
	uint32_t               hdr_len;
	efi_ip4_header         *hdr;
	uint32_t               opts_len;
	void                   *opts;
	uint32_t               data_len;
	uint32_t               frag_count;
	efi_ip4_fragment_data  frag_table[0];
}efi_ip4_receive_data;
typedef struct efi_ip4_override_data{
	efi_ipv4_address src;
	efi_ipv4_address gateway;
	uint8_t          proto;
	uint8_t          tos;
	uint8_t          ttl;
	bool             do_not_fragment;
}efi_ip4_override_data;
typedef struct efi_ip4_transmit_data{
	efi_ipv4_address       dst;
	efi_ip4_override_data  *override;
	uint32_t               opts_len;
	void                   *opts_buff;
	uint32_t               total_data_len;
	uint32_t               frag_count;
	efi_ip4_fragment_data  frag_table[0];
}efi_ip4_transmit_data;
typedef struct efi_ip4_completion_token{
	efi_event event;
	efi_status status;
	union{
		efi_ip4_receive_data  *rx;
		efi_ip4_transmit_data *tx;
	}packet;
}efi_ip4_completion_token;
typedef efi_status(efiapi*efi_ip4_get_mode_data)(
	const efi_ip4_protocol          *this,
        efi_ip4_mode_data               *mode,
        efi_managed_network_config_data *mnp,
        efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_ip4_configure)(
	efi_ip4_protocol    *this,
	efi_ip4_config_data *config
);
typedef efi_status(efiapi*efi_ip4_groups)(
	efi_ip4_protocol *this,
	bool             join_flag,
	efi_ipv4_address *group_addr
);
typedef efi_status(efiapi*efi_ip4_routes)(
	efi_ip4_protocol *this,
	bool             delete_route,
	efi_ipv4_address *subnet_addr,
	efi_ipv4_address *subnet_mask,
	efi_ipv4_address *gateway
);
typedef efi_status(efiapi*efi_ip4_transmit)(
	efi_ip4_protocol         *this,
	efi_ip4_completion_token *token
);
typedef efi_status(efiapi*efi_ip4_receive)(
	efi_ip4_protocol         *this,
	efi_ip4_completion_token *token
);
typedef efi_status(efiapi*efi_ip4_cancel)(
	efi_ip4_protocol         *this,
	efi_ip4_completion_token *token
);
typedef efi_status(efiapi*efi_ip4_poll)(
	efi_ip4_protocol *this
);
struct efi_ip4_protocol{
	efi_ip4_get_mode_data get_mode_data;
	efi_ip4_configure     configure;
	efi_ip4_groups        groups;
	efi_ip4_routes        routes;
	efi_ip4_transmit      transmit;
	efi_ip4_receive       receive;
	efi_ip4_cancel        cancel;
	efi_ip4_poll          poll;
};
extern const efi_guid gEfiIp4ServiceBindingProtocolGuid;
extern const efi_guid gEfiIp4ProtocolGuid;
#endif
