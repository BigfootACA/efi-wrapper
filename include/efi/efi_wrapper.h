//
// Created by bigfoot on 2023/6/24.
//

#ifndef EFI_WRAPPER_H
#define EFI_WRAPPER_H
#include<signal.h>
#include<ucontext.h>
#include"efi/efi_context.h"
#include"efi/device_path.h"
#include"efi/protocols/graphics_output.h"
#include"defines.h"
extern efi_status efi_get_protocol(
	efi_handle handle,
	const efi_guid*protocol,
	efi_interface_entry**entry
);
extern efi_status efi_get_drivers(
	efi_run_context*ctx,
	efi_handle handle,
	const efi_guid*protocol,
	size_t*count,
	efi_handle**buffer
);
extern efi_status efi_get_controllers(
	efi_run_context*ctx,
	efi_handle handle,
	efi_handle driver,
	size_t*count,
	efi_handle**buffer
);
extern efi_status efi_bind_controller(
	efi_handle controller,
	efi_handle driver,
	efi_device_path*remain
);
extern efi_status efi_disconnect_all_drivers(
	efi_run_context*ctx,
	efi_handle handle,
	const efi_guid*protocol,
	efi_handle child
);
extern efi_status efi_connect_single_controller(
	efi_handle controller,
	efi_handle*driver,
	efi_device_path*remain
);
extern efi_status efi_new_device_path_utils(efi_run_context*ctx,efi_handle*handle);
extern efi_status efi_new_graphics_output_from_sdl2(
	efi_handle*handle,
	efi_graphics_output_protocol**proto
);
extern efi_status efi_new_graphics_output_from_vnc(
	int port,efi_handle*handle,
	efi_graphics_output_protocol**proto
);
extern efi_status efi_new_graphics_output_from_memory(
	void*buffer,bool reallocate,
	uint32_t width,uint32_t height,
	efi_handle*handle,
	efi_graphics_output_protocol**proto
);
extern efi_status efi_new_hii_cfg_routing(efi_handle*handle);
extern efi_status efi_new_hii_database(efi_handle*handle);
extern efi_status efi_new_hii_font(efi_handle*handle);
extern efi_status efi_new_hii_image(efi_handle*handle);
extern efi_status efi_new_hii_string(efi_handle*handle);
extern int efi_simple_file_system_posix_fd_get_path(
	efi_file_protocol*fp,
	int*fd,char**path
);
extern efi_status efi_new_simple_file_system_from_posix_fd(
	int fd,
	bool writable,
	efi_handle*handle,
	efi_simple_file_system_protocol**proto
);
extern efi_status efi_new_simple_file_system_from_posix_cfd(
	int fd,
	const char*path,
	bool writable,
	efi_handle*handle,
	efi_simple_file_system_protocol**proto
);
extern efi_status efi_new_simple_file_system_from_posix_path(
	const char*path,
	bool writable,
	efi_handle*handle,
	efi_simple_file_system_protocol**proto
);
extern efi_status efi_file_open_from_device_path(
	const efi_device_path*dp,
	efi_file_protocol**file,
	uint64_t mode,
	uint64_t attr
);
extern efi_status efi_file_read_all(
	efi_file_protocol*fp,
	void**buff,uint64_t*size
);
extern efi_status efi_new_simple_text_input_from_posix_fd(
	int fd,
	efi_handle*handle,
	efi_simple_text_input_protocol**proto
);
extern efi_status efi_new_simple_text_output_from_posix_fd(
	int fd,
	efi_handle*handle,
	efi_simple_text_output_protocol**proto
);
extern bool proc_fake_ins(int sig,siginfo_t*info,ucontext_t*uc);
extern void signal_hand(int sig,siginfo_t*info,void*d);
extern void efi_install_sig_handler(efi_file*file,bool uninstall);
struct efi_wrapper_code{
	uint64_t base;
	list*phdr;
};
#endif
