//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_DISK_INFO_H
#define EFI_WRAPPER_DISK_INFO_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_DISK_INFO_PROTOCOL_GUID         {0xd432a67f,0x14dc,0x484b,{0xb3,0xbb,0x3f,0x02,0x91,0x84,0x93,0x27}}
#define EFI_DISK_INFO_IDE_INTERFACE_GUID    {0x5e948fe3,0x26d3,0x42b5,{0xaf,0x17,0x61,0x02,0x87,0x18,0x8d,0xec}}
#define EFI_DISK_INFO_SCSI_INTERFACE_GUID   {0x08f74baa,0xea36,0x41d9,{0x95,0x21,0x21,0xa7,0x0f,0x87,0x80,0xbc}}
#define EFI_DISK_INFO_USB_INTERFACE_GUID    {0xcb871572,0xc11a,0x47b5,{0xb4,0x92,0x67,0x5e,0xaf,0xa7,0x77,0x27}}
#define EFI_DISK_INFO_AHCI_INTERFACE_GUID   {0x9e498932,0x4abc,0x45af,{0xa3,0x4d,0x02,0x47,0x78,0x7b,0xe7,0xc6}}
#define EFI_DISK_INFO_NVME_INTERFACE_GUID   {0x3ab14680,0x5d3f,0x4a4d,{0xbc,0xdc,0xcc,0x38,0x00,0x18,0xc7,0xf7}}
#define EFI_DISK_INFO_UFS_INTERFACE_GUID    {0x4b3029cc,0x6b98,0x47fb,{0xbc,0x96,0x76,0xdc,0xb8,0x04,0x41,0xf0}}
#define EFI_DISK_INFO_SD_MMC_INTERFACE_GUID {0x8deec992,0xd39c,0x4a5c,{0xab,0x6b,0x98,0x6e,0x14,0x24,0x2b,0x9d}}
typedef struct efi_disk_info_protocol efi_disk_info_protocol;
typedef efi_status(efiapi*efi_disk_info_inquiry)(
	efi_disk_info_protocol  *proto,
	void                    *data,
	uint32_t                *size
);
typedef efi_status(efiapi*efi_disk_info_identify)(
	efi_disk_info_protocol  *proto,
	void                    *data,
	uint32_t                *size
);
typedef efi_status(efiapi*efi_disk_info_sense_data)(
	efi_disk_info_protocol  *proto,
	void                    *data,
	uint32_t                *size,
	uint8_t                 *num
);
typedef efi_status(efiapi*efi_disk_info_which_ide)(
	efi_disk_info_protocol  *proto,
	uint32_t                *channel,
	uint32_t                *device
);
struct efi_disk_info_protocol{
	efi_guid                  interface;
	efi_disk_info_inquiry     inquiry;
	efi_disk_info_identify    identify;
	efi_disk_info_sense_data  sense_data;
	efi_disk_info_which_ide   which_ide;
};
extern const efi_guid gEfiDiskInfoProtocolGuid;
extern const efi_guid gEfiDiskInfoIdeInterfaceGuid;
extern const efi_guid gEfiDiskInfoScsiInterfaceGuid;
extern const efi_guid gEfiDiskInfoUsbInterfaceGuid;
extern const efi_guid gEfiDiskInfoAhciInterfaceGuid;
extern const efi_guid gEfiDiskInfoNvmeInterfaceGuid;
extern const efi_guid gEfiDiskInfoUfsInterfaceGuid;
extern const efi_guid gEfiDiskInfoSdMmcInterfaceGuid;
#endif //EFI_WRAPPER_DISK_INFO_H
