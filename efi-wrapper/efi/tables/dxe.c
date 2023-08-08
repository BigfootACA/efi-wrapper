#include"internal.h"

efiapi efi_status efi_add_mem_space_impl(
	efi_gcd_mem_type type,
	efi_address      base,
	uint64_t         len,
	uint64_t         caps
){
	log_call(
		call_arg(hex,type),
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		call_arg(hex,caps),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_alloc_mem_space_impl(
	efi_gcd_alloc_type alloc_type,
	efi_gcd_mem_type   mem_type,
	uintn_t            align,
	uint64_t           len,
	efi_address*       base,
	efi_handle         image,
	efi_handle         device
){
	log_call(
		call_arg(hex,alloc_type),
		call_arg(hex,mem_type),
		call_arg(unsigned,align),
		call_arg(hex,len),
		call_arg(ptr,base),
		call_arg(handle,image),
		call_arg(handle,device),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_free_mem_space_impl(
	efi_address  base,
	uint64_t     len
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_remove_mem_space_impl(
	efi_address base,
	uint64_t    len
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_get_mem_space_desc_impl(
	efi_address             base,
	efi_gcd_mem_space_desc  *desc
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(ptr,desc),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_set_mem_space_attrs_impl(
	efi_address base,
	uint64_t    len,
	uint64_t    attrs
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		call_arg(hex,attrs),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_set_mem_space_caps_impl(
	efi_address base,
	uint64_t    len,
	uint64_t    caps
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		call_arg(hex,caps),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_get_mem_space_map_impl(
	uintn_t                *num,
	efi_gcd_mem_space_desc **map
){
	log_call(
		call_arg(ptr,num),
		call_arg(hex,map),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_add_io_space_impl(
	efi_gcd_io_type io_type,
	efi_address     base,
	uint64_t        len
){
	log_call(
		call_arg(hex,io_type),
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_alloc_io_space_impl(
	efi_gcd_alloc_type alloc_type,
	efi_gcd_io_type    io_type,
	uintn_t            align,
	uint64_t           len,
	efi_address        *base,
	efi_handle         image,
	efi_handle         device
){
	log_call(
		call_arg(hex,alloc_type),
		call_arg(hex,io_type),
		call_arg(unsigned,align),
		call_arg(hex,len),
		call_arg(ptr,base),
		call_arg(handle,image),
		call_arg(handle,device),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_free_io_space_impl(
	efi_address  base,
	uint64_t     len
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_remove_io_space_impl(
	efi_address  base,
	uint64_t     len
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(hex,len),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_get_io_space_desc_impl(
	efi_address            base,
	efi_gcd_io_space_desc  *desc
){
	log_call(
		call_arg(ptr,base.ptr),
		call_arg(ptr,desc),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_get_io_space_map_impl(
	uintn_t                *cnt,
	efi_gcd_io_space_desc  **map
){
	log_call(
		call_arg(ptr,cnt),
		call_arg(ptr,map),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_dispatch_impl(void){
	log_call(NULL);
	log_return(efi_unsupported);
}

efiapi efi_status efi_schedule_impl(
	efi_handle     fv_hand,
	const efi_guid *file
){
	log_call(
		call_arg(handle,fv_hand),
		call_arg(guid,file),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_trust_impl(
	efi_handle     fv_hand,
	const efi_guid *file
){
	log_call(
		call_arg(handle,fv_hand),
		call_arg(guid,file),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_proc_fv_impl(
	const void *fv_hdr,
	uintn_t    size,
	efi_handle *fv_hand
){
	log_call(
		call_arg(ptr,fv_hdr),
		call_arg(hex,size),
		call_arg(ptr,fv_hand),
		NULL
	);
	log_return(efi_unsupported);
}
