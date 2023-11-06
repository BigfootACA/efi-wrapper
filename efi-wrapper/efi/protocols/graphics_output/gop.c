//
// Created by bigfoot on 2023/5/4.
//

#include"internal.h"

efiapi efi_status efi_gop_query_mode(
	efi_graphics_output_protocol          *proto,
	uint32_t                              num,
	uintn_t                               *size,
	efi_graphics_output_mode_information  **info
){
	log_call(
		call_arg(ptr,proto),
		call_arg(unsigned,num),
		call_arg(ptr,size),
		call_arg(ptr,info),
		NULL
	);
	const efi_graphics_output_mode_information*mode;
	if(size)*size=0;
	if(info)*info=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!proto||!size||!info||num>=efi_gop_mode.max_mode)
		log_return(efi_invalid_parameter);
	mode=&efi_gop_modes[num];
	if(!(*info=mem_allocate(
		efi_current_ctx->mem[efi_bs_data].pool,
		sizeof(efi_graphics_output_mode_information)
	)))log_return(efi_out_of_resources);
	*size=sizeof(efi_graphics_output_mode_information);
	memcpy(*info,mode,*size);
	log_return(efi_success);
}

efi_status efi_update_size(
	efi_graphics_output_protocol_private*info,
	efi_graphics_output_mode_information*ni,
	efi_graphics_output_mode_information*oi,
	void*buffer
){
	void*buf;
	efi_status st;
	fb_cfg*cfg=NULL;
	int prot,flags;
	size_t new_size,cs=0,i;
	if(!ni)return efi_invalid_parameter;
	new_size=ni->width*ni->height*sizeof(efi_graphics_output_blt_pixel);
	if(info->buffer&&new_size==info->size)buf=info->buffer;
	else if(!buffer){
		if(info->buffer&&!info->reallocate)return efi_unsupported;
		prot=PROT_READ|PROT_WRITE;
		flags=MAP_PRIVATE|MAP_ANONYMOUS;
		#ifdef MAP_32BIT
		flags|=MAP_32BIT;
		#endif
		buf=mmap(NULL,new_size,prot,flags,-1,0);
		if(!buf||buf==MAP_FAILED)return efi_out_of_resources;
		if(info->buffer)munmap(info->buffer,info->size);
		memset(buf,0,info->size);
	}else buf=buffer;
	if(
		!oi||buf!=info->buffer||
		oi->width!=ni->width||
		oi->height!=ni->height
	){
		for(i=0;i<10;i++){
			st=fb_blt_setup(buf,ni,cfg,&cs);
			if(st!=efi_buffer_too_small||cs<=0)break;
			if(cfg)mem_free(cfg);
			cfg=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,cs);
		}
		if(efi_error(st))return st;
		if(!cfg)return efi_out_of_resources;
		if(info->cfg)mem_free(info->cfg);
	}
	info->cfg=cfg;
	info->size=new_size;
	info->buffer=buf;
	info->mode.fb_base.ptr=buf;
	info->mode.fb_size=new_size;
	return efi_success;
}
