//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_TIMESTAMP_H
#define EFI_WRAPPER_TIMESTAMP_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_TIMESTAMP_PROTOCOL_GUID {0xafbfde41,0x2e6e,0x4262,{0xba,0x65,0x62,0xb9,0x23,0x6e,0x54,0x95}}
typedef struct efi_timestamp_protocol efi_timestamp_protocol;
typedef struct efi_timestamp_properties{
	uint64_t frequency;
	uint64_t end_value;
}efi_timestamp_properties;
typedef uint64_t(efiapi*timestamp_get)(void);
typedef efi_status(efiapi*timestamp_get_properties)(efi_timestamp_properties*prop);
struct efi_timestamp_protocol{
	timestamp_get            get_timestamp;
	timestamp_get_properties get_properties;
};
extern const efi_guid gEfiTimestampProtocolGuid;
#endif //EFI_WRAPPER_TIMESTAMP_H
