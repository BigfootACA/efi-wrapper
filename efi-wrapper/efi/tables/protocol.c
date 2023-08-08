#include"internal.h"

efi_status efi_get_protocol(
	efi_handle handle,
	const efi_guid*protocol,
	efi_interface_entry**entry
){
	log_call(
		call_arg(ptr,handle),
		call_arg(guid,protocol),
		call_arg(ptr,entry),
		NULL
	);
	list*l;
	if(!protocol||!entry||!efi_valid_handle(handle))
		log_return(efi_invalid_parameter);
	if((l=list_first(handle->interface)))do{
		LIST_DATA_DECLARE(intf,l,efi_interface_entry*);
		if(!intf||!intf->interface)continue;
		if(!efi_guid_equals(protocol,&intf->guid))continue;
		*entry=intf;
		xlog(LOG_CALL,"out entry=%p",*entry);
		log_return(efi_success);
	}while((l=l->next));
	log_return(efi_not_found);
}

efiapi efi_status efi_locate_protocol_impl(
	const efi_guid*protocol,
	void*registration,
	void**interface
){
	log_call(
		call_arg(guid,protocol),
		call_arg(ptr,registration),
		call_arg(ptr,interface),
		NULL
	);
	list*l;
	efi_status st;
	efi_interface_entry*entry=NULL;
	efi_register_notify*notify=NULL;
	if(interface)*interface=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!protocol||!interface)log_return(efi_invalid_parameter);
	if(registration){
		notify=registration;
		if(!(l=list_first(notify->handles))||!efi_guid_equals(
			&notify->protocol,protocol
		))log_return(efi_not_found);
		LIST_DATA_DECLARE(hand,l,efi_handle);
		if(!efi_valid_handle(hand))log_return(efi_not_found);
		st=efi_get_protocol(hand,protocol,&entry);
		if(efi_error(st))log_return(st);
		*interface=entry->interface;
		xlog(LOG_CALL,"out interface=%p",*interface);
		log_return(efi_success);
	}else if((l=list_first(efi_current_ctx->handles)))do{
		LIST_DATA_DECLARE(hand,l,efi_handle);
		if(!efi_valid_handle(hand))continue;
		st=efi_get_protocol(hand,protocol,&entry);
		if(efi_error(st))continue;
		*interface=entry->interface;
		xlog(LOG_CALL,"out interface=%p",*interface);
		log_return(efi_success);
	}while((l=l->next));
	log_return(efi_not_found);
}

efiapi efi_status efi_install_protocol_interface_impl(
	efi_handle         *handle,
	const efi_guid     *protocol,
	efi_interface_type type,
	void               *interface
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(hex,type),
		call_arg(ptr,interface),
		NULL
	);
	list*l;
	char buff[64];
	efi_interface_entry entry;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!handle||!protocol||!interface||type!=efi_native_interface)
		log_return(efi_invalid_parameter);
	if(!*handle&&!(*handle=efi_new_handle(NULL)))log_return(efi_out_of_resources);
	if(!efi_valid_handle(*handle))log_return(efi_invalid_parameter);
	if((l=list_first((*handle)->interface)))do{
		LIST_DATA_DECLARE(intf,l,efi_interface_entry*);
		if(!intf||!intf->interface)continue;
		if(efi_guid_equals(protocol,&intf->guid)){
			xlog(LOG_CALL,"target interface already exists");
			log_return(efi_invalid_parameter);
		}
	}while((l=l->next));
	entry.open_list=NULL;
	entry.interface=interface;
	memcpy(&entry.guid,protocol,sizeof(efi_guid));
	if(list_obj_add_new_dup(
		efi_current_ctx->mem[efi_bs_data].pool,
		&(*handle)->interface,&entry,
		sizeof(efi_interface_entry)
	)!=0)log_return(efi_out_of_resources);
	if((l=list_first(efi_current_ctx->notifies)))do{
		LIST_DATA_DECLARE(notify,l,efi_register_notify*);
		if(!notify||!efi_guid_equals(
			&notify->protocol,protocol
		)||!notify->event)continue;
		list_obj_add_new(
			efi_current_ctx->mem[efi_bs_data].pool,
			&notify->handles,*handle
		);
		notify->event->signaled=false;
		efi_signal_event_impl(notify->event);
	}while((l=l->next));
	xlog(LOG_INFO,
		"install protocol %s to handle %p with interface %p",
		efi_guid_to_readable_string(protocol,buff,sizeof(buff)),
		*handle,interface
	);
	log_return(efi_success);
}

efiapi efi_status efi_install_multiple_protocol_interfaces_impl(
	efi_handle*handle,
	...
){
	log_call(
		call_arg(handle,handle),
		NULL
	);
	VA_LIST va1,va2;
	efi_guid*protocol;
	void*interface;
	efi_status st=efi_success;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!handle)log_return(efi_invalid_parameter);
	if(!*handle&&!(*handle=efi_new_handle(NULL)))log_return(efi_out_of_resources);
	if(!efi_valid_handle(*handle))log_return(efi_invalid_parameter);
	VA_START(va1,handle);
	VA_COPY(va2,va1);
	do{
		protocol=VA_ARG(va2,efi_guid*),interface=VA_ARG(va2,void*);
		if(!protocol)break;
		if(!interface)st=efi_invalid_parameter;
	}while(true);
	VA_END(va2);
	if(efi_error(st)){
		VA_END(va1);
		log_return(st);
	}
	VA_COPY(va2,va1);
	do{
		protocol=VA_ARG(va1,efi_guid*),interface=VA_ARG(va1,void*);
		if(!protocol)break;
		st=efi_install_protocol_interface_impl(
			handle,protocol,efi_native_interface,interface
		);
	}while(!efi_error(st));
	VA_END(va1);
	if(efi_error(st))while(true){
		protocol=VA_ARG(va2,efi_guid*),interface=VA_ARG(va2,void*);
		if(!protocol)break;
		efi_uninstall_protocol_interface_impl(*handle,protocol,interface);
	}
	VA_END(va2);
	log_return(st);
}

efiapi efi_status efi_reinstall_protocol_interface_impl(
	efi_handle handle,
	const efi_guid *protocol,
	void*old_interface,
	void*new_interface
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(ptr,old_interface),
		call_arg(ptr,new_interface),
		NULL
	);
	efi_status st;
	st=efi_uninstall_protocol_interface_impl(
		handle,protocol,old_interface
	);
	if(efi_error(st))log_return(st);
	st=efi_install_protocol_interface_impl(
		&handle,protocol,efi_native_interface,new_interface
	);
	if(efi_error(st))log_return(st);
	efi_connect_controller_impl(handle,NULL,NULL,true);
	log_return(efi_success);
}

efiapi efi_status efi_uninstall_protocol_interface_impl(
	efi_handle     handle,
	const efi_guid *protocol,
	void           *interface
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(ptr,interface),
		NULL
	);
	list*l,*n;
	efi_status st;
	efi_interface_entry*entry=NULL;
	st=efi_get_protocol(handle,protocol,&entry);
	if(efi_error(st))log_return(st);
	efi_disconnect_all_drivers(efi_current_ctx,handle,protocol,NULL);
	if((l=list_first(entry->open_list)))do{
		n=l->next;
		LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
		if(!info)continue;
		switch(info->attrs){
			case EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL:
			case EFI_OPEN_PROTOCOL_GET_PROTOCOL:
			case EFI_OPEN_PROTOCOL_TEST_PROTOCOL:break;
			default:continue;
		}
		list_obj_del(&handle->interface,l,list_default_free);
	}while((l=n));
	if(entry->open_list)log_return(efi_access_denied);
	if(interface!=entry->interface)log_return(efi_not_found);
	list_obj_del_data(&entry->open_list,entry,list_default_free);
	log_return(efi_success);
}

efiapi efi_status efi_uninstall_multiple_protocol_interfaces_impl(
	efi_handle handle,
	...
){
	log_call(
		call_arg(handle,handle),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_open_protocol_impl(
	efi_handle      handle,
	const efi_guid  *protocol,
	void            **intf,
	efi_handle      agent,
	efi_handle      controller,
	uint32_t        attrs
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(ptr,intf),
		call_arg(handle,agent),
		call_arg(handle,controller),
		call_arg(unsigned,attrs),
		NULL
	);
	list*l,*n;
	efi_status st;
	efi_open_proto_info*data=NULL;
	efi_interface_entry*entry=NULL;
        bool by_driver=false,exclusive=false;
	if(intf)memset(intf,0,sizeof(void*));
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!protocol||!efi_valid_handle(handle))
		log_return(efi_invalid_parameter);
	if((attrs!=EFI_OPEN_PROTOCOL_TEST_PROTOCOL)&&!intf)
		log_return(efi_invalid_parameter);
	switch(attrs){
		case EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER:
			if(agent==controller)
				log_return(efi_invalid_parameter);
		//fallthrough
		case EFI_OPEN_PROTOCOL_BY_DRIVER:
		case EFI_OPEN_PROTOCOL_BY_DRIVER|EFI_OPEN_PROTOCOL_EXCLUSIVE:
			if(!efi_valid_handle(controller))
				log_return(efi_invalid_parameter);
		//fallthrough
		case EFI_OPEN_PROTOCOL_EXCLUSIVE:
			if(!efi_valid_handle(agent))
				log_return(efi_invalid_parameter);
		break;
		case EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL:
		case EFI_OPEN_PROTOCOL_GET_PROTOCOL:
		case EFI_OPEN_PROTOCOL_TEST_PROTOCOL:
		break;
		default:log_return(efi_invalid_parameter);
	}
	st=efi_get_protocol(handle,protocol,&entry);
	if(st==efi_not_found)st=efi_unsupported;
	if(efi_error(st))log_return(st);
	if(!entry)log_return(efi_unsupported);
	if(!agent){
		if(attrs!=EFI_OPEN_PROTOCOL_TEST_PROTOCOL)
			memcpy(intf,&entry->interface,sizeof(void*));
		log_return(efi_success);
	}
	if(intf&&(l=list_first(entry->open_list)))do{
		LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
		if(!info)continue;
		if(info->agent==agent){
			if((attrs&EFI_OPEN_PROTOCOL_BY_DRIVER)&&(info->attrs==attrs))
				log_return(efi_already_started);
		}else if((info->attrs&EFI_OPEN_PROTOCOL_BY_DRIVER))by_driver=true;
		if((info->attrs&EFI_OPEN_PROTOCOL_EXCLUSIVE))exclusive=true;
	}while((l=l->next));
	if((attrs&EFI_OPEN_PROTOCOL_EXCLUSIVE)){
		if(exclusive)log_return(efi_access_denied);
	}else if((attrs&EFI_OPEN_PROTOCOL_BY_DRIVER)){
		if(exclusive||by_driver)log_return(efi_access_denied);
	}
	if((attrs&EFI_OPEN_PROTOCOL_EXCLUSIVE)){
		by_driver=false;
		if((l=list_first(entry->open_list)))do{
			n=l->next;
			LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
			if(!info||info->attrs!=EFI_OPEN_PROTOCOL_BY_DRIVER)continue;
			st=efi_disconnect_controller_impl(info->controller,info->agent,NULL);
			if(efi_error(st))by_driver=true;
			else n=list_first(entry->open_list);
		}while((l=n));
		if(by_driver)log_return(efi_access_denied);
	}
	if((l=list_first(entry->open_list)))do{
		LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
		if(!info)continue;
		if(info->agent!=agent)continue;
		if(info->attrs!=attrs)continue;
		if(info->controller!=controller)continue;
		data=info;
	}while((l=l->next));
	if(!data){
		if(!(data=mem_allocate(
			efi_current_ctx->mem[efi_bs_data].pool,
			sizeof(efi_open_proto_info)
		)))log_return(efi_out_of_resources);
		memset(data,0,sizeof(efi_open_proto_info));
		list_obj_add_new(
			efi_current_ctx->mem[efi_bs_data].pool,
			&entry->open_list,data
		);
	}
	data->agent=agent;
	data->controller=controller;
	data->attrs=attrs;
	data->count++;
	if(attrs!=EFI_OPEN_PROTOCOL_TEST_PROTOCOL)
		memcpy(intf,&entry->interface,sizeof(void*));
	log_return(efi_success);
}

efiapi efi_status efi_close_protocol_impl(
	efi_handle     handle,
	const efi_guid *protocol,
	efi_handle     agent,
	efi_handle     controller
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(handle,agent),
		call_arg(handle,controller),
		NULL
	);
	list*l,*n;
	efi_status st;
	efi_interface_entry*entry=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!protocol||!efi_valid_handle(handle)||!efi_valid_handle(agent))
		log_return(efi_invalid_parameter);
	if(controller&&!efi_valid_handle(controller))
		log_return(efi_invalid_parameter);
	st=efi_get_protocol(handle,protocol,&entry);
	if(efi_error(st))log_return(st);
	st=efi_not_found;
	if((l=list_first(entry->open_list)))do{
		n=l->next;
		LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
		if(!info)continue;
		if(info->agent!=agent)continue;
		if(info->controller!=controller)continue;
		list_obj_del(&entry->open_list,l,NULL);
		memset(info,0,sizeof(efi_open_proto_info));
		mem_free(info);
		st=efi_success;
	}while((l=n));
	log_return(st);
}

efiapi efi_status efi_open_protocol_information_impl(
	efi_handle          handle,
	const efi_guid      *protocol,
	efi_open_proto_info **buffer,
	uintn_t             *count
){
	log_call(
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(ptr,buffer),
		call_arg(ptr,count),
		NULL
	);
	list*l;
	efi_status st;
	size_t cnt=0,len,i=0;
	efi_interface_entry*entry=NULL;
	if(count)*count=0;
	if(buffer)*buffer=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	st=efi_get_protocol(handle,protocol,&entry);
	if(efi_error(st))log_return(st);
	if((l=list_first(entry->open_list)))do{
		LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
		if(info&&info->count>0)cnt++;
	}while((l=l->next));
	if(count)*count=cnt;
	if(buffer){
		len=(cnt+1)*sizeof(efi_open_proto_info);
		*buffer=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,len);
		if(!*buffer)log_return(efi_out_of_resources);
		memset(*buffer,0,len);
		if(cnt>0&&(l=list_first(entry->open_list)))do{
			LIST_DATA_DECLARE(info,l,efi_open_proto_info*);
			if(info&&info->count>0)memcpy(
				&(*buffer)[i++],info,
				sizeof(efi_open_proto_info)
			);
		}while((l=l->next)&&i<cnt);
	}
	log_return(efi_success);
}

efiapi efi_status efi_locate_device_path_impl(
	const efi_guid           *protocol,
	efi_device_path_protocol **device_path,
	efi_handle               *device
){
	log_call(
		call_arg(guid,protocol),
		call_arg(device_path,device_path),
		call_arg(ptr,device),
		NULL
	);
	efi_status st;
	uintn_t cnt=0,i;
	efi_device_path*dp,*idp;
	efi_handle*handles=NULL;
	size_t len,len_dp,len_best=0;
	if(device)*device=NULL;
	if(!protocol||!device_path)log_return(efi_invalid_parameter);
	memcpy(&idp,device_path,sizeof(void*));
	len=efi_device_path_get_instance_size(idp);
	st=efi_locate_handle_buffer_impl(search_by_protocol,protocol,NULL,&cnt,&handles);
	if(efi_error(st))log_return(st);
	for(i=0;i<cnt;i++){
		dp=NULL,st=efi_handle_protocol_impl(
			handles[i],
			&gEfiDevicePathProtocolGuid,
			(void**)&dp
		);
		if(efi_error(st)||!dp)continue;
		len_dp=efi_device_path_get_instance_size(dp);
                if(len_dp<=len_best||len_dp>len)continue;
                if(memcmp(idp,dp,len_dp)!=0)continue;
                if(!device)log_return(efi_invalid_parameter);
                *device=handles[i];
                len_best=len_dp;
	}
        if(len_best>0){
		idp=(void*)idp+len_best;
	        memcpy(device_path,&idp,sizeof(void*));
	        log_return(efi_success);
        }
	log_return(efi_not_found);
}
