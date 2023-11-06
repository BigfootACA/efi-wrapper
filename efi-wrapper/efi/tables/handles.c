//
// Created by bigfoot on 2023/5/10.
//
#include"internal.h"

static bool efi_search_all_handles(list*l,void*d){
	(void)l;
	(void)d;
	return true;
}

static bool efi_search_by_register_notify(list*l1,void*d){
	list*l2;
	efi_register_notify*notify=d;
	if(!(l2=list_first(notify->handles)))return false;
	LIST_DATA_DECLARE(hand1,l1,efi_handle);
	LIST_DATA_DECLARE(hand2,l2,efi_handle);
	return hand1==hand2;
}

static bool efi_search_by_protocol(list*l1,void*d){
	list*l2;
	efi_guid*guid=d;
	LIST_DATA_DECLARE(hand,l1,efi_handle);
	if((l2=list_first(hand->interface)))do{
		LIST_DATA_DECLARE(intf,l2,efi_interface_entry*);
		if(intf&&efi_guid_equals(&intf->guid,guid))return true;
	}while((l2=l2->next));
	return false;
}

efiapi efi_status efi_protocols_per_handle_impl(
	efi_handle handle,
	efi_guid***buffer,
	uintn_t    *count
){
	log_call(
		call_arg(handle,handle),
		call_arg(ptr,buffer),
		call_arg(ptr,count),
		NULL
	);
	list*l;
	int cnt;
	size_t len,i=0;
	if(count)*count=0;
	if(buffer)*buffer=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_handle(handle))log_return(efi_invalid_parameter);
	if((cnt=list_count(handle->interface))<0)cnt=0;
	if(count)*count=cnt;
	if(buffer){
		len=(cnt+1)*sizeof(efi_guid*);
		*buffer=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,len);
		if(!*buffer)log_return(efi_out_of_resources);
		memset(*buffer,0,len);
		if(cnt>0&&(l=list_first(handle->interface)))do{
			LIST_DATA_DECLARE(info,l,efi_interface_entry *);
			if(info)(*buffer)[i++]=&info->guid;
		}while((l=l->next)&&i<(size_t)cnt);
	}
	log_return(efi_success);
}

efiapi efi_status efi_locate_handle_impl(
	efi_locate_search_type type,
	const efi_guid*protocol,
	void*key,
	uintn_t*length,
	efi_handle*buffer
){
	log_call(
		call_arg(hex,type),
		call_arg(guid,protocol),
		call_arg(ptr,key),
		call_arg(ptr,length),
		call_arg(ptr,buffer),
		NULL
	);
	list*l;
	void*arg=NULL;
	size_t size,cnt=0,i=0;
	list_comparator cmp=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!length)log_return(efi_invalid_parameter);
	switch(type){
		case search_all_handles:cmp=efi_search_all_handles;break;
		case search_by_register_notify:
			if(!key)log_return(efi_invalid_parameter);
			cmp=efi_search_all_handles,arg=key;
		break;
		case search_by_protocol:
			if(!protocol)log_return(efi_invalid_parameter);
			cmp=efi_search_by_protocol,arg=(efi_guid*)protocol;
		break;
		default:log_return(efi_invalid_parameter);
	}
	if((l=list_first(efi_current_ctx->handles)))do{
		LIST_DATA_DECLARE(hand,l,efi_handle);
		if(efi_valid_handle(hand)&&cmp(l,arg))cnt++;
	}while((l=l->next));
	if(cnt<=0)log_return(efi_not_found);
	size=cnt*sizeof(efi_handle);
	if(*length<size){
		*length=size;
		log_return(efi_buffer_too_small);
	}
	if(!buffer)log_return(efi_invalid_parameter);
	memset(buffer,0,*length);
	*length=size;
	xlog(LOG_CALL,"found %zu match handles",cnt);
	if((l=list_first(efi_current_ctx->handles)))do{
		LIST_DATA_DECLARE(hand,l,efi_handle);
		if(!efi_valid_handle(hand)||!cmp(l,arg))continue;
		xlog(LOG_CALL,"add handle %p",hand);
		buffer[i++]=hand;
	}while((l=l->next)&&i<cnt);
	log_return(efi_success);
}

efiapi efi_status efi_handle_protocol_impl(
	efi_handle handle,
	const efi_guid*protocol,
	void**interface
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(ptr,interface),
		NULL
	);
	efi_file*f=efi_running_get();
	if(!efi_current_ctx||!f)
		log_return(efi_not_ready);
	log_return(efi_open_protocol_impl(
		handle,protocol,interface,
		f->image_handle,NULL,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
	));
}

efiapi efi_status efi_locate_handle_buffer_impl(
	efi_locate_search_type type,
	const efi_guid*protocol,
	void*key,
	uintn_t*handles,
	efi_handle**buffer
){
	log_call(
		call_arg(hex,type),
		call_arg(guid,protocol),
		call_arg(ptr,key),
		call_arg(ptr,handles),
		call_arg(ptr,buffer),
		NULL
	);
	efi_status st;
        uintn_t size=0;
	if(handles)*handles=0;
	if(buffer)*buffer=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
        if(!handles||!buffer)log_return(efi_invalid_parameter);
        st=efi_locate_handle_impl(type,protocol,key,&size,*buffer);
        if(st!=efi_buffer_too_small)log_return(efi_error(st)?st:efi_invalid_parameter);
	if(size<sizeof(efi_handle))log_return(efi_invalid_parameter);
        *buffer=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,size);
        if(!*buffer)log_return(efi_out_of_resources);
        st=efi_locate_handle_impl(type,protocol,key,&size,*buffer);
        if(efi_error(st))log_return(st);
        *handles=(size/sizeof(efi_handle));
	log_return(efi_success);
}
