//
// Created by bigfoot on 2023/5/9.
//

#ifndef EFI_WRAPPER_DEVICE_PATH_H
#define EFI_WRAPPER_DEVICE_PATH_H
#include"mem.h"
#include"efi_types.h"
#include"protocols/device_path.h"
extern uint8_t efi_device_path_get_type(const void*node);
extern uint8_t efi_device_path_get_sub_type(const void*node);
extern uint16_t efi_device_path_get_node_length(const void*node);
extern uint16_t efi_device_path_set_node_length(void*node,uint16_t length);
extern efi_device_path*efi_device_path_node_next(const void*node);
extern bool efi_device_path_is_end_type(const void*node);
extern bool efi_device_path_is_end_sub_type(const void*node);
extern bool efi_device_path_is_end_instance_sub_type(const void*node);
extern bool efi_device_path_is_end(const void*node);
extern bool efi_device_path_is_end_instance(const void*node);
extern bool efi_device_path_valid(const efi_device_path*dp,size_t max);
extern void efi_device_path_add_node_end(void*node);
extern size_t efi_device_path_get_size(const efi_device_path*dp);
extern size_t efi_device_path_get_instance_size(const efi_device_path*dp);
extern efi_device_path*efi_device_path_dup(mem_pool*pool,const efi_device_path*dp);
extern efi_device_path*efi_device_path_append(mem_pool*pool,const efi_device_path*dp1,const efi_device_path*dp2);
extern efi_device_path*efi_device_path_node_append(mem_pool*pool,const efi_device_path*dp,const efi_device_path*node);
extern efi_device_path*efi_device_path_instance_append(mem_pool*pool,const efi_device_path*dp,const efi_device_path*instance);
extern efi_device_path*efi_device_path_instance_next(mem_pool*pool,efi_device_path**dp,uintn_t*size);
extern bool efi_device_path_is_multi_instance(const efi_device_path*dp);
extern efi_device_path*efi_device_path_create_node(mem_pool*pool,uint8_t type,uint8_t sub_type,uint16_t length);
extern efi_device_path*efi_device_path_from_handle(efi_handle handle);
extern ssize_t efi_device_path_get_path(const efi_device_path*dp,char sep,char*buff,size_t len);
extern efi_device_path*efi_device_path_from_file_str8(mem_pool*pool,efi_handle handle,const char*file);
extern efi_device_path*efi_device_path_from_file_str16(mem_pool*pool,efi_handle handle,const char16*file);
#endif //EFI_WRAPPER_DEVICE_PATH_H
