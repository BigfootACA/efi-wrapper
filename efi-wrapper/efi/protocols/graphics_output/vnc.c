//
// Created by bigfoot on 2023/5/3.
//
#include"internal.h"
#include "efi/protocols/absolute_pointer.h"
#include "efi/protocols/simple_pointer.h"
#include "efi/protocols/simple_text_input_ex.h"

#ifdef VNCSERVER_FOUND
#include<rfb/rfb.h>
#include<semaphore.h>
#include<pthread.h>
#include <fcntl.h>

typedef struct efi_graphics_output_protocol_private_vnc{
	efi_graphics_output_protocol_private data;
	rfbScreenInfoPtr server;
}efi_graphics_output_protocol_private_vnc;

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
	efi_graphics_output_protocol_private_vnc*p=
		(efi_graphics_output_protocol_private_vnc*)proto;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!proto||num>=efi_gop_mode.max_mode)
		log_return(efi_invalid_parameter);
	if(!efi_guid_equals(&p->data.magic,&gEfiWrapperVNCGraphicsOutputProtocolGuid))
		log_return(efi_invalid_parameter);
	if(!p->data.reallocate)log_return(efi_unsupported);
	mode=&efi_gop_modes[num];
	st=efi_update_size(&p->data,mode,p->data.cur,NULL);
	if(!efi_error(st))p->data.mode.mode=num,p->data.cur=mode;
	log_return(st);
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
	efi_graphics_output_protocol_private_vnc*p=
		(efi_graphics_output_protocol_private_vnc*)proto;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperVNCGraphicsOutputProtocolGuid))
		log_return(efi_invalid_parameter);
	if(!p->data.cfg)log_return(efi_not_ready);
	st=fb_blt(
		p->data.cfg,buffer,oper,
		src_x,src_y,dst_x,dst_y,
		width,height,delta
	);
	if(!efi_error(st)){
		int x1=dst_x,x2=dst_x+width-src_x;
		int y1=dst_y,y2=dst_y+height-src_y;
		xlog(LOG_CALL,"(%d,%d)-(%d,%d)",x1,y1,x2,y2);
		rfbMarkRectAsModified(p->server,x1,y1,x2,y2);
		rfbProcessEvents(p->server,1000);
	}
	log_return(st);
}

static const efi_graphics_output_protocol protocol={
	.query_mode = efi_gop_query_mode,
	.set_mode   = efi_gop_set_mode,
	.blt        = efi_gop_blt,
	.mode       = NULL,
};

efi_status efi_new_graphics_output_from_vnc(
	int port,efi_handle*handle,
	efi_graphics_output_protocol**proto
){
	efi_status st;
	static const size_t size=sizeof(efi_graphics_output_protocol_private_vnc);
	efi_graphics_output_protocol_private_vnc*ret;
	if(!efi_current_ctx)return efi_not_ready;
	if(!handle)return efi_invalid_parameter;
	if(!(ret=mem_allocate(
		efi_current_ctx->mem[efi_bs_data].pool,size
	)))return efi_out_of_resources;
	memset(ret,0,size);
	memcpy(&ret->data.protocol,&protocol,sizeof(ret->data.protocol));
	memcpy(&ret->data.mode,&efi_gop_mode,sizeof(efi_gop_mode));
	memcpy(&ret->data.magic,&gEfiWrapperVNCGraphicsOutputProtocolGuid,sizeof(efi_guid));
	ret->data.cur=&efi_gop_modes[0];
	ret->data.reallocate=true;
	ret->data.protocol.mode=&ret->data.mode;
	st=efi_update_size(&ret->data,ret->data.cur,NULL,NULL);
	if(efi_error(st))goto fail;
	if(!(ret->server=rfbGetScreen(
		0,NULL,ret->data.cur->width,ret->data.cur->height,
		8,3,sizeof(efi_graphics_output_blt_pixel)
	)))goto fail;
	ret->server->desktopName="efi-wrapper";
	ret->server->frameBuffer=ret->data.buffer;
	ret->server->alwaysShared=true;
	ret->server->httpDir=NULL;
	ret->server->port=port;
	rfbLogEnable(true);
	rfbInitServer(ret->server);
	rfbRunEventLoop(ret->server,-1,1);
	if(proto)*proto=(efi_graphics_output_protocol*)ret;
	return efi_current_ctx->bs->install_proto_interface(
		handle,&gEfiGraphicsOutputProtocolGuid,
		efi_native_interface,ret
	);
	fail:
	if(ret->data.cfg)mem_free(ret->data.cfg);
	if(ret->data.buffer)mem_free(ret->data.buffer);
	mem_free(ret);
	return efi_device_error;
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	return efi_new_graphics_output_from_vnc(
		5900,&ins->handle,
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

static const efi_service efi_vnc_graphics_output_service={
	.guid     = &gEfiWrapperVNCGraphicsOutputProtocolGuid,
	.name     = "VNCGraphicsOutputProtocol",
	.provides = {
		&gEfiGraphicsOutputProtocolGuid,
		&gEfiAbsolutePointerProtocolGuid,
		&gEfiSimplePointerProtocolGuid,
		&gEfiSimpleTextInProtocolGuid,
		&gEfiSimpleTextInputExProtocolGuid,
		NULL
	},
	.after    = {
		&gEfiWrapperSDL2GraphicsOutputProtocolGuid,
		NULL
	},
	.depends  = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.condition = svc_condition,
	.start     = svc_start,
	.disabled  = true,
};

DECLARE_EFI_SERVICE(efi_vnc_graphics_output_service)

#else
efi_status efi_new_graphics_output_from_vnc(
	int port cdecl_attr_unused,
	efi_handle*handle cdecl_attr_unused,
	efi_graphics_output_protocol**proto cdecl_attr_unused
){
	return efi_unsupported;
}
#endif