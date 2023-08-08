//
// Created by bigfoot on 2023/5/12.
//

#include"efi/efi_context.h"
#include"efi/protocols/timestamp.h"

static efiapi uint64_t timestamp_get_impl(void){
	return 0;
}

static efiapi efi_status timestamp_get_properties_impl(
	efi_timestamp_properties*prop
){
	log_call(
		call_arg(ptr,prop),
		NULL
	);
	log_return(efi_unsupported);
}

efi_timestamp_protocol ts_protocol={
	.get_timestamp  = timestamp_get_impl,
	.get_properties = timestamp_get_properties_impl,
};
