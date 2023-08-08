#include"internal.h"

efiapi void efi_reset_system_impl(
	efi_reset_type type,
	efi_status     status,
	uintn_t        size,
	void           *data
){
	log_call(
		call_arg(hex,type),
		call_arg(status,status),
		call_arg(unsigned,size),
		call_arg(ptr,data),
		NULL
	);
	efi_file*f;
	if(!(f=efi_running_get()))return;
	xerror("trying to call reset system, terminate app");
	efi_exit_impl(f->image_handle,status,size,data);
	log_return_void();
}

efiapi efi_status efi_update_capsule_impl(
	efi_capsule_header    **header_array,
	uintn_t                count,
	efi_address            scatter_gather_list
){
	log_call(
		call_arg(hex,header_array),
		call_arg(unsigned,count),
		call_arg(ptr,scatter_gather_list.ptr),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_query_capsule_caps_impl(
	efi_capsule_header **header_array,
	uintn_t            count,
	uint64_t           *max_size,
	efi_reset_type     *reset_type
){
	log_call(
		call_arg(hex,header_array),
		call_arg(unsigned,count),
		call_arg(unsigned,max_size),
		call_arg(ptr,reset_type),
		NULL
	);
	log_return(efi_unsupported);
}
