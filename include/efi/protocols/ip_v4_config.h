#ifndef EFI_WRAPPER_IP_V4_CONFIG_H
#define EFI_WRAPPER_IP_V4_CONFIG_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"ip_v4.h"
#define EFI_IP4_CONFIG_PROTOCOL_GUID {0x3b95aa31,0x3793,0x434b,{0x86,0x67,0xc8,0x07,0x08,0x92,0xe0,0x5e}}
typedef struct efi_ip4_config_protocol efi_ip4_config_protocol;
#define IP4_CONFIG_VARIABLE_ATTRIBUTES (EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_BOOTSERVICE_ACCESS)
typedef struct efi_ip4_ipconfig_data{
	efi_ipv4_address    station_addr;
	efi_ipv4_address    subnet_mask;
	uint32_t            route_table_size;
	efi_ip4_route_table *route_table;
}efi_ip4_ipconfig_data;
typedef efi_status(efiapi*efi_ip4_config_start)(
	efi_ip4_config_protocol *this,
	efi_event               done,
	efi_event               re_config
);
typedef efi_status(efiapi*efi_ip4_config_stop)(
	efi_ip4_config_protocol *this
);
typedef efi_status(efiapi*efi_ip4_config_get_data)(
	efi_ip4_config_protocol *this,
	uintn_t                 *size,
	efi_ip4_ipconfig_data   *data
);
struct efi_ip4_config_protocol{
	efi_ip4_config_start    start;
	efi_ip4_config_stop     stop;
	efi_ip4_config_get_data get_data;
};
extern const efi_guid gEfiIp4ConfigProtocolGuid;
#endif
