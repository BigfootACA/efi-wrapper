//
// Created by bigfoot on 2023/5/8.
//

#include<string.h>
#include"efi/efi_spec.h"
#include"efi/efi_context.h"
#include"efi/device_path.h"
#include"efi/protocols/simple_filesystem.h"
#include "efi/guids/file_info.h"

efi_status efi_file_open_from_device_path(
	const efi_device_path*dp,
	efi_file_protocol**file,
	uint64_t mode,
	uint64_t attr
){
	bool end;
	uint64_t sm,sa;
	efi_status status;
	efi_handle device=NULL;
	filepath_device_path*fp;
	efi_device_path*next,*idp;
	efi_simple_file_system_protocol*fs=NULL;
	efi_file_protocol*cur_file=NULL,*last_file;
	static const efi_guid*guid=&gEfiSimpleFileSystemProtocolGuid;
	if(!dp||!file||!efi_current_ctx)return efi_invalid_parameter;
	*file=NULL,idp=(efi_device_path*)dp;
	status=efi_current_ctx->bs->locate_device_path(guid,&idp,&device);
	if(efi_error(status))return status;
	if(!device)return efi_unsupported;
	status=efi_current_ctx->bs->handle_proto(device,guid,(void**)&fs);
	if(efi_error(status))return status;
	if(!fs)return efi_load_error;
	status=fs->open_volume(fs,&cur_file);
	if(efi_error(status))return status;
	if(!cur_file)return efi_load_error;
	do{
		if(
			efi_device_path_get_type(idp)!=MEDIA_DEVICE_PATH||
			efi_device_path_get_type(idp)!=MEDIA_FILEPATH_DP
		){
			last_file->close(last_file);
			return efi_unsupported;
		}
		fp=(filepath_device_path*)idp;
		last_file=cur_file,cur_file=NULL;
		next=efi_device_path_node_next(idp);
		end=efi_device_path_is_end(next);
		sm=end?mode:EFI_FILE_MODE_READ,sa=end?attr:0;
		status=last_file->open(last_file,&cur_file,fp->path_name,sm,sa);
                last_file->close(last_file);
		idp=next;
        }while(!efi_error(status)&&!end);
	if(!efi_error(status))*file=cur_file;
	return status;
}

efi_status efi_file_read_all(efi_file_protocol*fp,void**buff,uint64_t*size){
	void*buf;
	efi_file_info*info=NULL;
	size_t len=sizeof(efi_file_info),off=0,r;
	efi_status status=efi_out_of_resources;
	if(!fp||!buff)return efi_invalid_parameter;
	do{
		if(info)free(info);
		if(!(info=malloc(len)))break;
		memset(info,0,len);
		status=fp->get_info(fp,&gEfiFileInfoGuid,&len,info);
	}while(status==efi_buffer_too_small);
	if(efi_error(status)){
		if(info)free(info);
		return status;
	}
	if(!info)return efi_out_of_resources;
	if(info->file_size<=0)return efi_end_of_file;
	len=info->file_size,buf=*buff;
	if(buf&&size&&*size>0&&len>*size)len=*size;
	if(!buf&&!(buf=malloc(len+1)))return efi_out_of_resources;
	memset(buf,0,len+1);
	while(off<len){
		r=len-off;
		status=fp->read(fp,&r,buf+off);
		if(efi_error(status))break;
		off+=r;
	}
	if(status==efi_end_of_file||!efi_error(status)){
		if(size)*size=off;
		status=efi_success;
		*buff=buf;
	}else{
		if(!*buff)free(buf);
		if(size)*size=0;
		*buff=NULL;
	}
	return status;
}