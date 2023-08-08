#include"internal.h"
#include"path.h"

efiapi efi_status efi_image_load_impl(
	bool                      boot_policy,
	efi_handle                parent,
	efi_device_path_protocol  *device_path,
	void                      *source_buffer,
	uintn_t                   source_size,
	efi_handle                *image_handle
){
	log_call(
		call_arg(bool,boot_policy),
		call_arg(handle,parent),
		call_arg(device_path,device_path),
		call_arg(ptr,source_buffer),
		call_arg(unsigned,source_size),
		call_arg(handle,image_handle),
		NULL
	);
	size_t len=0;
	void*img=NULL;
	efi_status status;
	efi_device_path*dp;
	efi_file_protocol*file=NULL;
	efi_handle handle,device=NULL;
	efi_file*f=efi_running_get(),*nf;
	if(image_handle)*image_handle=NULL;
	if(!efi_current_ctx||!f)log_return(efi_not_ready);
	if(efi_current_ctx->tpl>=TPL_NOTIFY)log_return(efi_load_error);
	if(!device_path||!parent)log_return(efi_invalid_parameter);
	if(source_buffer){
		img=source_buffer,len=source_size,dp=device_path;
		status=efi_locate_device_path_impl(&gEfiDevicePathProtocolGuid,&dp,&device);
		if(efi_error(status))log_return(status);
		if(source_size<=0)log_return(efi_load_error);
	}else{
		status=efi_file_open_from_device_path(device_path,&file,EFI_FILE_MODE_READ,0);
		if(efi_error(status))goto done;
		status=efi_file_read_all(file,&img,&len);
		if(efi_error(status))goto done;
	}
	status=efi_out_of_resources;
	if(!(nf=efi_file_new(f->context)))goto done;
	nf->context=f->context;
	nf->loaded.parent_handle=parent;
	nf->device_path=efi_device_path_dup(f->pool,device_path);
	strncpy(nf->path,"/",sizeof(nf->path)-1);
	efi_device_path_get_path(device_path,'/',nf->path+1,sizeof(nf->path)-1);
	path_basename(nf->name,sizeof(nf->name),nf->path);
	status=efi_unsupported;
	if(!(efi_load_by_data(nf,img,len)))goto done;
	if(!(handle=efi_file_get_image_handle(nf)))
		log_return(efi_load_error);
	if(image_handle)*image_handle=handle;
	xlog(LOG_CALL,"out handle=%p",handle);
	log_return(efi_success);
	done:
	if(file)file->close(file);
	if(img&&img!=source_buffer)free(img);
	log_return(status);
}

efiapi efi_status efi_image_start_impl(
	efi_handle image_handle,
	uintn_t    *exit_data_size,
	char16     **exit_data
){
	log_call(
		call_arg(handle,image_handle),
		call_arg(unsigned,exit_data_size),
		call_arg(ptr,exit_data),
		NULL
	);
	efi_file*file;
	if(exit_data)*exit_data=NULL;
	if(exit_data_size)*exit_data_size=0;
	if(!(file=efi_image_handle_get_file(image_handle)))
		log_return(efi_invalid_parameter);
	file->st=efi_run(file,false);
	if(exit_data)*exit_data=file->exit_data;
	if(exit_data_size)*exit_data_size=file->exit_size;
	log_return(file->st);
}

efiapi efi_status efi_exit_impl(
	efi_handle image_handle,
	efi_status exit_status,
	uintn_t    exit_data_size,
	char16     *exit_data
){
	log_call(
		call_arg(handle,image_handle),
		call_arg(status,exit_status),
		call_arg(unsigned,exit_data_size),
		call_arg(str16,exit_data),
		NULL
	);
	efi_file*f=efi_running_get();
	if(!efi_current_ctx||!f)
		log_return(efi_not_ready);
	if(image_handle!=f->image_handle)
		log_return(efi_invalid_parameter);
	if(exit_data){
		if(f->exit_data)mem_free(f->exit_data);
		f->exit_data=NULL,f->exit_size=0;
		if(!(f->exit_data=mem_allocate(f->pool,exit_data_size)))
			log_return(efi_out_of_resources);
		memcpy(f->exit_data,exit_data,exit_data_size);
		f->exit_size=exit_data_size;
	}
	f->st=exit_status;
	log_call_level--;
	longjmp(f->jmp,0);
	log_call_level++;
	log_return(efi_device_error);
}

efiapi efi_status efi_image_unload_impl(
	efi_handle image_handle
){
	log_call(
		call_arg(handle,image_handle),
		NULL
	);
	log_return(efi_unsupported);
}

efiapi efi_status efi_exit_boot_services_impl(
	efi_handle image_handle,
	uintn_t    map_key
){
	log_call(
		call_arg(handle,image_handle),
		call_arg(unsigned,map_key),
		NULL
	);
	efi_file*f=efi_running_get();
	if(!efi_current_ctx||!f)
		log_return(efi_not_ready);
	if(image_handle!=f->image_handle)
		log_return(efi_invalid_parameter);
	efi_current_ctx->timer=false;
	xlog(LOG_ERROR,"trying to call exit boot services");
	log_return(efi_success);
}
