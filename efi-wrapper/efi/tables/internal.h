//
// Created by bigfoot on 2023/5/9.
//

#ifndef EFI_WRAPPER_INTERNAL_H
#define EFI_WRAPPER_INTERNAL_H
#include<time.h>
#include<unistd.h>
#include<sys/time.h>
#include"efi/efi_file.h"
#include"efi/efi_string.h"
#include"efi/efi_wrapper.h"
#include"efi/efi_context.h"
#include"efi/device_path.h"
#include"efi/efi_template.h"
#include"efi/system_table.h"
#include"efi/dxe_services.h"
#include"efi/boot_services.h"
#include"efi/runtime_services.h"
#include"efi/protocols/device_path.h"
#include"efi/protocols/driver_binding.h"
#include"defines.h"

extern efiapi efi_status efi_get_time_impl(
	efi_time              *time,
	efi_time_capabilities *caps
);
extern efiapi efi_status efi_set_time_impl(efi_time*t);
extern efiapi efi_status efi_get_wakeup_time_impl(
	bool     *enabled,
	bool     *pending,
	efi_time *time
);
extern efiapi efi_status efi_set_wakeup_time_impl(
	bool     enable,
	efi_time *time
);
extern efiapi efi_status efi_set_va_map_impl(
	uintn_t                map_size,
	uintn_t                desc_size,
	uint32_t               desc_ver,
	efi_memory_descriptor  *virt_map
);
extern efiapi efi_status efi_convert_pointer_impl(
	uintn_t debug_disposition,
	void    **address
);
extern efiapi efi_status efi_get_var_impl(
	const char16*variable_name,
	const efi_guid*vendor_guid,
	uint32_t*attributes,
	uintn_t*data_size,
	void*data
);
extern efiapi efi_status efi_get_next_var_name_impl(
	uintn_t*variable_name_size,
	char16*variable_name,
	const efi_guid*vendor_guid
);
extern efiapi efi_status efi_set_var_impl(
	const char16*variable_name,
	const efi_guid*vendor_guid,
	uint32_t attributes,
	uintn_t data_size,
	void*data
);
extern efiapi void efi_reset_system_impl(
	efi_reset_type type,
	efi_status status,
	uintn_t size,
	void*data
);
extern efiapi efi_status efi_get_next_high_mono_count_impl(uint32_t*count);
extern efiapi efi_status efi_update_capsule_impl(
	efi_capsule_header **header_array,
	uintn_t            count,
	efi_address        scatter_gather_list
);
extern efiapi efi_status efi_query_capsule_caps_impl(
	efi_capsule_header **header_array,
	uintn_t            count,
	uint64_t           *max_size,
	efi_reset_type     *reset_type
);
extern efiapi efi_status efi_query_var_info_impl(
	uint32_t attributes,
	uint64_t *max_var_storage,
	uint64_t *remain_var_storage,
	uint64_t *max_var_size
);
extern efiapi efi_status efi_add_mem_space_impl(
	efi_gcd_mem_type type,
	efi_address      base,
	uint64_t         len,
	uint64_t         caps
);
extern efiapi efi_status efi_alloc_mem_space_impl(
	efi_gcd_alloc_type alloc_type,
	efi_gcd_mem_type   mem_type,
	uintn_t            align,
	uint64_t           len,
	efi_address*       base,
	efi_handle         image,
	efi_handle         device
);
extern efiapi efi_status efi_free_mem_space_impl(
	efi_address  base,
	uint64_t     len
);
extern efiapi efi_status efi_remove_mem_space_impl(
	efi_address base,
	uint64_t    len
);
extern efiapi efi_status efi_get_mem_space_desc_impl(
	efi_address             base,
	efi_gcd_mem_space_desc  *desc
);
extern efiapi efi_status efi_set_mem_space_attrs_impl(
	efi_address base,
	uint64_t    len,
	uint64_t    attrs
);
extern efiapi efi_status efi_set_mem_space_caps_impl(
	efi_address base,
	uint64_t    len,
	uint64_t    caps
);
extern efiapi efi_status efi_get_mem_space_map_impl(
	uintn_t                *num,
	efi_gcd_mem_space_desc **map
);
extern efiapi efi_status efi_add_io_space_impl(
	efi_gcd_io_type io_type,
	efi_address     base,
	uint64_t        len
);
extern efiapi efi_status efi_alloc_io_space_impl(
	efi_gcd_alloc_type alloc_type,
	efi_gcd_io_type    io_type,
	uintn_t            align,
	uint64_t           len,
	efi_address        *base,
	efi_handle         image,
	efi_handle         device
);
extern efiapi efi_status efi_free_io_space_impl(
	efi_address  base,
	uint64_t     len
);
extern efiapi efi_status efi_remove_io_space_impl(
	efi_address  base,
	uint64_t     len
);
extern efiapi efi_status efi_get_io_space_desc_impl(
	efi_address            base,
	efi_gcd_io_space_desc  *desc
);
extern efiapi efi_status efi_get_io_space_map_impl(
	uintn_t                *cnt,
	efi_gcd_io_space_desc  **map
);
extern efiapi efi_status efi_dispatch_impl(void);
extern efiapi efi_status efi_schedule_impl(
	efi_handle     fv_hand,
	const efi_guid *file
);
extern efiapi efi_status efi_trust_impl(
	efi_handle     fv_hand,
	const efi_guid *file
);
extern efiapi efi_status efi_proc_fv_impl(
	const void *fv_hdr,
	uintn_t    size,
	efi_handle *fv_hand
);
extern efiapi efi_status efi_allocate_pages_impl(
	efi_allocate_type type,
	efi_memory_type   memory_type,
	uintn_t           pages,
	efi_address       *memory
);
extern efiapi efi_status efi_free_pages_impl(
	efi_address memory,
	uintn_t     pages
);
extern efiapi efi_status efi_get_memory_map_impl(
	uintn_t               *map_size,
	efi_memory_descriptor *map,
	uintn_t               *key,
	uintn_t               *desc_size,
	uint32_t              *desc_ver
);
extern efiapi efi_status efi_allocate_pool_impl(
	efi_memory_type type,
	uintn_t         size,
	void            **buffer
);
extern efiapi efi_status efi_free_pool_impl(void*buffer);
extern efiapi efi_status efi_connect_controller_impl(
	efi_handle               controller,
	efi_handle               *driver,
	efi_device_path_protocol *remaining_device_path,
	bool                     recursive
);
extern efiapi efi_status efi_disconnect_controller_impl(
	efi_handle controller,
	efi_handle driver,
	efi_handle child
);
extern efiapi efi_status efi_create_event_impl(
	uint32_t         type,
	efi_tpl          tpl,
	efi_event_notify func,
	void             *context,
	efi_event        *event
);
extern efiapi efi_status efi_create_event_ex_impl(
	uint32_t         type,
	efi_tpl          tpl,
	efi_event_notify func,
	const void       *context,
	const efi_guid   *group,
	efi_event        *event
);
extern efiapi efi_status efi_set_timer_impl(
	efi_event       event,
	efi_timer_delay type,
	uint64_t        trigger_time
);
extern efiapi efi_status efi_signal_event_impl(efi_event event);
extern efiapi efi_status efi_wait_for_event_impl(
	uintn_t   events,
	efi_event *event,
	uintn_t   *index
);
extern efiapi efi_status efi_close_event_impl(efi_event event);
extern efiapi efi_status efi_check_event_impl(efi_event event);
extern efiapi efi_tpl efi_raise_tpl_impl(efi_tpl new_tpl);
extern efiapi void efi_restore_tpl_impl(efi_tpl old_tpl);
extern efiapi efi_status efi_image_load_impl(
	bool                      boot_policy,
	efi_handle                parent_image_handle,
	efi_device_path_protocol  *device_path,
	void                      *source_buffer,
	uintn_t                   source_size,
	efi_handle                *image_handle
);
extern efiapi efi_status efi_image_start_impl(
	efi_handle image_handle,
	uintn_t    *exit_data_size,
	char16     **exit_data
);
extern efiapi efi_status efi_exit_impl(
	efi_handle image_handle,
	efi_status exit_status,
	uintn_t    exit_data_size,
	char16     *exit_data
);
extern efiapi efi_status efi_image_unload_impl(efi_handle image_handle);
extern efiapi efi_status efi_exit_boot_services_impl(
	efi_handle image_handle,
	uintn_t    map_key
);
extern efiapi efi_status efi_get_next_monotonic_count_impl(uint64_t*count);
extern efiapi efi_status efi_stall_impl(uintn_t microseconds);
extern efiapi efi_status efi_set_watchdog_timer_impl(
	uintn_t  timeout,
	uint64_t watchdog_code,
	uintn_t  data_size,
	char16   *watchdog_data
);
extern efiapi efi_status efi_calculate_crc32_impl(
	void     *data,
	uintn_t  size,
	uint32_t *crc32
);
extern efiapi void efi_copy_mem_impl(
	void    *dst,
	void    *src,
	uintn_t len
);
extern efiapi void efi_set_mem_impl(
	void    *buffer,
	uintn_t size,
	uint8_t value
);
extern efiapi efi_status efi_install_protocol_interface_impl(
	efi_handle         *handle,
	const efi_guid           *protocol,
	efi_interface_type type,
	void               *interface
);
extern efiapi efi_status efi_install_multiple_protocol_interfaces_impl(
	efi_handle*handle,
	...
);
extern efiapi efi_status efi_reinstall_protocol_interface_impl(
	efi_handle handle,
	const efi_guid   *protocol,
	void       *old_interface,
	void       *new_interface
);
extern efiapi efi_status efi_uninstall_protocol_interface_impl(
	efi_handle handle,
	const efi_guid   *protocol,
	void       *interface
);
extern efiapi efi_status efi_uninstall_multiple_protocol_interfaces_impl(
	efi_handle handle,
	...
);
extern efiapi efi_status efi_handle_protocol_impl(
	efi_handle handle,
	const efi_guid   *protocol,
	void       **interface
);
extern efiapi efi_status efi_open_protocol_impl(
	efi_handle handle,
	const efi_guid*protocol,
	void**interface,
	efi_handle agent,
	efi_handle controller,
	uint32_t   attributes
);
extern efiapi efi_status efi_close_protocol_impl(
	efi_handle handle,
	const efi_guid*protocol,
	efi_handle agent,
	efi_handle controller
);
extern efiapi efi_status efi_open_protocol_information_impl(
	efi_handle          handle,
	const efi_guid      *protocol,
	efi_open_proto_info **buffer,
	uintn_t             *count
);
extern efiapi efi_status efi_protocols_per_handle_impl(
	efi_handle handle,
	efi_guid   ***protocol_buffer,
	uintn_t    *protocol_buffer_count
);
extern efiapi efi_status efi_register_protocol_notify_impl(
	const efi_guid*protocol,
	efi_event event,
	void**registration
);
extern efiapi efi_status efi_locate_handle_impl(
	efi_locate_search_type type,
	const efi_guid         *protocol,
	void                   *key,
	uintn_t                *size,
	efi_handle             *buffer
);
extern efiapi efi_status efi_locate_device_path_impl(
	const efi_guid                 *protocol,
	efi_device_path_protocol **device_path,
	efi_handle               *device
);
extern efiapi efi_status efi_install_configuration_table_impl(
	const efi_guid *guid,
	void     *table
);
extern efiapi efi_status efi_locate_handle_buffer_impl(
	efi_locate_search_type type,
	const efi_guid               *protocol,
	void                   *key,
	uintn_t                *handles,
	efi_handle             **buffer
);
extern efiapi efi_status efi_locate_protocol_impl(
	const efi_guid *protocol,
	void     *registration,
	void     **interface
);
#endif //EFI_WRAPPER_INTERNAL_H
