#include<unistd.h>
#include"mem.h"
#include"defines.h"
#include"efi/efi_hii.h"
#include"efi/efi_context.h"

efi_handle efi_new_handle(efi_run_context*ctx){
	efi_handle handle;
	if(!ctx){
		if(!efi_current_ctx)return NULL;
		else ctx=efi_current_ctx;
	}
	if(!(handle=mem_allocate(
		ctx->mem[efi_bs_data].pool,
		sizeof(struct efi_handle)
	)))return NULL;
	memset(handle,0,sizeof(struct efi_handle));
	handle->sig=EFI_HANDLE_SIGN;
	xlog(LOG_CALL,"new handle %p",handle);
	xlog_call_stack(3);
	if(list_obj_add_new(
		ctx->mem[efi_bs_data].pool,
		&ctx->handles,handle
	)!=0){
		mem_free(handle);
		handle=NULL;
	}
	return handle;
}

bool efi_valid_handle(efi_handle hand){
	if(!hand)return false;
	if(efi_current_ctx&&!list_search_one(
		efi_current_ctx->handles,
		list_pointer_comparator,hand
	))return false;
	if(hand->sig!=EFI_HANDLE_SIGN)return false;
	return true;
}

bool efi_valid_event(efi_event event){
	if(!event)return false;
	if(efi_current_ctx&&!list_search_one(
		efi_current_ctx->events,
		list_pointer_comparator,event
	))return false;
	if(event->sig!=EFI_EVENT_SIGN)return false;
	return true;
}
