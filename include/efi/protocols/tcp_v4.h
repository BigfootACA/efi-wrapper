#ifndef EFI_WRAPPER_TCP_V4_H
#define EFI_WRAPPER_TCP_V4_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"defines.h"
#include"ip_v4.h"
#define EFI_TCP4_SERVICE_BINDING_PROTOCOL_GUID {0x00720665,0x67EB,0x4a99,{0xBA,0xF7,0xD3,0xC3,0x3A,0x1C,0x7C,0xC9}}
#define EFI_TCP4_PROTOCOL_GUID                 {0x65530BC7,0xA359,0x410f,{0xB0,0x10,0x5A,0xAD,0xC7,0xEC,0x2B,0x62}}
typedef struct efi_tcp4_protocol efi_tcp4_protocol;
typedef struct efi_tcp4_service_point{
	efi_handle       instance;
	efi_ipv4_address local_addr;
	uint16_t         local_port;
	efi_ipv4_address remote_addr;
	uint16_t         remote_port;
}efi_tcp4_service_point;
typedef struct efi_tcp4_variable_data{
	efi_handle             driver;
	uint32_t               count;
	efi_tcp4_service_point services[0];
}efi_tcp4_variable_data;
typedef struct efi_tcp4_access_point{
	bool             use_default_addr;
	efi_ipv4_address station_addr;
	efi_ipv4_address subnet_mask;
	uint16_t         station_port;
	efi_ipv4_address remote_addr;
	uint16_t         remote_port;
	bool             active_flag;
}efi_tcp4_access_point;
typedef struct efi_tcp4_option{
	uint32_t receive_buf_size;
	uint32_t send_buf_size;
	uint32_t max_syn_backlog;
	uint32_t conn_timeout;
	uint32_t data_retries;
	uint32_t fin_timeout;
	uint32_t time_wait_timeout;
	uint32_t keep_alive_probes;
	uint32_t keep_alive_time;
	uint32_t keep_alive_interval;
	bool enable_nagle;
	bool enable_timestamp;
	bool enable_window_scaling;
	bool enable_selective_ack;
	bool enable_path_mtu_discovery;
}efi_tcp4_option;
typedef struct efi_tcp4_config_data{
	uint8_t tos;
	uint8_t ttl;
	efi_tcp4_access_point access_point;
	efi_tcp4_option *ctrl_opt;
}efi_tcp4_config_data;
typedef enum efi_tcp4_connection_state{
	tcp4_state_closed       = 0,
	tcp4_state_listen       = 1,
	tcp4_state_syn_sent     = 2,
	tcp4_state_syn_received = 3,
	tcp4_state_established  = 4,
	tcp4_state_fin_wait1    = 5,
	tcp4_state_fin_wait2    = 6,
	tcp4_state_closing      = 7,
	tcp4_state_time_wait    = 8,
	tcp4_state_close_wait   = 9,
	tcp4_state_last_ack     = 10
}efi_tcp4_connection_state;
typedef struct efi_tcp4_completion_token{
	efi_event event;
	efi_status status;
}efi_tcp4_completion_token;
typedef struct efi_tcp4_connection_token{
	efi_tcp4_completion_token token;
}efi_tcp4_connection_token;
typedef struct efi_tcp4_listen_token{
	efi_tcp4_completion_token token;
	efi_handle new_child;
}efi_tcp4_listen_token;
typedef struct efi_tcp4_fragment_data{
	uint32_t len;
	void*buf;
}efi_tcp4_fragment_data;
typedef struct efi_tcp4_receive_data{
	bool urgent_flag;
	uint32_t data_len;
	uint32_t frag_count;
	efi_tcp4_fragment_data frag_table[0];
}efi_tcp4_receive_data;
typedef struct efi_tcp4_transmit_data{
	bool push;
	bool urgent;
	uint32_t data_len;
	uint32_t frag_count;
	efi_tcp4_fragment_data frag_table[0];
}efi_tcp4_transmit_data;
typedef struct efi_tcp4_io_token{
	efi_tcp4_completion_token token;
	union{
		efi_tcp4_receive_data  *rx;
		efi_tcp4_transmit_data *tx;
	}pkt;
}efi_tcp4_io_token;
typedef struct efi_tcp4_close_token{
	efi_tcp4_completion_token token;
	bool abort_on_close;
}efi_tcp4_close_token;
typedef efi_status(efiapi*efi_tcp4_get_mode_data)(
	efi_tcp4_protocol               *this,
	efi_tcp4_connection_state       *state,
	efi_tcp4_config_data            *config,
	efi_ip4_mode_data               *ipv4_mode,
	efi_managed_network_config_data *mnp,
	efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_tcp4_configure)(
	efi_tcp4_protocol    *this,
	efi_tcp4_config_data *config
);

typedef efi_status(efiapi*efi_tcp4_routes)(
	efi_tcp4_protocol *this,
	bool              delete_route,
	efi_ipv4_address  *subnet_addr,
	efi_ipv4_address  *subnet_mask,
	efi_ipv4_address  *gateway
);
typedef efi_status(efiapi*efi_tcp4_connect)(
	efi_tcp4_protocol         *this,
	efi_tcp4_connection_token *token
);
typedef efi_status(efiapi*efi_tcp4_accept)(
	efi_tcp4_protocol     *this,
	efi_tcp4_listen_token *token
);
typedef efi_status(efiapi*efi_tcp4_transmit)(
	efi_tcp4_protocol *this,
	efi_tcp4_io_token *token
);
typedef efi_status(efiapi*efi_tcp4_receive)(
	efi_tcp4_protocol *this,
	efi_tcp4_io_token *token
);
typedef efi_status(efiapi*efi_tcp4_close)(
	efi_tcp4_protocol    *this,
	efi_tcp4_close_token *token
);
typedef efi_status(efiapi*efi_tcp4_cancel)(
	efi_tcp4_protocol         *this,
	efi_tcp4_completion_token *token
);
typedef efi_status(efiapi*efi_tcp4_poll)(
	efi_tcp4_protocol*this
);
struct efi_tcp4_protocol{
	efi_tcp4_get_mode_data get_mode_data;
	efi_tcp4_configure     configure;
	efi_tcp4_routes        routes;
	efi_tcp4_connect       connect;
	efi_tcp4_accept        accept;
	efi_tcp4_transmit      transmit;
	efi_tcp4_receive       receive;
	efi_tcp4_close         close;
	efi_tcp4_cancel        cancel;
	efi_tcp4_poll          poll;
};
extern const efi_guid gEfiTcp4ServiceBindingProtocolGuid;
extern const efi_guid gEfiTcp4ProtocolGuid;
#endif
