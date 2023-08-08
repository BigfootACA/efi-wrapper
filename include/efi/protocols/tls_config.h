#ifndef EFI_WRAPPER_TLS_CONFIG_H
#define EFI_WRAPPER_TLS_CONFIG_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_TLS_CONFIGURATION_PROTOCOL_GUID {0x1682fe44,0xbd7a,0x4407,{0xb7,0xc7,0xdc,0xa3,0x7c,0xa3,0x92,0x2d}}
typedef struct efi_tls_config_protocol efi_tls_config_protocol;
typedef enum efi_tls_config_data_type{
	efi_tls_cfg_type_host_public_cert,
	efi_tls_cfg_type_host_private_key,
	efi_tls_cfg_type_ca_cert,
	efi_tls_cfg_type_cert_Revocation_list,
	efi_tls_cfg_type_max
}efi_tls_config_data_type;
typedef efi_status(efiapi*efi_tls_config_set_data)(
	efi_tls_config_protocol  *this,
	efi_tls_config_data_type type,
	void                     *data,
	uintn_t                  size
);
typedef efi_status(efiapi*efi_tls_config_get_data)(
	efi_tls_config_protocol  *this,
	efi_tls_config_data_type type,
	void                     *data,
	uintn_t                  *size
);
struct efi_tls_config_protocol{
	efi_tls_config_set_data set_data;
	efi_tls_config_get_data get_data;
};
extern const efi_guid gEfiTlsConfigurationProtocolGuid;
#endif
