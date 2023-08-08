#ifndef EFI_WRAPPER_UDP_V6_H
#define EFI_WRAPPER_UDP_V6_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"defines.h"
#include"ip_v6.h"
#define EFI_UDP6_SERVICE_BINDING_PROTOCOL_GUID {0x66ed4721,0x3c98,0x4d3e,{0x81,0xe3,0xd0,0x3d,0xd3,0x9a,0x72,0x54}}
#define EFI_UDP6_PROTOCOL_GUID                 {0x4f948815,0xb4b9,0x43cb,{0x8a,0x33,0x90,0xe0,0x60,0xb3,0x49,0x55}}
typedef struct efi_udp6_protocol efi_udp6_protocol;
typedef struct efi_udp6_service_point{
	efi_handle       instance;
	efi_ipv6_address local_addr;
	uint16_t         local_port;
	efi_ipv6_address remote_addr;
	uint16_t         remote_port;
}efi_udp6_service_point;
typedef struct efi_udp6_variable_data{
	efi_handle driver;
	uint32_t service_count;
	efi_udp6_service_point services[0];
}efi_udp6_variable_data;
typedef struct efi_udp6_fragment_data{
	uint32_t len;
	void     *buf;
}efi_udp6_fragment_data;
typedef struct efi_udp6_session_data{
	efi_ipv6_address src_addr;
	uint16_t         src_port;
	efi_ipv6_address dst_addr;
	uint16_t         dst_port;
}efi_udp6_session_data;
typedef struct efi_udp6_config_data{
	bool accept_promiscuous;
	bool accept_any_port;
	bool allow_dup_port;
	uint8_t traffic_class;
	uint8_t hop_limit;
	uint32_t rx_timeout;
	uint32_t tx_timeout;
	efi_ipv6_address station_addr;
	uint16_t station_port;
	efi_ipv6_address remote_addr;
	uint16_t remote_port;
}efi_udp6_config_data;
typedef struct efi_udp6_transmit_data{
	efi_udp6_session_data  *session;
	uint32_t               data_len;
	uint32_t               frag_count;
	efi_udp6_fragment_data frag_table[0];
}efi_udp6_transmit_data;
typedef struct efi_udp6_receive_data{
	efi_time               timestamp;
	efi_event              recycle_signal;
	efi_udp6_session_data  session;
	uint32_t               data_len;
	uint32_t               frag_count;
	efi_udp6_fragment_data frag_table[0];
}efi_udp6_receive_data;
typedef struct efi_udp6_completion_token{
	efi_event  event;
	efi_status status;
	union{
		efi_udp6_receive_data  *rx;
		efi_udp6_transmit_data *tx;
	}pkt;
}efi_udp6_completion_token;
typedef efi_status(efiapi*efi_udp6_get_mode_data)(
	efi_udp6_protocol               *this,
	efi_udp6_config_data            *config,
	efi_ip6_mode_data               *mode,
	efi_managed_network_config_data *mnp,
	efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_udp6_configure)(
	efi_udp6_protocol    *this,
	efi_udp6_config_data *config
);
typedef efi_status(efiapi*efi_udp6_groups)(
	efi_udp6_protocol *this,
	bool              join_flag,
	efi_ipv6_address  *multicast
);
typedef efi_status(efiapi*efi_udp6_transmit)(
	efi_udp6_protocol         *this,
	efi_udp6_completion_token *token
);
typedef efi_status(efiapi*efi_udp6_receive)(
	efi_udp6_protocol         *this,
	efi_udp6_completion_token *token
);
typedef efi_status(efiapi*efi_udp6_cancel)(
	efi_udp6_protocol         *this,
	efi_udp6_completion_token *token
);
typedef efi_status(efiapi*efi_udp6_poll)(
	efi_udp6_protocol *this
);
struct efi_udp6_protocol{
	efi_udp6_get_mode_data get_mode_data;
	efi_udp6_configure     configure;
	efi_udp6_groups        groups;
	efi_udp6_transmit      transmit;
	efi_udp6_receive       receive;
	efi_udp6_cancel        cancel;
	efi_udp6_poll          poll;
};
extern const efi_guid gEfiUdp6ServiceBindingProtocolGuid;
extern const efi_guid gEfiUdp6ProtocolGuid;
#endif
