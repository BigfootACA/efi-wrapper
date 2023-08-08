#include"internal.h"

efi_status efi_get_drivers(
	efi_run_context*ctx,
	efi_handle handle,
	const efi_guid*protocol,
	size_t*count,
	efi_handle**buffer
){
	log_call(
		call_arg(ptr,ctx),
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(ptr,count),
		call_arg(ptr,buffer),
		NULL
	);
	list*l1,*l2;
        size_t cnt=0,i;
        bool duplicate;
	if((l1=list_first(handle->interface)))do{
		LIST_DATA_DECLARE(intf,l1,efi_interface_entry*);
		if(!intf)continue;
		if(protocol&&!efi_guid_equals(&intf->guid,protocol))continue;
		if((l2=list_first(intf->open_list)))do{
			LIST_DATA_DECLARE(info,l2,efi_open_proto_info*);
			if(info&&(info->attrs&EFI_OPEN_PROTOCOL_BY_DRIVER))cnt++;
		}while((l2=l2->next));
	}while((l1=l1->next));
        *count=0;
        if(cnt<=0)log_return(efi_success);
        *buffer=mem_allocate(ctx->mem[efi_bs_data].pool,cnt*sizeof(struct efi_handle));
        if(!*buffer)log_return(efi_out_of_resources);
	if((l1=list_first(handle->interface)))do{
		LIST_DATA_DECLARE(intf,l1,efi_interface_entry*);
		if(!intf)continue;
		if(protocol&&!efi_guid_equals(&intf->guid,protocol))continue;
		if((l2=list_first(intf->open_list)))do{
			LIST_DATA_DECLARE(info,l2,efi_open_proto_info*);
			if(!info||!(info->attrs&EFI_OPEN_PROTOCOL_BY_DRIVER))continue;
			duplicate=false;
                        for(i=0;i<*count;i++)if((*buffer)[i]==info->agent)
				duplicate=true;
                        if(!duplicate)i=(*count)++,(*buffer)[i]=info->agent;
		}while((l2=l2->next));
	}while((l1=l1->next));
        log_return(efi_success);
}

efi_status efi_get_controllers(
	efi_run_context*ctx,
	efi_handle handle,
	efi_handle driver,
	size_t*count,
	efi_handle**buffer
){
	log_call(
		call_arg(ptr,ctx),
		call_arg(handle,handle),
		call_arg(handle,driver),
		call_arg(ptr,count),
		call_arg(ptr,buffer),
		NULL
	);
	list*l1,*l2;
        size_t cnt=0,i;
        bool dup;
	if((l1=list_first(handle->interface)))do{
		LIST_DATA_DECLARE(intf,l1,efi_interface_entry*);
		if(intf&&(l2=list_first(intf->open_list)))do{
			LIST_DATA_DECLARE(info,l2,efi_open_proto_info*);
			if(
				info&&info->agent==driver&&
				(info->attrs&EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER)
			)cnt++;
		}while((l2=l2->next));
	}while((l1=l1->next));
        *count=0;
        if(cnt<=0)log_return(efi_success);
        if(!(*buffer=mem_allocate(
		ctx->mem[efi_bs_data].pool,
		cnt*sizeof(struct efi_handle)
	)))log_return(efi_out_of_resources);
	if((l1=list_first(handle->interface)))do{
		LIST_DATA_DECLARE(intf,l1,efi_interface_entry*);
		if(intf&&(l2=list_first(intf->open_list)))do{
			LIST_DATA_DECLARE(info,l2,efi_open_proto_info*);
			if(
				!info||info->agent!=driver||
				!(info->attrs&EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER)
			)continue;
			dup=false;
                        for(i=0;i<*count;i++)if((*buffer)[i]==info->controller)dup=true;
                        if(!dup)i=(*count)++,(*buffer)[i]=info->controller;
		}while((l2=l2->next));
	}while((l1=l1->next));
        log_return(efi_success);
}

efi_status efi_disconnect_all_drivers(
	efi_run_context*ctx,
	efi_handle handle,
	const efi_guid*protocol,
	efi_handle child
){
	log_call(
		call_arg(ptr,ctx),
		call_arg(handle,handle),
		call_arg(guid,protocol),
		call_arg(handle,child),
		NULL
	);
        size_t count=0;
        efi_status ret;
        efi_handle*buffer=NULL;
        ret=efi_get_drivers(ctx,handle,protocol,&count,&buffer);
	if(efi_error(ret))log_return(ret);
        if(count<=0)log_return(efi_success);
        ret=efi_not_found;
        while(count>0){
		count--;
                if(!efi_error(efi_disconnect_controller_impl(
			handle,buffer[count],child
		)))ret=efi_success;
        }
        mem_free(buffer);
        log_return(ret);
}

efi_status efi_bind_controller(
	efi_handle controller,
	efi_handle driver,
	efi_device_path*remain
){
	efi_status r;
	efi_driver_binding_protocol*binding;
	log_call(
		call_arg(handle,controller),
		call_arg(handle,driver),
		call_arg(device_path,remain),
		NULL
	);
	r=efi_open_protocol_impl(driver,
		&gEfiDriverBindingProtocolGuid,
		(void**)&binding,driver,NULL,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
	);
	if(efi_error(r))log_return(r);
	r=binding->supported(binding,controller,remain);
	if(!efi_error(r))r=binding->start(binding,controller,remain);
	efi_close_protocol_impl(
		driver,
		&gEfiDriverBindingProtocolGuid,
		driver,NULL
	);
        log_return(r);
}

efi_status efi_connect_single_controller(
	efi_handle controller,
	efi_handle*driver,
	efi_device_path*remain
){
        efi_status st;
        efi_handle*buffer;
        size_t count,i,connected=0;
	log_call(
		call_arg(handle,controller),
		call_arg(handle,driver),
		call_arg(device_path,remain),
		NULL
	);
        st=efi_locate_handle_buffer_impl(
		search_by_protocol,
		&gEfiDriverBindingProtocolGuid,
		NULL,&count,&buffer
	);
        if(efi_error(st))log_return(st);
        if(driver)for(;*driver;driver++)for(i=0;i<count;i++)if(buffer[i]==*driver){
                buffer[i]=NULL;
                st=efi_bind_controller(controller,*driver,remain);
                if(!efi_error(st))connected++;
        }
        for(i=0;i<count;i++)if(buffer[i]){
                st=efi_bind_controller(controller,buffer[i],remain);
                if(!efi_error(st))connected++;
        }
        mem_free(buffer);
        log_return(connected>0?efi_success:efi_not_found);
}

efiapi efi_status efi_connect_controller_impl(
	efi_handle               controller,
	efi_handle               *driver,
	efi_device_path_protocol *remaining,
	bool                     recursive
){
	log_call(
		call_arg(handle,controller),
		call_arg(handle,driver),
		call_arg(device_path,remaining),
		call_arg(bool,recursive),
		NULL
	);
	list*l1,*l2;
	efi_status ret=efi_not_found;
	if(driver)*driver=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_handle(controller))log_return(efi_invalid_parameter);
	if(!efi_error(efi_connect_single_controller(
		controller,driver,remaining
	)))ret=efi_success;
	if(recursive&&(l1=list_first(controller->interface)))do{
		LIST_DATA_DECLARE(intf,l1,efi_interface_entry*);
		if(intf&&(l2=list_first(intf->open_list)))do{
			LIST_DATA_DECLARE(info,l2,efi_open_proto_info*);
			if(!info||!(info->attrs&EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER))continue;
			if(!efi_error(efi_connect_controller_impl(
				info->controller,driver,
				remaining,recursive
			)))ret=efi_success;
		}while((l2=l2->next));
	}while((l1=l1->next));
	if(
		efi_error(ret)&&remaining&&
		remaining->type==DEVICE_PATH_TYPE_END
	)ret=efi_success;
	log_return(ret);
}

efiapi efi_status efi_disconnect_controller_impl(
	efi_handle controller,
	efi_handle driver,
	efi_handle child
){
	log_call(
		call_arg(handle,controller),
		call_arg(handle,driver),
		call_arg(handle,child),
		NULL
	);
	size_t cnt=0;
	efi_status st;
	efi_handle*buffer=NULL;
	efi_driver_binding_protocol*bind;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_handle(controller))log_return(efi_invalid_parameter);
	if(child&&!efi_valid_handle(child))log_return(efi_invalid_parameter);
        if(!driver)log_return(efi_disconnect_all_drivers(efi_current_ctx,controller,NULL,child));
        st=efi_get_controllers(efi_current_ctx,controller,driver,&cnt,&buffer);
	if(efi_error(st))log_return(st);
	if(child){
		if(buffer)mem_free(buffer);
		cnt=0,buffer=&child;
	}
	if(efi_error(efi_open_protocol_impl(
		driver,&gEfiDriverBindingProtocolGuid,
		(void**)&bind,driver,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL
	))){
		if(!child)mem_free(buffer);
		log_return(efi_invalid_parameter);
	}
	if(
		(cnt>0&&efi_error(bind->stop(bind,controller,cnt,buffer)))||
		((!child||cnt==1)&&efi_error(bind->stop(bind,controller,0,NULL)))
	){
		if(!child)mem_free(buffer);
		log_return(efi_device_error);
	}
	efi_close_protocol_impl(
		driver,&gEfiDriverBindingProtocolGuid,
		driver,NULL
	);
	log_return(efi_success);
}
