#ifndef EFI_WRAPPER_TCP_V6_H
#define EFI_WRAPPER_TCP_V6_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"defines.h"
#include"ip_v6.h"
#define EFI_TCP6_SERVICE_BINDING_PROTOCOL_GUID {0xec20eb79,0x6c1a,0x4664,{0x9a,0x0d,0xd2,0xe4,0xcc,0x16,0xd6,0x64}}
#define EFI_TCP6_PROTOCOL_GUID                 {0x46e44855,0xbd60,0x4ab7,{0xab,0x0d,0xa6,0x79,0xb9,0x44,0x7d,0x77}}
typedef struct efi_tcp6_protocol efi_tcp6_protocol;
typedef struct efi_tcp6_service_point{
	efi_handle       instance;
	efi_ipv6_address local_addr;
	uint16_t         local_port;
	efi_ipv6_address remote_addr;
	uint16_t         remote_port;
}efi_tcp6_service_point;
typedef struct efi_tcp6_variable_data{
	efi_handle driver;
	uint32_t service_count;
	efi_tcp6_service_point services[0];
}efi_tcp6_variable_data;
typedef struct efi_tcp6_access_point{
	efi_ipv6_address station_addr;
	uint16_t         station_port;
	efi_ipv6_address remote_addr;
	uint16_t         remote_port;
	bool             active_flag;
}efi_tcp6_access_point;
typedef struct efi_tcp6_option{
	uint32_t receive_buf_size;
	uint32_t send_buf_size;
	uint32_t max_syn_back_log;
	uint32_t conn_timeout;
	uint32_t data_retries;
	uint32_t fin_timeout;
	uint32_t time_wait_timeout;
	uint32_t keep_alive_probes;
	uint32_t keep_alive_time;
	uint32_t keep_alive_interval;
	bool enable_nagle;
	bool enable_time_stamp;
	bool enable_window_scaling;
	bool enable_selective_ack;
	bool enable_path_mtu_discovery;
}efi_tcp6_option;
typedef struct efi_tcp6_config_data{
	uint8_t               traffic_class;
	uint8_t               hop_limit;
	efi_tcp6_access_point access_point;
	efi_tcp6_option       *control_option;
}efi_tcp6_config_data;
typedef enum efi_tcp6_connection_state{
	tcp6_state_closed       = 0,
	tcp6_state_listen       = 1,
	tcp6_state_syn_sent     = 2,
	tcp6_state_syn_received = 3,
	tcp6_state_established  = 4,
	tcp6_state_fin_wait1    = 5,
	tcp6_state_fin_wait2    = 6,
	tcp6_state_closing      = 7,
	tcp6_state_time_wait    = 8,
	tcp6_state_close_wait   = 9,
	tcp6_state_last_ack     = 10
}efi_tcp6_connection_state;
typedef struct efi_tcp6_completion_token{
	efi_event   event;
	efi_status  status;
}efi_tcp6_completion_token;
typedef struct efi_tcp6_connection_token{
	efi_tcp6_completion_token token;
}efi_tcp6_connection_token;
typedef struct efi_tcp6_listen_token{
	efi_tcp6_completion_token token;
	efi_handle new_child;
}efi_tcp6_listen_token;
typedef struct efi_tcp6_fragment_data{
	uint32_t len;
	void *buf;
}efi_tcp6_fragment_data;
typedef struct efi_tcp6_receive_data{
	bool urgent_flag;
	uint32_t data_len;
	uint32_t frag_count;
	efi_tcp6_fragment_data frag_table[0];
}efi_tcp6_receive_data;
typedef struct efi_tcp6_transmit_data{
	bool push;
	bool urgent;
	uint32_t data_len;
	uint32_t frag_count;
	efi_tcp6_fragment_data frag_table[0];
}efi_tcp6_transmit_data;
typedef struct efi_tcp6_io_token{
	efi_tcp6_completion_token token;
	union{
		efi_tcp6_receive_data  *rx;
		efi_tcp6_transmit_data *tx;
	}pkt;
}efi_tcp6_io_token;
typedef struct efi_tcp6_close_token{
	efi_tcp6_completion_token token;
	bool abort_on_close;
}efi_tcp6_close_token;
typedef efi_status(efiapi*efi_tcp6_get_mode_data)(
	efi_tcp6_protocol               *this,
	efi_tcp6_connection_state       *state,
	efi_tcp6_config_data            *config,
	efi_ip6_mode_data               *mode,
	efi_managed_network_config_data *mnp,
	efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_tcp6_configure)(
	efi_tcp6_protocol    *this,
	efi_tcp6_config_data *config
);
typedef efi_status(efiapi*efi_tcp6_connect)(
	efi_tcp6_protocol         *this,
	efi_tcp6_connection_token *token
);
typedef efi_status(efiapi*efi_tcp6_accept)(
	efi_tcp6_protocol     *this,
	efi_tcp6_listen_token *token
);
typedef efi_status(efiapi*efi_tcp6_transmit)(
	efi_tcp6_protocol *this,
	efi_tcp6_io_token *token
);
typedef efi_status(efiapi*efi_tcp6_receive)(
	efi_tcp6_protocol *this,
	efi_tcp6_io_token *token
);
typedef efi_status(efiapi*efi_tcp6_close)(
	efi_tcp6_protocol    *this,
	efi_tcp6_close_token *token
);
typedef efi_status(efiapi*efi_tcp6_cancel)(
	efi_tcp6_protocol         *this,
	efi_tcp6_completion_token *token
);
typedef efi_status(efiapi*efi_tcp6_poll)(
	efi_tcp6_protocol *this
);
struct efi_tcp6_protocol{
	efi_tcp6_get_mode_data get_mode_data;
	efi_tcp6_configure     configure;
	efi_tcp6_connect       connect;
	efi_tcp6_accept        accept;
	efi_tcp6_transmit      transmit;
	efi_tcp6_receive       receive;
	efi_tcp6_close         close;
	efi_tcp6_cancel        cancel;
	efi_tcp6_poll          poll;
};
extern const efi_guid gEfiTcp6ServiceBindingProtocolGuid;
extern const efi_guid gEfiTcp6ProtocolGuid;
#endif
