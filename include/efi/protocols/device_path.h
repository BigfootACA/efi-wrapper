//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_DEVICE_PATH_PROTOCOL_H
#define EFI_WRAPPER_DEVICE_PATH_PROTOCOL_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_DEVICE_PATH_PROTOCOL_GUID    {0x09576e91,0x6d3f,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_VIRTUAL_DISK_GUID            {0x77ab535a,0x45fc,0x624b,{0x55,0x60,0xf7,0xb2,0x81,0xd1,0xf9,0x6e}}
#define EFI_VIRTUAL_CD_GUID              {0x3d5abd30,0x4175,0x87ce,{0x6d,0x64,0xd2,0xad,0xe5,0x23,0xc4,0xbb}}
#define EFI_PERSISTENT_VIRTUAL_DISK_GUID {0x5cea02c9,0x4d07,0x69d3,{0x26,0x9f,0x44,0x96,0xfb,0xe0,0x96,0xf9}}
#define EFI_PERSISTENT_VIRTUAL_CD_GUID   {0x08018188,0x42cd,0xbb48,{0x10,0x0f,0x53,0x87,0xd5,0x3d,0xed,0x3d}}
#define DEVICE_PATH_TYPE_END                  0x7f
#define DEVICE_PATH_SUB_TYPE_INSTANCE_END     0x01
#define DEVICE_PATH_SUB_TYPE_END              0xff
#define DEVICE_PATH_TYPE_HARDWARE_DEVICE      0x01
#define DEVICE_PATH_SUB_TYPE_MEMORY           0x03
#define DEVICE_PATH_SUB_TYPE_VENDOR           0x04
#define DEVICE_PATH_SUB_TYPE_CONTROLLER       0x05
#pragma pack(1)
typedef struct efi_device_path_protocol{
	uint8_t type;
	uint8_t sub_type;
	uint16_t length;
}efi_device_path_protocol;
typedef efi_device_path_protocol efi_device_path;
#define HARDWARE_DEVICE_PATH      0x01
#define HW_MEMMAP_DP              0x03
typedef struct memmap_device_path{
	efi_device_path_protocol header;
	uint32_t                 type;
	efi_address              starting;
	efi_address              ending;
}memmap_device_path;
#define HW_VENDOR_DP              0x04
typedef struct vendor_device_path{
	efi_device_path_protocol header;
	efi_guid                 guid;
}vendor_device_path;
#define HW_CONTROLLER_DP          0x05
typedef struct controller_device_path{
	efi_device_path_protocol header;
	uint32_t                 controller;
}controller_device_path;
#define MSG_MAC_ADDR_DP           0x0b
typedef struct mac_addr_device_path{
	efi_device_path_protocol header;
	efi_mac_address          mac;
	uint8_t                  type;
}mac_addr_device_path;
#define MSG_IPv4_DP 0x0c
typedef struct ipv4_device_path{
	efi_device_path_protocol header;
	efi_ipv4_address         local_ip;
	efi_ipv4_address         remote_ip;
	uint16_t                 local_port;
	uint16_t                 remote_port;
	uint16_t                 protocol;
	bool                     static_ip;
	efi_ipv4_address         gateway_ip;
	efi_ipv4_address         subnet_mask;
}ipv4_device_path;
#define MSG_IPv6_DP               0x0d
typedef struct ipv6_device_path{
	efi_device_path_protocol header;
	efi_ipv6_address         local_ip;
	efi_ipv6_address         remote_ip;
	uint16_t                 local_port;
	uint16_t                 remote_port;
	uint16_t                 protocol;
	uint8_t                  ip_origin;
	uint8_t                  prefix;
	efi_ipv6_address         gateway_ip;
}ipv6_device_path;
#define MSG_URI_DP                0x18
typedef struct uri_device_path{
	efi_device_path_protocol header;
	char                     uri[0];
}uri_device_path;
#define MEDIA_DEVICE_PATH         0x04
#define MEDIA_HARDDRIVE_DP        0x01
typedef struct harddrive_device_path{
	efi_device_path_protocol header;
	uint32_t                 part_number;
	uint64_t                 part_start;
	uint64_t                 part_size;
	uint8_t                  signature[16];
	uint8_t                  mbr_type;
	uint8_t                  signature_type;
}harddrive_device_path;
#define MBR_TYPE_PCAT             0x01
#define MBR_TYPE_EFI_PARTITION_TABLE_HEADER 0x02
#define NO_DISK_SIGNATURE         0x00
#define SIGNATURE_TYPE_MBR        0x01
#define SIGNATURE_TYPE_GUID       0x02
#define MEDIA_CDROM_DP            0x02
#define MEDIA_VENDOR_DP           0x03
typedef struct cdrom_device_path{
	efi_device_path_protocol header;
	uint32_t                 boot_entry;
	uint64_t                 part_start;
	uint64_t                 part_size;
}cdrom_device_path;
#define MEDIA_VENDOR_DP           0x03
#define MEDIA_FILEPATH_DP         0x04
typedef struct filepath_device_path{
	efi_device_path_protocol header;
	char16                   path_name[0];
}filepath_device_path;
#define SIZE_OF_FILEPATH_DEVICE_PATH  OFFSET_OF(FILEPATH_DEVICE_PATH,PathName)
#define MEDIA_PROTOCOL_DP         0x05
typedef struct media_protocol_device_path{
	efi_device_path_protocol header;
	efi_guid                 protocol;
}media_protocol_device_path;
#define MEDIA_PIWG_FW_FILE_DP     0x06
typedef struct media_fw_vol_filepath_device_path{
	efi_device_path_protocol header;
	efi_guid                 fv_file_name;
}media_fw_vol_filepath_device_path;
#define MEDIA_PIWG_FW_VOL_DP      0x07
typedef struct media_fw_vol_device_path{
	efi_device_path_protocol header;
	efi_guid                 fv_name;
}media_fw_vol_device_path;
#define MEDIA_RELATIVE_OFFSET_RANGE_DP 0x08
typedef struct media_relative_offset_range_device_path{
	efi_device_path_protocol header;
	uint32_t                 reserved;
	uint64_t                 starting;
	uint64_t                 ending;
}media_relative_offset_range_device_path;
#define MEDIA_RAM_DISK_DP         0x09
typedef struct media_ram_disk_device_path{
	efi_device_path_protocol header;
	uint32_t                 start[2];
	uint32_t                 end[2];
	efi_guid                 type_guid;
	uint16_t                 instance;
}media_ram_disk_device_path;
#define BBS_DEVICE_PATH           0x05
#define BBS_BBS_DP                0x01
typedef struct bbs_bbs_device_path{
	efi_device_path_protocol header;
	uint16_t                 device_type;
	uint16_t                 status_flag;
	char                     string[0];
}bbs_bbs_device_path;
#define BBS_TYPE_FLOPPY           0x01
#define BBS_TYPE_HARDDRIVE        0x02
#define BBS_TYPE_CDROM            0x03
#define BBS_TYPE_PCMCIA           0x04
#define BBS_TYPE_USB              0x05
#define BBS_TYPE_EMBEDDED_NETWORK 0x06
#define BBS_TYPE_BEV              0x80
#define BBS_TYPE_UNKNOWN          0xFF
typedef union efi_dev_path{
	efi_device_path_protocol                devpath;
	memmap_device_path                      memmap;
	vendor_device_path                      vendor;
	mac_addr_device_path                    mac_addr;
	ipv4_device_path                        ipv4;
	ipv6_device_path                        ipv6;
	uri_device_path                         uri;
	harddrive_device_path                   hard_drive;
	cdrom_device_path                       cdrom;
	filepath_device_path                    filepath;
	media_protocol_device_path              media;
	media_fw_vol_device_path                firm_vol;
	media_fw_vol_filepath_device_path       firm_file;
	media_relative_offset_range_device_path offset;
	media_ram_disk_device_path              ramdisk;
	bbs_bbs_device_path                     bbs;
}efi_dev_path;
typedef union efi_dev_path_ptr{
	efi_device_path_protocol                *devpath;
	memmap_device_path                      *memmap;
	vendor_device_path                      *vendor;
	mac_addr_device_path                    *mac_addr;
	ipv4_device_path                        *ipv4;
	ipv6_device_path                        *ipv6;
	uri_device_path                         *uri;
	harddrive_device_path                   *hard_drive;
	cdrom_device_path                       *cdrom;
	filepath_device_path                    *filepath;
	media_protocol_device_path              *media;
	media_fw_vol_device_path                *firm_vol;
	media_fw_vol_filepath_device_path       *firm_file;
	media_relative_offset_range_device_path *offset;
	media_ram_disk_device_path              *ramdisk;
	bbs_bbs_device_path                     *bbs;
	uint8_t                                 *raw;
}efi_dev_path_ptr;
#pragma pack()
#define END_DEVICE_PATH_TYPE                 0x7f
#define END_ENTIRE_DEVICE_PATH_SUBTYPE       0xFF
#define END_INSTANCE_DEVICE_PATH_SUBTYPE     0x01
extern const efi_guid gEfiDevicePathProtocolGuid;
extern const efi_guid gEfiVirtualDiskGuid;
extern const efi_guid gEfiVirtualCdGuid;
extern const efi_guid gEfiPersistentVirtualDiskGuid;
extern const efi_guid gEfiPersistentVirtualCdGuid;
#endif
