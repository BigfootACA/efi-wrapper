//
// Created by bigfoot on 2023/5/11.
//

#ifndef EFI_WRAPPER_HII_DATABASE_H
#define EFI_WRAPPER_HII_DATABASE_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"../efi_hii.h"
#define EFI_HII_DATABASE_PROTOCOL_GUID {0xef9fc172,0xa1b2,0x4693,{0xb3,0x27,0x6d,0x32,0xfc,0x41,0x60,0x42}}
typedef struct efi_hii_database_protocol efi_hii_database_protocol;
typedef uintn_t efi_hii_database_notify_type;
#define EFI_HII_DATABASE_NOTIFY_NEW_PACK    0x00000001
#define EFI_HII_DATABASE_NOTIFY_REMOVE_PACK 0x00000002
#define EFI_HII_DATABASE_NOTIFY_EXPORT_PACK 0x00000004
#define EFI_HII_DATABASE_NOTIFY_ADD_PACK    0x00000008
typedef efi_status(efiapi*efi_hii_database_notify)(
	uint8_t                      type,
	const efi_guid               *guid,
	const efi_hii_package_header *package,
	efi_hii_handle               handle,
	efi_hii_database_notify_type notify
);
typedef efi_status(efiapi*efi_hii_database_new_pack)(
	const efi_hii_database_protocol   *proto,
	const efi_hii_package_list_header *list,
	efi_handle                        driver,
	efi_hii_handle                    *handle
);
typedef efi_status(efiapi*efi_hii_database_remove_pack)(
	const efi_hii_database_protocol *proto,
	efi_hii_handle                  handle
);
typedef efi_status(efiapi*efi_hii_database_update_pack)(
	const efi_hii_database_protocol   *proto,
	efi_hii_handle                    handle,
	const efi_hii_package_list_header *list
);
typedef efi_status(efiapi*efi_hii_database_list_packs)(
	const efi_hii_database_protocol *proto,
	uint8_t                         type,
	const efi_guid                  *guid,
	uintn_t                         *length,
	efi_hii_handle                  *handle
);
typedef efi_status(efiapi*efi_hii_database_export_packs)(
	const efi_hii_database_protocol *proto,
	efi_hii_handle                  handle,
	uintn_t                         *size,
	efi_hii_package_list_header     *buffer
);
typedef efi_status(efiapi*efi_hii_database_register_notify)(
	const efi_hii_database_protocol *proto,
	uint8_t                         type,
	const efi_guid                  *guid,
	efi_hii_database_notify         func,
	efi_hii_database_notify_type    notify,
	efi_handle                      *handle
);
typedef efi_status(efiapi*efi_hii_database_unregister_notify)(
	const efi_hii_database_protocol *proto,
	efi_handle                       handle
);
typedef efi_status(efiapi*efi_hii_find_keyboard_layouts)(
	const efi_hii_database_protocol *proto,
	uint16_t                        *length,
	efi_guid                        *buffer
);
typedef efi_status(efiapi*efi_hii_get_keyboard_layout)(
	const efi_hii_database_protocol *proto,
	const efi_guid                  *guid,
	uint16_t                        *length,
	efi_hii_keyboard_layout         *layout
);
typedef efi_status(efiapi*efi_hii_set_keyboard_layout)(
	const efi_hii_database_protocol *proto,
	const efi_guid                  *guid
);
typedef efi_status(efiapi*efi_hii_database_get_pack_handle)(
	const efi_hii_database_protocol *proto,
	efi_hii_handle                  list,
	efi_handle                      *driver
);
struct efi_hii_database_protocol{
	efi_hii_database_new_pack           new_package_list;
	efi_hii_database_remove_pack        remove_package_list;
	efi_hii_database_update_pack        update_package_list;
	efi_hii_database_list_packs         list_package_lists;
	efi_hii_database_export_packs       export_package_lists;
	efi_hii_database_register_notify    register_package_notify;
	efi_hii_database_unregister_notify  unregister_package_notify;
	efi_hii_find_keyboard_layouts       find_keyboard_layouts;
	efi_hii_get_keyboard_layout         get_keyboard_layout;
	efi_hii_set_keyboard_layout         set_keyboard_layout;
	efi_hii_database_get_pack_handle    get_package_list_handle;
};
extern const efi_guid gEfiHiiDatabaseProtocolGuid;
#endif //EFI_WRAPPER_HII_DATABASE_H
