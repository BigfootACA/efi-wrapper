//
// Created by bigfoot on 2023/5/3.
//
#include<iconv.h>
#include"internal.h"

typedef struct efi_graphics_output_protocol_private_memory{
	efi_graphics_output_protocol_private data;
}efi_graphics_output_protocol_private_memory;

static efiapi efi_status efi_gop_set_mode(
	efi_graphics_output_protocol *proto,
	uint32_t                     num
){
	log_call(
		call_arg(ptr,proto),
		call_arg(unsigned,num),
		NULL
	);
	efi_status st;
	efi_graphics_output_mode_information*mode;
	efi_graphics_output_protocol_private_memory*p=
		(efi_graphics_output_protocol_private_memory*)proto;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!proto||num>=efi_gop_mode.max_mode)
		log_return(efi_invalid_parameter);
	if(!efi_guid_equals(&p->data.magic,&gEfiWrapperMemoryGraphicsOutputProtocolGuid))
		log_return(efi_invalid_parameter);
	if(!p->data.reallocate)log_return(efi_unsupported);
	mode=&efi_gop_modes[num];
	st=efi_update_size(&p->data,mode,p->data.cur,NULL);
	if(efi_error(st))return st;
	p->data.mode.mode=num,p->data.cur=mode;
	log_return(efi_success);
}

static efiapi efi_status efi_gop_blt(
	efi_graphics_output_protocol      *proto,
	efi_graphics_output_blt_pixel     *buffer,
	efi_graphics_output_blt_operation oper,
	uintn_t                           src_x,
	uintn_t                           src_y,
	uintn_t                           dst_x,
	uintn_t                           dst_y,
	uintn_t                           width,
	uintn_t                           height,
	uintn_t                           delta
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,buffer),
		call_arg(hex,oper),
		call_arg(unsigned,src_x),
		call_arg(unsigned,src_y),
		call_arg(unsigned,dst_x),
		call_arg(unsigned,dst_y),
		call_arg(unsigned,width),
		call_arg(unsigned,height),
		call_arg(unsigned,delta),
		NULL
	);
	efi_status st;
	efi_graphics_output_protocol_private_memory*p=
		(efi_graphics_output_protocol_private_memory*)proto;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperMemoryGraphicsOutputProtocolGuid))
		log_return(efi_invalid_parameter);
	if(!p->data.cfg)log_return(efi_not_ready);
	st=fb_blt(
		p->data.cfg,buffer,oper,
		src_x,src_y,dst_x,dst_y,
		width,height,delta
	);
	log_return(st);
}

static const efi_graphics_output_protocol protocol={
	.query_mode = efi_gop_query_mode,
	.set_mode   = efi_gop_set_mode,
	.blt        = efi_gop_blt,
	.mode       = NULL,
};

efi_status efi_new_graphics_output_from_memory(
	void*buffer,bool reallocate,
	uint32_t width,uint32_t height,
	efi_handle*handle,
	efi_graphics_output_protocol**proto
){
	bool found=false;
	efi_status st;
	static const size_t size=sizeof(efi_graphics_output_protocol_private_memory);
	efi_graphics_output_protocol_private_memory*ret;
	if(!efi_current_ctx)return efi_not_ready;
	if(!handle||!proto)return efi_invalid_parameter;
	if(!(ret=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,size)))return efi_out_of_resources;
	memset(ret,0,size);
	memcpy(&ret->data.protocol,&protocol,sizeof(ret->data.protocol));
	memcpy(&ret->data.mode,&efi_gop_mode,sizeof(efi_gop_mode));
	memcpy(&ret->data.magic,&gEfiWrapperMemoryGraphicsOutputProtocolGuid,sizeof(efi_guid));
	if(width==0&&height==0)found=true,ret->data.cur=&efi_gop_modes[0];
	else for(uint32_t i=0;i<efi_gop_mode.max_mode;i++){
		if(ret->data.cur->width!=width||ret->data.cur->height!=height)continue;
		ret->data.cur=&efi_gop_modes[i];
		found=true;
		break;
	}
	if(!found)return efi_unsupported;
	ret->data.reallocate=reallocate;
	ret->data.protocol.mode=&ret->data.mode;
	st=efi_update_size(&ret->data,ret->data.cur,NULL,buffer);
	if(efi_error(st))return st;
	*proto=(efi_graphics_output_protocol*)ret;
	return efi_current_ctx->bs->install_proto_interface(
		handle,&gEfiGraphicsOutputProtocolGuid,
		efi_native_interface,ret
	);
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	return efi_new_graphics_output_from_memory(
		NULL,true,1024,768,&ins->handle,
		(efi_graphics_output_protocol**)&ins->cust_data
	);
}

static efi_status svc_condition(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)
		return efi_invalid_parameter;
	if(efi_service_is_svc_started(ctx,&gEfiGraphicsOutputProtocolGuid))
		return efi_already_started;
	return efi_success;
}

static const efi_service efi_mem_graphics_output_service={
	.guid      = &gEfiWrapperMemoryGraphicsOutputProtocolGuid,
	.name      = "MemoryGraphicsOutputProtocol",
	.provides  = {
		&gEfiGraphicsOutputProtocolGuid,
		NULL
	},
	.after     = {
		&gEfiWrapperSDL2GraphicsOutputProtocolGuid,
		&gEfiWrapperVNCGraphicsOutputProtocolGuid,
		NULL
	},
	.depends   = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.condition = svc_condition,
	.start     = svc_start,
};

DECLARE_EFI_SERVICE(efi_mem_graphics_output_service)
