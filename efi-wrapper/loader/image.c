#include<errno.h>
#include<unistd.h>
#include<string.h>
#include"efi/efi_file.h"
#include"efi/efi_string.h"
#include"efi/device_path.h"

#define EFI_FILE_HANDLE_MAGIC 0x6d001e95de64bf80

static efi_loaded_image_protocol loaded_image={
	.revision = EFI_LOADED_IMAGE_INFORMATION_REVISION,
	.image_code_type = efi_loader_code,
	.image_data_type = efi_loader_data,
};

static efi_status add_loaded_image(efi_file*file){
	efi_status st;
	efi_run_context*old;
	efi_handle device=NULL;
	pe_subsystem subsys;
	efi_service_instance*ins=NULL;
	old=efi_current_ctx,efi_current_ctx=file->context;
	memcpy(&file->loaded,&loaded_image,sizeof(efi_loaded_image_protocol));
	if(!file->loaded.revision){
		file->loaded.revision=EFI_LOADED_IMAGE_INFORMATION_REVISION;
		OPT_HDR_GET(file,subsys,subsystem,EDONE(st=efi_unsupported))
		switch(subsys){
			case SUBSYSTEM_EFI_APPLICATION:
				file->loaded.image_code_type=efi_loader_code;
				file->loaded.image_data_type=efi_loader_data;
			break;
			case SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			case SUBSYSTEM_EFI_RUNTIME_DRIVER:
			case SUBSYSTEM_EFI_ROM:
				file->loaded.image_code_type=efi_bs_code;
				file->loaded.image_data_type=efi_bs_data;
			break;
			default:EDONE(st=efi_unsupported);
		}
	}
	if(!file->loaded.device_handle){
		ins=efi_service_lookup_instance(file->context,&gEfiSimpleFileSystemProtocolGuid);
		if(ins)device=ins->handle;
		else xerror("no file system found");
	}else device=file->loaded.device_handle;
	if(!file->device_path&&!(file->device_path=efi_device_path_from_file_str8(
		file->pool,device,file->path
	))){
		xerror("generate device path for efi file failed");
		EDONE(st=efi_out_of_resources);
	}
	file->loaded.file_path=file->device_path;
	file->loaded.image_base=file->image;
	file->loaded.image_size=file->image_size;
	file->loaded.unload=file->context->bs->unload_image;
	file->loaded.system_table=file->context->st;
	file->loaded.device_handle=device;
	st=file->context->bs->install_multiple_proto_interfaces(
		&file->image_handle,
		&gEfiDevicePathProtocolGuid,file->device_path,
		&gEfiLoadedImageProtocolGuid,&file->loaded,
		NULL
	);
	if(efi_error(st))xerror(
		"install loaded image protocol failed: %s",
		efi_status_to_string(st)
	);
	done:efi_current_ctx=old;
	return st;
}

efi_handle efi_file_get_image_handle(efi_file*file){
	efi_status status;
	if(!file||!file->context||!file->entry)EPRET(EINVAL);
	if(!file->image_handle){
		file->image_handle=efi_new_handle(file->context);
		if(!file->image_handle)EPRET(ENOMEM);
		file->image_handle->type=EFI_FILE_HANDLE_MAGIC;
		file->image_handle->userdata=file;
		status=add_loaded_image(file);
		if(efi_error(status))EPRET(efi_status_to_errno(status));
	}
	return file->image_handle;
}

efi_file*efi_image_handle_get_file(efi_handle handle){
	if(!handle||handle->type!=EFI_FILE_HANDLE_MAGIC)return NULL;
	return handle->userdata;
}
