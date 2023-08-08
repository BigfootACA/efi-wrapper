#ifndef EFI_WRAPPER_UDP_V4_H
#define EFI_WRAPPER_UDP_V4_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"defines.h"
#include"ip_v4.h"
#define EFI_UDP4_SERVICE_BINDING_PROTOCOL_GUID {0x83f01464,0x99bd,0x45e5,{0xb3,0x83,0xaf,0x63,0x05,0xd8,0xe9,0xe6}}
#define EFI_UDP4_PROTOCOL_GUID                 {0x3ad9df29,0x4501,0x478d,{0xb1,0xf8,0x7f,0x7f,0xe7,0x0e,0x50,0xf3}}
typedef struct efi_udp4_protocol efi_udp4_protocol;
typedef struct efi_udp4_service_point{
	efi_handle       instance;
	efi_ipv4_address local_addr;
	uint16_t         local_port;
	efi_ipv4_address remote_addr;
	uint16_t         remote_port;
}efi_udp4_service_point;
typedef struct efi_udp4_variable_data{
	efi_handle driver;
	uint32_t service_count;
	efi_udp4_service_point services[0];
}efi_udp4_variable_data;
typedef struct efi_udp4_fragment_data{
	uint32_t len;
	void*buf;
}efi_udp4_fragment_data;
typedef struct efi_udp4_session_data{
	efi_ipv4_address src_addr;
	uint16_t         src_port;
	efi_ipv4_address dest_addr;
	uint16_t         dest_port;
}efi_udp4_session_data;
typedef struct efi_udp4_config_data{
	bool             accept_broadcast;
	bool             accept_promiscuous;
	bool             accept_any_port;
	bool             allow_dup_port;
	uint8_t          tos;
	uint8_t          ttl;
	bool             do_not_frag;
	uint32_t         rx_timeout;
	uint32_t         tx_timeout;
	bool             use_default_addr;
	efi_ipv4_address station_addr;
	efi_ipv4_address subnet_mask;
	uint16_t         station_port;
	efi_ipv4_address remote_addr;
	uint16_t         remote_port;
}efi_udp4_config_data;
typedef struct efi_udp4_transmit_data{
	efi_udp4_session_data  *udp_session;
	efi_ipv4_address       *gateway;
	uint32_t               data_len;
	uint32_t               frag_count;
	efi_udp4_fragment_data frag_table[0];
}efi_udp4_transmit_data;
typedef struct efi_udp4_receive_data{
	efi_time               timestamp;
	efi_event              recycle_signal;
	efi_udp4_session_data  udp_session;
	uint32_t               data_len;
	uint32_t               frag_count;
	efi_udp4_fragment_data frag_table[0];
}efi_udp4_receive_data;
typedef struct efi_udp4_completion_token{
	efi_event event;
	efi_status status;
	union{
		efi_udp4_receive_data  *rx;
		efi_udp4_transmit_data *tx;
	}pkt;
}efi_udp4_completion_token;
typedef efi_status(efiapi*efi_udp4_get_mode_data)(
	efi_udp4_protocol               *this,
	efi_udp4_config_data            *config,
	efi_ip4_mode_data               *mode,
	efi_managed_network_config_data *mnp,
	efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_udp4_configure)(
	efi_udp4_protocol    *this,
	efi_udp4_config_data *config
);
typedef efi_status(efiapi*efi_udp4_groups)(
	efi_udp4_protocol *this,
	bool              join_flag,
	efi_ipv4_address  *multicast
);
typedef efi_status(efiapi*efi_udp4_routes)(
	efi_udp4_protocol *this,
	bool              delete_route,
	efi_ipv4_address  *subnet_addr,
	efi_ipv4_address  *subnet_mask,
	efi_ipv4_address  *gateway_addr
);
typedef efi_status(efiapi*efi_udp4_poll)(
	efi_udp4_protocol *this
);
typedef efi_status(efiapi*efi_udp4_receive)(
	efi_udp4_protocol         *this,
	efi_udp4_completion_token *token
);
typedef efi_status(efiapi*efi_udp4_transmit)(
	efi_udp4_protocol         *this,
	efi_udp4_completion_token *token
);
typedef efi_status(efiapi*efi_udp4_cancel)(
	efi_udp4_protocol         *this,
	efi_udp4_completion_token *token
);
struct efi_udp4_protocol{
	efi_udp4_get_mode_data get_mode_data;
	efi_udp4_configure     configure;
	efi_udp4_groups        groups;
	efi_udp4_routes        routes;
	efi_udp4_transmit      transmit;
	efi_udp4_receive       receive;
	efi_udp4_cancel        cancel;
	efi_udp4_poll          poll;
};
extern const efi_guid gEfiUdp4ServiceBindingProtocolGuid;
extern const efi_guid gEfiUdp4ProtocolGuid;
#endif
