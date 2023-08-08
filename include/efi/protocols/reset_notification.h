#ifndef EFI_WRAPPER_RESET_NOTIFICATION_H
#define EFI_WRAPPER_RESET_NOTIFICATION_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"../runtime_services.h"
#define EFI_RESET_NOTIFICATION_PROTOCOL_GUID {0x9da34ae0,0xeaf9,0x4bbf,{0x8e,0xc3,0xfd,0x60,0x22,0x6c,0x44,0xbe}}
typedef struct efi_reset_notification_protocol efi_reset_notification_protocol;
typedef efi_status(efiapi*efi_register_reset_notify)(
	efi_reset_notification_protocol*this,
	efi_reset_system func
);
typedef efi_status(efiapi*efi_unregister_reset_notify)(
	efi_reset_notification_protocol*this,
	efi_reset_system func
);
struct efi_reset_notification_protocol{
	efi_register_reset_notify register_reset_notify;
	efi_unregister_reset_notify unregister_reset_notify;
};
extern const efi_guid gEfiResetNotificationProtocolGuid;
#endif
