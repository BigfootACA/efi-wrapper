#include"internal.h"

static bool var_cmp(list*l,void*d){
	efi_variable_item*i1=d;
	LIST_DATA_DECLARE(i2,l,efi_variable_item*);
	if(!i1||!i2)return false;
	if((i1->name!=NULL)!=(i2->name!=NULL))return false;
	if(i1->name&&ustrcmp(i1->name,i2->name)!=0)return false;
	if(!efi_guid_equals(&i1->guid,&i2->guid))return false;
	return true;
}

efiapi efi_status efi_get_var_impl(
	const char16   *variable_name,
	const efi_guid *vendor_guid,
	uint32_t *attributes,
	uintn_t  *data_size,
	void     *data
){
	log_call(
		call_arg(str16,variable_name),
		call_arg(guid,vendor_guid),
		call_arg(ptr,attributes),
		call_arg(ptr,data_size),
		call_arg(ptr,data),
		NULL
	);
	list*l;
	efi_variable_item var,*ret;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!variable_name||!vendor_guid||!data_size)
		log_return(efi_invalid_parameter);
	memset(&var,0,sizeof(var));
	memcpy(&var.guid,vendor_guid,sizeof(efi_guid));
	var.name=(char16*)variable_name;
	if(!(l=list_search_one(
		efi_current_ctx->vars,
		var_cmp,&var
	)))log_return(efi_not_found);
	if(!(ret=LIST_DATA(l,efi_variable_item*)))
		log_return(efi_device_error);
	if(!ret->data||ret->len<=0)log_return(efi_device_error);
	if(ret->len>*data_size){
		*data_size=ret->len;
		log_return(efi_buffer_too_small);
	}
	if(!data)log_return(efi_invalid_parameter);
	memcpy(data,ret->data,ret->len);
	log_return(efi_success);
}

efiapi efi_status efi_get_next_var_name_impl(
	uintn_t*variable_name_size,
	char16*variable_name,
	const efi_guid*vendor_guid
){
	log_call(
		call_arg(ptr,variable_name_size),
		call_arg(ptr,variable_name),
		call_arg(guid,vendor_guid),
		NULL
	);
	if(!efi_current_ctx)log_return(efi_not_ready);
	log_return(efi_unsupported);
}

efiapi efi_status efi_set_var_impl(
	const char16   *variable_name,
	const efi_guid *vendor_guid,
	uint32_t attrs,
	uintn_t  data_size,
	void     *data
){
	log_call(
		call_arg(str16,variable_name),
		call_arg(guid,vendor_guid),
		call_arg(unsigned,attrs),
		call_arg(unsigned,data_size),
		call_arg(ptr,data),
		NULL
	);
	list*l;
	size_t len;
	efi_status st;
	efi_variable_item var,*ret;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!variable_name||!*variable_name||!vendor_guid)
		log_return(efi_invalid_parameter);
	memset(&var,0,sizeof(var));
	memcpy(&var.guid,vendor_guid,sizeof(efi_guid));
	var.name=(char16*)variable_name,var.len=data_size,var.attrs=attrs;
	if(data&&data_size>0){
		var.data=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,data_size+1);
		if(!var.data)EDONE(st=efi_out_of_resources);
		memset(var.data,0,data_size+1);
		memcpy(var.data,data,data_size);
	}
	if((l=list_search_one(
		efi_current_ctx->vars,
		var_cmp,&var
	))){
		if(!(ret=LIST_DATA(l,efi_variable_item*)))
			EDONE(st=efi_device_error);
		if(ret->data)mem_free(ret->data);
		if(!var.data){
			mem_free(ret->name);
			memset(ret,0,sizeof(var));
			list_obj_del(&efi_current_ctx->vars,l,list_default_free);
		}else{
			var.name=ret->name;
			memcpy(ret,&var,sizeof(var));
		}
	}else if(var.data){
		len=ustrsize(variable_name);
		var.name=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,len);
		if(!var.name)EDONE(st=efi_out_of_resources);
		memcpy(var.name,variable_name,len);
		list_obj_add_new_dup(
			efi_current_ctx->mem[efi_bs_data].pool,
			&efi_current_ctx->vars,
			&var,sizeof(var)
		);
	}else EDONE(st=efi_not_found);
	log_return(efi_success);
	done:
	if(var.data)mem_free(var.data);
	log_return(st);
}

efiapi efi_status efi_query_var_info_impl(
	uint32_t attrs,
	uint64_t *max_var_storage,
	uint64_t *remain_var_storage,
	uint64_t *max_var_size
){
	log_call(
		call_arg(unsigned,attrs),
		call_arg(ptr,max_var_storage),
		call_arg(ptr,remain_var_storage),
		call_arg(ptr,max_var_size),
		NULL
	);
	if((attrs&EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS)!=0)
		log_return(efi_unsupported);
	else if((attrs&EFI_VARIABLE_ATTRIBUTES_MASK)==0)
		log_return(efi_unsupported);
	else if((attrs&(EFI_VARIABLE_RUNTIME_ACCESS|EFI_VARIABLE_BOOTSERVICE_ACCESS))==EFI_VARIABLE_RUNTIME_ACCESS)
		log_return(efi_invalid_parameter);
	else if((attrs&(EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_HARDWARE_ERROR_RECORD))==EFI_VARIABLE_HARDWARE_ERROR_RECORD)
		log_return(efi_invalid_parameter);
	else if((attrs&EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS)!=0)
		log_return(efi_unsupported);
	else if((attrs&EFI_VARIABLE_HARDWARE_ERROR_RECORD)!=0)
		log_return(efi_unsupported);
	if(max_var_storage)*max_var_storage=UINT64_MAX;
	if(remain_var_storage)*remain_var_storage=UINT64_MAX;
	if(max_var_size)*max_var_size=UINT64_MAX;
	log_return(efi_success);
}
