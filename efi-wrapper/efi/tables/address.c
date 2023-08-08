#include"internal.h"

efiapi efi_status efi_set_va_map_impl(
	uintn_t                map_size,
	uintn_t                desc_size,
	uint32_t               desc_ver,
	efi_memory_descriptor  *virt_map
){
	log_call(
		call_arg(unsigned,map_size),
		call_arg(unsigned,desc_size),
		call_arg(unsigned,desc_ver),
		call_arg(ptr,virt_map),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_convert_pointer_impl(
	uintn_t debug_disposition,
	void    **address
){
	log_call(
		call_arg(unsigned,debug_disposition),
		call_arg(ptr,address),
		NULL
	);
	log_return(efi_unsupported);
}
