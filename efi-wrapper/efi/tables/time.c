#include"internal.h"

efiapi efi_status efi_get_time_impl(
	efi_time*t,
	efi_time_capabilities*c
){
	log_call(
		call_arg(ptr,t),
		call_arg(ptr,c),
		NULL
	);
	struct timeval tv;
	if(!t)log_return(efi_invalid_parameter);
	if(gettimeofday(&tv,NULL)!=0)log_return(efi_device_error);
	if(!efi_timeval_to_time(&tv,t))log_return(efi_device_error);
	if(c){
		memset(c,0,sizeof(efi_time_capabilities));
		c->resolution=1;
		c->accuracy=50000000;
		c->sets_to_zero=false;
	}
	log_return(efi_success);
}

efiapi efi_status efi_set_time_impl(
	efi_time*t
){
	log_call(
		call_arg(ptr,t),
		NULL
	);
	log_return(efi_access_denied);
}

efiapi efi_status efi_get_wakeup_time_impl(
	bool     *enabled,
	bool     *pending,
	efi_time *time
){
	log_call(
		call_arg(ptr,enabled),
		call_arg(ptr,pending),
		call_arg(ptr,time),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_set_wakeup_time_impl(
	bool     enable,
	efi_time *time
){
	log_call(
		call_arg(bool,enable),
		call_arg(ptr,time),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_get_next_high_mono_count_impl(
	uint32_t*count
){
	log_call(
		call_arg(ptr,count),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_get_next_monotonic_count_impl(
	uint64_t*count
){
	log_call(
		call_arg(ptr,count),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_set_watchdog_timer_impl(
	uintn_t  timeout,
	uint64_t watchdog_code,
	uintn_t  data_size,
	char16   *watchdog_data
){
	log_call(
		call_arg(unsigned,timeout),
		call_arg(unsigned,watchdog_code),
		call_arg(unsigned,data_size),
		call_arg(str16,watchdog_data),
		NULL
	);
	log_return(efi_unsupported);
}
