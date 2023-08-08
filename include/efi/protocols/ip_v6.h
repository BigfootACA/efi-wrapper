#ifndef EFI_WRAPPER_IP_V6_H
#define EFI_WRAPPER_IP_V6_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"defines.h"
#include"managed_network.h"
#define EFI_IP6_SERVICE_BINDING_PROTOCOL_GUID {0xec835dd3,0xfe0f,0x617b,{0xa6,0x21,0xb3,0x50,0xc3,0xe1,0x33,0x88}}
#define EFI_IP6_PROTOCOL_GUID                 {0x2c8759d5,0x5c2d,0x66ef,{0x92,0x5f,0xb6,0x6c,0x10,0x19,0x57,0xe2}}
typedef struct efi_ip6_protocol efi_ip6_protocol;
typedef struct efi_ip6_address_pair{
	efi_handle       instance;
	efi_ipv6_address address;
	uint8_t          prefix;
}efi_ip6_address_pair;
typedef struct efi_ip6_variable_data{
	efi_handle           driver;
	uint32_t             addr_count;
	efi_ip6_address_pair addr_pairs[0];
}efi_ip6_variable_data;
#define ICMP_V6_DEST_UNREACHABLE                 0x1
#define ICMP_V6_PACKET_TOO_BIG                   0x2
#define ICMP_V6_TIME_EXCEEDED                    0x3
#define ICMP_V6_PARAMETER_PROBLEM                0x4
#define ICMP_V6_ECHO_REQUEST                     0x80
#define ICMP_V6_ECHO_REPLY                       0x81
#define ICMP_V6_LISTENER_QUERY                   0x82
#define ICMP_V6_LISTENER_REPORT                  0x83
#define ICMP_V6_LISTENER_DONE                    0x84
#define ICMP_V6_ROUTER_SOLICIT                   0x85
#define ICMP_V6_ROUTER_ADVERTISE                 0x86
#define ICMP_V6_NEIGHBOR_SOLICIT                 0x87
#define ICMP_V6_NEIGHBOR_ADVERTISE               0x88
#define ICMP_V6_REDIRECT                         0x89
#define ICMP_V6_LISTENER_REPORT_2                0x8F
#define ICMP_V6_NO_ROUTE_TO_DEST                 0x0
#define ICMP_V6_COMM_PROHIBITED                  0x1
#define ICMP_V6_BEYOND_SCOPE                     0x2
#define ICMP_V6_ADDR_UNREACHABLE                 0x3
#define ICMP_V6_PORT_UNREACHABLE                 0x4
#define ICMP_V6_SOURCE_ADDR_FAILED               0x5
#define ICMP_V6_ROUTE_REJECTED                   0x6
#define ICMP_V6_TIMEOUT_HOP_LIMIT                0x0
#define ICMP_V6_TIMEOUT_REASSEMBLE               0x1
#define ICMP_V6_ERRONEOUS_HEADER                 0x0
#define ICMP_V6_UNRECOGNIZE_NEXT_HDR             0x1
#define ICMP_V6_UNRECOGNIZE_OPTION               0x2
typedef struct efi_ip6_config_data{
	uint8_t default_proto;
	bool accept_any_proto;
	bool accept_icmp_errors;
	bool accept_promiscuous;
	efi_ipv6_address dest_addr;
	efi_ipv6_address station_addr;
	uint8_t traffic_class;
	uint8_t hop_limit;
	uint32_t flow_label;
	uint32_t receive_timeout;
	uint32_t transmit_timeout;
}efi_ip6_config_data;
typedef struct efi_ip6_address_info{
	efi_ipv6_address addr;
	uint8_t          prefix;
}efi_ip6_address_info;
typedef struct efi_ip6_route_table{
	efi_ipv6_address gateway;
	efi_ipv6_address dest;
	uint8_t          prefix;
}efi_ip6_route_table;
typedef enum efi_ip6_neighbor_state{
	efi_ip6_neigh_incomplete,
	efi_ip6_neigh_reachable,
	efi_ip6_neigh_stale,
	efi_ip6_neigh_delay,
	efi_ip6_neigh_probe
}efi_ip6_neighbor_state;
typedef struct efi_ip6_neighbor_cache{
	efi_ipv6_address        neighbor;
	efi_mac_address         link_addr;
	efi_ip6_neighbor_state  state;
}efi_ip6_neighbor_cache;
typedef struct efi_ip6_icmp_type{
	uint8_t type;
	uint8_t code;
}efi_ip6_icmp_type;
typedef struct efi_ip6_mode_data{
	bool                   is_started;
	uint32_t               max_pkt_size;
	efi_ip6_config_data    config;
	bool                   is_configured;
	uint32_t               addr_count;
	efi_ip6_address_info   *addr_list;
	uint32_t               group_count;
	efi_ipv6_address       *group_table;
	uint32_t               route_count;
	efi_ip6_route_table    *route_table;
	uint32_t               neighbor_count;
	efi_ip6_neighbor_cache *neighbor_cache;
	uint32_t               prefix_count;
	efi_ip6_address_info   *prefix_table;
	uint32_t               icmp_type_count;
	efi_ip6_icmp_type      *icmp_type_list;
}efi_ip6_mode_data;
typedef struct efi_ip6_header{
	uint8_t          traffic_class_h:4;
	uint8_t          version:4;
	uint8_t          flow_label_h:4;
	uint8_t          traffic_class_l:4;
	uint16_t         flow_label_l;
	uint16_t         payload_len;
	uint8_t          next_hdr;
	uint8_t          hop_limit;
	efi_ipv6_address src_addr;
	efi_ipv6_address dst_addr;
}cdecl_attr_packed efi_ip6_header;
typedef struct efi_ip6_fragment_data{
	uint32_t len;
	void     *buf;
}efi_ip6_fragment_data;
typedef struct efi_ip6_receive_data{
	efi_time              timestamp;
	efi_event             recycle_signal;
	uint32_t              hdr_len;
	efi_ip6_header        *hdr;
	uint32_t              data_len;
	uint32_t              frag_count;
	efi_ip6_fragment_data frag_table[0];
}efi_ip6_receive_data;
typedef struct efi_ip6_override_data{
	uint8_t  proto;
	uint8_t  hop_limit;
	uint32_t flow_label;
}efi_ip6_override_data;
typedef struct efi_ip6_transmit_data{
	efi_ipv6_address      dest_addr;
	efi_ip6_override_data *override_data;
	uint32_t              ext_hdrs_len;
	void                  *ext_hdrs;
	uint8_t               next_hdr;
	uint32_t              data_len;
	uint32_t              frag_count;
	efi_ip6_fragment_data frag_table[0];
}efi_ip6_transmit_data;
typedef struct efi_ip6_completion_token{
	efi_event  event;
	efi_status status;
	union{
		efi_ip6_receive_data  *rx;
		efi_ip6_transmit_data *tx;
	}packet;
}efi_ip6_completion_token;
typedef efi_status(efiapi*efi_ip6_get_mode_data)(
	efi_ip6_protocol                *this,
	efi_ip6_mode_data               *mode,
	efi_managed_network_config_data *mnp,
	efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_ip6_configure)(
	efi_ip6_protocol    *this,
	efi_ip6_config_data *config
);
typedef efi_status(efiapi*efi_ip6_groups)(
	efi_ip6_protocol *this,
	bool             join_flag,
	efi_ipv6_address *group_addr
);
typedef efi_status(efiapi*efi_ip6_routes)(
	efi_ip6_protocol *this,
	bool             delete_route,
	efi_ipv6_address *dst,
	uint8_t          prefix,
	efi_ipv6_address *gateway
);
typedef efi_status(efiapi*efi_ip6_neighbors)(
	efi_ip6_protocol *this,
	bool             delete_flag,
	efi_ipv6_address *target_ip6_addr,
	efi_mac_address  *target_link_addr,
	uint32_t         timeout,
	bool             override
);
typedef efi_status(efiapi*efi_ip6_transmit)(
	efi_ip6_protocol         *this,
	efi_ip6_completion_token *token
);
typedef efi_status(efiapi*efi_ip6_receive)(
	efi_ip6_protocol         *this,
	efi_ip6_completion_token *token
);
typedef efi_status(efiapi*efi_ip6_cancel)(
	efi_ip6_protocol         *this,
	efi_ip6_completion_token *token
);
typedef efi_status(efiapi*efi_ip6_poll)(
	efi_ip6_protocol *this
);
struct efi_ip6_protocol{
	efi_ip6_get_mode_data   get_mode_data;
	efi_ip6_configure       configure;
	efi_ip6_groups          groups;
	efi_ip6_routes          routes;
	efi_ip6_neighbors       neighbors;
	efi_ip6_transmit        transmit;
	efi_ip6_receive         receive;
	efi_ip6_cancel          cancel;
	efi_ip6_poll            poll;
};
extern const efi_guid gEfiIp6ServiceBindingProtocolGuid;
extern const efi_guid gEfiIp6ProtocolGuid;
#endif
