//
// Created by bigfoot on 2023/5/12.
//

#ifndef EFI_WRAPPER_HII_CONFIG_ROUTING_H
#define EFI_WRAPPER_HII_CONFIG_ROUTING_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"../efi_hii.h"
#include"device_path.h"
#define EFI_HII_CONFIG_ROUTING_PROTOCOL_GUID {0x587e72d7,0xcc50,0x4f79,{0x82,0x09,0xca,0x29,0x1f,0xc1,0xa1,0x0f}}
typedef struct efi_hii_config_routing_protocol efi_hii_config_routing_protocol;
typedef efi_status(efiapi*efi_hii_extract_config)(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      request,
	efi_string                            *progress,
	efi_string                            *results
);
typedef efi_status(efiapi*efi_hii_export_config)(
	const efi_hii_config_routing_protocol *proto,
	efi_string                            *results
);
typedef efi_status(efiapi*efi_hii_route_config)(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      cfg,
	efi_string                            *progress
);
typedef efi_status(efiapi*efi_hii_block_to_config)(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      request,
	const uint8_t                         *block,
	const uintn_t                         size,
	efi_string                            *config,
	efi_string                            *progress
);
typedef efi_status(efiapi*efi_hii_config_to_block)(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      resp,
	uint8_t                               *block,
	uintn_t                               *size,
	efi_string                            *progress
);
typedef efi_status(efiapi*efi_hii_get_alt_cfg)(
	const efi_hii_config_routing_protocol *proto,
	const efi_string                      resp,
	const efi_guid                        *guid,
	const efi_string                      name,
	const efi_device_path_protocol        *device_path,
	const uint16_t                        *alt_cfg_id,
	efi_string                            *alt_cfg_resp
);
struct efi_hii_config_routing_protocol{
	efi_hii_extract_config  extract_config;
	efi_hii_export_config   export_config;
	efi_hii_route_config    route_config;
	efi_hii_block_to_config block_to_config;
	efi_hii_config_to_block config_to_block;
	efi_hii_get_alt_cfg     get_alt_config;
};
extern const efi_guid gEfiHiiConfigRoutingProtocolGuid;
#endif //EFI_WRAPPER_HII_CONFIG_ROUTING_H
