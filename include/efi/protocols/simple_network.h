//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_SIMPLE_NETWORK_H
#define EFI_WRAPPER_SIMPLE_NETWORK_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_SIMPLE_NETWORK_PROTOCOL_GUID {0xa19832b9,0xac25,0x11d3,{0x9a,0x2d,0x00,0x90,0x27,0x3f,0xc1,0x4d}}
typedef struct efi_simple_network_protocol efi_simple_network_protocol;
typedef efi_simple_network_protocol efi_simple_network;
typedef struct efi_network_statistics{
	uint64_t rx_total_frames;
	uint64_t rx_good_frames;
	uint64_t rx_undersize_frames;
	uint64_t rx_oversize_frames;
	uint64_t rx_dropped_frames;
	uint64_t rx_unicast_frames;
	uint64_t rx_broadcast_frames;
	uint64_t rx_multicast_frames;
	uint64_t rx_crcError_frames;
	uint64_t rx_total_bytes;
	uint64_t tx_total_frames;
	uint64_t tx_good_frames;
	uint64_t tx_undersize_frames;
	uint64_t tx_oversize_frames;
	uint64_t tx_dropped_frames;
	uint64_t tx_unicast_frames;
	uint64_t tx_broadcast_frames;
	uint64_t tx_multicast_frames;
	uint64_t tx_crc_error_frames;
	uint64_t tx_total_bytes;
	uint64_t collisions;
	uint64_t unsupported_protocol;
	uint64_t rx_duplicated_frames;
	uint64_t rx_decrypt_error_frames;
	uint64_t tx_error_frames;
	uint64_t tx_retry_frames;
}efi_network_statistics;
typedef enum efi_simple_network_state{
	efi_simple_network_stopped,
	efi_simple_network_started,
	efi_simple_network_initialized,
	efi_simple_network_max_state
}efi_simple_network_state;
#define EFI_SIMPLE_NETWORK_RECEIVE_UNICAST                0x01
#define EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST              0x02
#define EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST              0x04
#define EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS            0x08
#define EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST  0x10
#define EFI_SIMPLE_NETWORK_RECEIVE_INTERRUPT              0x01
#define EFI_SIMPLE_NETWORK_TRANSMIT_INTERRUPT             0x02
#define EFI_SIMPLE_NETWORK_COMMAND_INTERRUPT              0x04
#define EFI_SIMPLE_NETWORK_SOFTWARE_INTERRUPT             0x08
#define MAX_MCAST_FILTER_CNT                              16
typedef struct efi_simple_network_mode{
	uint32_t        state;
	uint32_t        hw_addr_size;
	uint32_t        media_header_size;
	uint32_t        max_packet_size;
	uint32_t        nv_ram_size;
	uint32_t        nv_ram_access_size;
	uint32_t        receive_filter_mask;
	uint32_t        receive_filter_setting;
	uint32_t        max_mcast_filter_count;
	uint32_t        mcast_filter_count;
	efi_mac_address mcast_filter[MAX_MCAST_FILTER_CNT];
	efi_mac_address cur_addr;
	efi_mac_address broadcast_addr;
	efi_mac_address perm_addr;
	uint8_t         if_type;
	bool            mac_addr_changeable;
	bool            multi_tx_supported;
	bool            media_present_supported;
	bool            media_present;
}efi_simple_network_mode;
typedef efi_status(efiapi*efi_simple_network_start)(
	efi_simple_network_protocol*proto
);
typedef efi_status(efiapi*efi_simple_network_stop)(
	efi_simple_network_protocol*proto
);
typedef efi_status(efiapi*efi_simple_network_initialize)(
	efi_simple_network_protocol*proto,
	uintn_t extra_rx_buf_size,
	uintn_t extra_tx_buf_size
);
typedef efi_status(efiapi*efi_simple_network_reset)(
	efi_simple_network_protocol*proto,
	bool ext_verify
);
typedef efi_status(efiapi*efi_simple_network_shutdown)(
	efi_simple_network_protocol*proto
);
typedef efi_status(efiapi*efi_simple_network_receive_filters)(
	efi_simple_network_protocol *proto,
	uint32_t                    enable,
	uint32_t                    disable,
	bool                        reset_mcast_filter,
	uintn_t                     mcast_filter_cnt,
	efi_mac_address             *mcast_filter
);
typedef efi_status(efiapi*efi_simple_network_station_address)(
	efi_simple_network_protocol *proto,
	bool                        reset,
	efi_mac_address             *new
);
typedef efi_status(efiapi*efi_simple_network_statistics)(
	efi_simple_network_protocol *proto,
	bool                        reset,
	uintn_t                     *size,
	efi_network_statistics      *table
);
typedef efi_status(efiapi*efi_simple_network_mcast_ip_to_mac)(
	efi_simple_network_protocol *proto,
	bool                        ipv6,
	efi_ip_address              *ip,
	efi_mac_address             *mac
);
typedef efi_status(efiapi*efi_simple_network_nvdata)(
	efi_simple_network_protocol *proto,
	bool                        rw,
	uintn_t                     offset,
	uintn_t                     size,
	void                        *buf
);
typedef efi_status(efiapi*efi_simple_network_get_status)(
	efi_simple_network_protocol *proto,
	uint32_t                    *int_status,
	void                        **tx_buf
);
typedef efi_status(efiapi*efi_simple_network_transmit)(
	efi_simple_network_protocol *proto,
	uintn_t                     hdr_size,
	uintn_t                     buf_size,
	void                        *buf,
	efi_mac_address             *src_addr,
	efi_mac_address             *dest_addr,
	uint16_t                    *protocol
);
typedef efi_status(efiapi*efi_simple_network_receive)(
	efi_simple_network_protocol *proto,
	uintn_t                     *hdr_size,
	uintn_t                     *buf_size,
	void                        *buf,
	efi_mac_address             *src_addr,
	efi_mac_address             *dest_addr,
	uint16_t                    *protocol
);
#define EFI_SIMPLE_NETWORK_PROTOCOL_REVISION 0x00010000
#define EFI_SIMPLE_NETWORK_INTERFACE_REVISION EFI_SIMPLE_NETWORK_PROTOCOL_REVISION
struct efi_simple_network_protocol{
	uint64_t                            revision;
	efi_simple_network_start            start;
	efi_simple_network_stop             stop;
	efi_simple_network_initialize       initialize;
	efi_simple_network_reset            reset;
	efi_simple_network_shutdown         shutdown;
	efi_simple_network_receive_filters  receive_filters;
	efi_simple_network_station_address  station_address;
	efi_simple_network_statistics       statistics;
	efi_simple_network_mcast_ip_to_mac  mcast_ip_to_mac;
	efi_simple_network_nvdata           nvdata;
	efi_simple_network_get_status       getstatus;
	efi_simple_network_transmit         transmit;
	efi_simple_network_receive          receive;
	efi_event                           wait_for_packet;
	efi_simple_network_mode             *mode;
};
extern const efi_guid gEfiSimpleNetworkProtocolGuid;
#endif //EFI_WRAPPER_SIMPLE_NETWORK_H
