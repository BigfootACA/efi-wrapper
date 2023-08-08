//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_MANAGED_NETWORK_H
#define EFI_WRAPPER_MANAGED_NETWORK_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"simple_network.h"
#define EFI_MANAGED_NETWORK_SERVICE_BINDING_PROTOCOL_GUID {0xf36ff770,0xa7e1,0x42cf,{0x9e,0xd2,0x56,0xf0,0xf2,0x71,0xf4,0x4c}}
#define EFI_MANAGED_NETWORK_PROTOCOL_GUID                 {0x7ab33a91,0xace5,0x4326,{0xb5,0x72,0xe7,0xee,0x33,0xd3,0x9f,0x16}}
typedef struct efi_managed_network_protocol efi_managed_network_protocol;
typedef struct efi_managed_network_config_data{
	uint32_t received_queue_timeout;
	uint32_t transmit_queue_timeout;
	uint16_t protocol_type_filter;
	bool enable_unicast_receive;
	bool enable_multicast_receive;
	bool enable_broadcast_receive;
	bool enable_promiscuous_receive;
	bool flush_queue_son_reset;
	bool enable_receive_timestamps;
	bool disable_background_polling;
}efi_managed_network_config_data;
typedef struct efi_managed_network_receive_data{
	efi_time timestamp;
	efi_event recycle_event;
	uint32_t pkt_len;
	uint32_t hdr_len;
	uint32_t addr_len;
	uint32_t data_len;
	bool broadcast_flag;
	bool multicast_flag;
	bool promiscuous_flag;
	uint16_t proto_type;
	void*dest_addr;
	void*src_addr;
	void*media_hdr;
	void*pkt_data;
}efi_managed_network_receive_data;
typedef struct efi_managed_network_fragment_data{
	uint32_t frag_len;
	void*frag_buffer;
}efi_managed_network_fragment_data;
typedef struct{
	efi_mac_address*dest_addr;
	efi_mac_address*src_addr;
	uint16_t proto_type;
	uint32_t data_len;
	uint16_t hdr_len;
	uint16_t frag_count;
	efi_managed_network_fragment_data frag_table[0];
}efi_managed_network_transmit_data;
typedef struct efi_managed_network_completion_token{
	efi_event  event;
	efi_status status;
	union{
		efi_managed_network_receive_data  *rx;
		efi_managed_network_transmit_data *tx;
	}packet;
}efi_managed_network_completion_token;
typedef efi_status(efiapi*efi_managed_network_get_mode_data)(
	efi_managed_network_protocol    *this,
	efi_managed_network_config_data *mnp,
	efi_simple_network_mode         *snp
);
typedef efi_status(efiapi*efi_managed_network_configure)(
	efi_managed_network_protocol    *this,
	efi_managed_network_config_data *mnp
);
typedef efi_status(efiapi*efi_managed_network_mcast_ip_to_mac)(
	efi_managed_network_protocol *this,
	bool                         ipv6_flag,
	efi_ip_address               *ip,
	efi_mac_address              *mac
);
typedef efi_status(efiapi*efi_managed_network_groups)(
	efi_managed_network_protocol *this,
	bool                         join_flag,
	efi_mac_address              *mac
);

typedef efi_status(efiapi*efi_managed_network_transmit)(
	efi_managed_network_protocol         *this,
	efi_managed_network_completion_token *token
);
typedef efi_status(efiapi*efi_managed_network_receive)(
	efi_managed_network_protocol         *this,
	efi_managed_network_completion_token *token
);
typedef efi_status(efiapi*efi_managed_network_cancel)(
	efi_managed_network_protocol         *this,
	efi_managed_network_completion_token *token
);
typedef efi_status(efiapi*efi_managed_network_poll)(
	efi_managed_network_protocol*this
);
struct efi_managed_network_protocol {
	efi_managed_network_get_mode_data   get_mode_data;
	efi_managed_network_configure       configure;
	efi_managed_network_mcast_ip_to_mac mcast_ip_to_mac;
	efi_managed_network_groups          groups;
	efi_managed_network_transmit        transmit;
	efi_managed_network_receive         receive;
	efi_managed_network_cancel          cancel;
	efi_managed_network_poll            poll;
};
extern const efi_guid gEfiManagedNetworkServiceBindingProtocolGuid;
extern const efi_guid gEfiManagedNetworkProtocolGuid;
#endif
