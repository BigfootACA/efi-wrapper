//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_BOOT_SERVICES_H
#define EFI_WRAPPER_BOOT_SERVICES_H
#include"efi_types.h"
#include"efi/protocols/device_path.h"

#define EFI_BOOT_SERVICES_SIGNATURE   SIGNATURE_64 ('B','O','O','T','S','E','R','V')
#define EFI_BOOT_SERVICES_REVISION    EFI_SPECIFICATION_VERSION
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020
typedef enum efi_timer_delay{
	timer_cancel   = 0x00,
	timer_periodic = 0x01,
	timer_relative = 0x02,
}efi_timer_delay;
typedef enum efi_interface_type{
	efi_native_interface = 0x00
}efi_interface_type;
typedef enum efi_locate_search_type{
	search_all_handles        = 0x00,
	search_by_register_notify = 0x01,
	search_by_protocol        = 0x02,
}efi_locate_search_type;
typedef struct efi_open_proto_info{
	efi_handle  agent;
	efi_handle  controller;
	uint32_t    attrs;
	uint32_t    count;
}efi_open_proto_info;
typedef efiapi efi_status(*efi_allocate_pages)(
	efi_allocate_type type,
	efi_memory_type   memory_type,
	uintn_t           pages,
	efi_address       *memory
);
typedef efiapi efi_status(*efi_free_pages)(
	efi_address memory,
	uintn_t     pages
);
typedef efiapi efi_status(*efi_get_memory_map)(
	uintn_t               *map_size,
	efi_memory_descriptor *map,
	uintn_t               *key,
	uintn_t               *desc_size,
	uint32_t              *desc_ver
);
typedef efiapi efi_status(*efi_allocate_pool)(
	efi_memory_type type,
	uintn_t         size,
	void            **buffer
);
typedef efiapi efi_status(*efi_free_pool)(void*buffer);
typedef efiapi efi_status(*efi_connect_controller)(
	efi_handle               controller_handle,
	efi_handle               *driver_image_handle,
	efi_device_path_protocol *remaining_device_path,
	bool                     recursive
);
typedef efiapi efi_status(*efi_disconnect_controller)(
	efi_handle controller_handle,
	efi_handle driver_image_handle,
	efi_handle child_handle
);
typedef efiapi void(*efi_event_notify)(
	efi_event event,
	void      *context
);
typedef efiapi efi_status(*efi_create_event)(
	uint32_t         type,
	efi_tpl          tpl,
	efi_event_notify func,
	void             *context,
	efi_event        *event
);
typedef efiapi efi_status(*efi_create_event_ex)(
	uint32_t         type,
	efi_tpl          tpl,
	efi_event_notify func,
	const void       *context,
	const efi_guid   *group,
	efi_event        *event
);
typedef efiapi efi_status(*efi_set_timer)(
	efi_event       event,
	efi_timer_delay type,
	uint64_t        trigger_time
);
typedef efiapi efi_status(*efi_signal_event)(efi_event event);
typedef efiapi efi_status(*efi_wait_for_event)(
	uintn_t   events,
	efi_event *event,
	uintn_t   *index
);
typedef efiapi efi_status(*efi_close_event)(efi_event event);
typedef efiapi efi_status(*efi_check_event)(efi_event event);
typedef efiapi efi_tpl(*efi_raise_tpl)(efi_tpl new_tpl);
typedef efiapi void(*efi_restore_tpl)(efi_tpl old_tpl);
typedef efiapi efi_status(*efi_image_load)(
	bool                      boot_policy,
	efi_handle                parent_image_handle,
	efi_device_path_protocol  *device_path,
	void                      *source_buffer,
	uintn_t                   source_size,
	efi_handle                *image_handle
);
typedef efiapi efi_status(*efi_image_start)(
	efi_handle image_handle,
	uintn_t    *exit_data_size,
	char16     **exit_data
);
typedef efiapi efi_status(*efi_exit)(
	efi_handle image_handle,
	efi_status exit_status,
	uintn_t    exit_data_size,
	char16     *exit_data
);
typedef efiapi efi_status(*efi_image_unload)(efi_handle image_handle);
typedef efiapi efi_status(*efi_exit_boot_services)(
	efi_handle image_handle,
	uintn_t    map_key
);
typedef efiapi efi_status(*efi_get_next_monotonic_count)(uint64_t*count);
typedef efiapi efi_status(*efi_stall)(uintn_t microseconds);
typedef efiapi efi_status(*efi_set_watchdog_timer)(
	uintn_t  timeout,
	uint64_t watchdog_code,
	uintn_t  data_size,
	char16   *watchdog_data
);
typedef efiapi efi_status(*efi_calculate_crc32)(
	void     *data,
	uintn_t  size,
	uint32_t *crc32
);
typedef efiapi void(*efi_copy_mem)(
	void    *dst,
	void    *src,
	uintn_t len
);
typedef efiapi void(*efi_set_mem)(
	void    *buffer,
	uintn_t size,
	uint8_t value
);
typedef efiapi efi_status(*efi_install_protocol_interface)(
	efi_handle         *handle,
	const efi_guid           *protocol,
	efi_interface_type type,
	void               *interface
);
typedef efiapi efi_status(*efi_install_multiple_protocol_interfaces)(
	efi_handle*handle,
	...
);
typedef efiapi efi_status(*efi_reinstall_protocol_interface)(
	efi_handle handle,
	const efi_guid   *protocol,
	void       *old_interface,
	void       *new_interface
);
typedef efiapi efi_status(*efi_uninstall_protocol_interface)(
	efi_handle handle,
	const efi_guid   *protocol,
	void       *interface
);
typedef efiapi efi_status(*efi_uninstall_multiple_protocol_interfaces)(
	efi_handle handle,
	...
);
typedef efiapi efi_status(*efi_handle_protocol)(
	efi_handle handle,
	const efi_guid   *protocol,
	void       **interface
);
typedef efiapi efi_status
(*efi_open_protocol)(
	efi_handle handle,
	const efi_guid   *protocol,
	void       **interface,
	efi_handle agent_handle,
	efi_handle controller_handle,
	uint32_t   attributes
);
typedef efiapi efi_status(*efi_close_protocol)(
	efi_handle handle,
	const efi_guid   *protocol,
	efi_handle agent_handle,
	efi_handle controller_handle
);
typedef efiapi efi_status(*efi_open_protocol_information)(
	efi_handle          handle,
	const efi_guid            *protocol,
	efi_open_proto_info **buffer,
	uintn_t             *count
);
typedef efiapi efi_status(*efi_protocols_per_handle)(
	efi_handle handle,
	efi_guid   ***protocol_buffer,
	uintn_t    *protocol_buffer_count
);
typedef efiapi efi_status(*efi_register_protocol_notify)(
	const efi_guid*protocol,
	efi_event event,
	void**registration
);
typedef efiapi efi_status(*efi_locate_handle)(
	efi_locate_search_type type,
	const efi_guid         *protocol,
	void                   *key,
	uintn_t                *size,
	efi_handle             *buffer
);
typedef efiapi efi_status(*efi_locate_device_path)(
	const efi_guid           *protocol,
	efi_device_path_protocol **device_path,
	efi_handle               *device
);
typedef efiapi efi_status(*efi_install_configuration_table)(
	const efi_guid*guid,
	void*table
);
typedef efiapi efi_status(*efi_locate_handle_buffer)(
	efi_locate_search_type type,
	const efi_guid         *protocol,
	void                   *key,
	uintn_t                *handles,
	efi_handle             **buffer
);
typedef efiapi efi_status(*efi_locate_protocol)(
	const efi_guid*protocol,
	void*registration,
	void**interface
);
extern const efi_guid gEfiEventMemoryMapChangeGuid;
extern const efi_guid gEfiEventVirtualAddressChangeGuid;
extern const efi_guid gEfiEventExitBootServicesGuid;
typedef struct efi_boot_services{
	efi_table_header                           header;
	efi_raise_tpl                              raise_tpl;
	efi_restore_tpl                            restore_tpl;
	efi_allocate_pages                         alloc_pages;
	efi_free_pages                             free_pages;
	efi_get_memory_map                         get_mem_map;
	efi_allocate_pool                          allocate_pool;
	efi_free_pool                              free_pool;
	efi_create_event                           create_event;
	efi_set_timer                              set_timer;
	efi_wait_for_event                         wait_for_event;
	efi_signal_event                           signal_event;
	efi_close_event                            close_event;
	efi_check_event                            check_event;
	efi_install_protocol_interface             install_proto_interface;
	efi_reinstall_protocol_interface           reinstall_proto_interface;
	efi_uninstall_protocol_interface           uninstall_proto_interface;
	efi_handle_protocol                        handle_proto;
	void                                       *reserved;
	efi_register_protocol_notify               register_proto_notify;
	efi_locate_handle                          locate_handle;
	efi_locate_device_path                     locate_device_path;
	efi_install_configuration_table            install_cfg_table;
	efi_image_load                             load_image;
	efi_image_start                            start_image;
	efi_exit                                   exit;
	efi_image_unload                           unload_image;
	efi_exit_boot_services                     exit_boot_services;
	efi_get_next_monotonic_count               get_next_mono_count;
	efi_stall                                  stall;
	efi_set_watchdog_timer                     set_watchdog_timer;
	efi_connect_controller                     connect_controller;
	efi_disconnect_controller                  disconnect_controller;
	efi_open_protocol                          open_proto;
	efi_close_protocol                         close_proto;
	efi_open_protocol_information              open_proto_info;
	efi_protocols_per_handle                   proto_per_handle;
	efi_locate_handle_buffer                   locate_handle_buffer;
	efi_locate_protocol                        locate_proto;
	efi_install_multiple_protocol_interfaces   install_multiple_proto_interfaces;
	efi_uninstall_multiple_protocol_interfaces uninstall_multiple_proto_interfaces;
	efi_calculate_crc32                        calculate_crc32;
	efi_copy_mem                               copy_mem;
	efi_set_mem                                set_mem;
	efi_create_event_ex                        create_event_ex;
}efi_boot_services;
#endif
