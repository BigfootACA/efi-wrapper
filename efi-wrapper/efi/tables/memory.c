#include"internal.h"

efiapi efi_status efi_allocate_pages_impl(
	efi_allocate_type type,
	efi_memory_type   memory_type,
	uintn_t           pages,
	efi_address       *memory
){
	log_call(
		call_arg(hex,type),
		call_arg(hex,memory_type),
		call_arg(unsigned,pages),
		call_arg(ptr,memory),
		NULL
	);
	mem_pool*pool;
	size_t mem=pages*PAGE_SIZE;
	if(!memory||pages<=0)log_return(efi_invalid_parameter);
	if(memory_type>=efi_max_memory_type)log_return(efi_invalid_parameter);
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!(pool=efi_current_ctx->mem[type].pool))log_return(efi_unsupported);
	memory->ptr=mem_aligned_allocate(pool,PAGE_SIZE,mem);
	xlog(LOG_CALL,"out memory=%p",memory->ptr);
	log_return(memory->ptr?efi_success:efi_out_of_resources);
}

efiapi efi_status efi_free_pages_impl(
	efi_address memory,
	uintn_t     pages
){
	log_call(
		call_arg(ptr,memory.ptr),
		call_arg(unsigned,pages),
		NULL
	);
	mem_pool*pool;
	mem_desc*desc;
	efi_memory_type type;
	if(!memory.ptr)log_return(efi_invalid_parameter);
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!(desc=mem_get_desc(memory.ptr)))log_return(efi_not_found);
	if(!(pool=desc->pool))log_return(efi_invalid_parameter);
	if(desc->align!=PAGE_SIZE)log_return(efi_invalid_parameter);
	if(desc->size/PAGE_SIZE!=pages)log_return(efi_invalid_parameter);
	if(pool->userdata[2]!=pool)log_return(efi_invalid_parameter);
	type=(uintptr_t)pool->userdata[0];
	if(type>=efi_max_memory_type)log_return(efi_invalid_parameter);
	if(efi_current_ctx->mem[type].pool!=pool)log_return(efi_invalid_parameter);
	mem_free(memory.ptr);
	log_return(efi_success);
}

efiapi efi_status efi_get_memory_map_impl(
	uintn_t               *map_size,
	efi_memory_descriptor *map,
	uintn_t               *key,
	uintn_t               *desc_size,
	uint32_t              *desc_ver
){
	log_call(
		call_arg(ptr,map_size),
		call_arg(ptr,map),
		call_arg(ptr,key),
		call_arg(ptr,desc_size),
		call_arg(ptr,desc_ver),
		NULL
	);
	size_t size=0;
	efi_memory_type type;
	if(key)*key=0;
	if(desc_ver)*desc_ver=1;
	if(desc_size){
		*desc_size=sizeof(efi_memory_descriptor);
		(*desc_size)+=sizeof(uint64_t)-(*desc_size%sizeof(uint64_t));
	}
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!map_size||!key||!desc_size||!desc_ver)
		log_return(efi_invalid_parameter);
	for(type=efi_reserved_memory_type;type<efi_max_memory_type;type++)
		if(efi_current_ctx->mem[type].pool)size+=*desc_size;
	if(size>*map_size){
		*map_size=size;
		log_return(efi_buffer_too_small);
	}
	if(!map)log_return(efi_invalid_parameter);
	*map_size=size;
	memset(map,0,size);
	for(type=efi_reserved_memory_type;type<efi_max_memory_type;type++){
		if(!efi_current_ctx->mem[type].pool)continue;
		memcpy(map,&efi_current_ctx->mem[type].map,sizeof(efi_memory_descriptor));
		map=(void*)map+*desc_size;
	}
	log_return(efi_success);
}

efiapi efi_status efi_allocate_pool_impl(
	efi_memory_type type,
	uintn_t         size,
	void            **buffer
){
	log_call(
		call_arg(hex,type),
		call_arg(unsigned,size),
		call_arg(ptr,buffer),
		NULL
	);
	mem_pool*pool;
	if(size<=0||!buffer||type>=efi_max_memory_type)log_return(efi_invalid_parameter);
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!(pool=efi_current_ctx->mem[type].pool))log_return(efi_unsupported);
	*buffer=mem_allocate(pool,size);
	xlog(LOG_CALL,"out buffer=%p",*buffer);
	log_return(efi_success);
}

efiapi efi_status efi_free_pool_impl(void*buffer){
	log_call(
		call_arg(ptr,buffer),
		NULL
	);
	mem_pool*pool;
	mem_desc*desc;
	efi_memory_type type;
	if(!buffer)log_return(efi_invalid_parameter);
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!(desc=mem_get_desc(buffer)))log_return(efi_not_found);
	if(!(pool=desc->pool))log_return(efi_invalid_parameter);
	if(pool->userdata[2]!=pool)log_return(efi_invalid_parameter);
	type=(uintptr_t)pool->userdata[0];
	if(type>=efi_max_memory_type)log_return(efi_invalid_parameter);
	if(efi_current_ctx->mem[type].pool!=pool)log_return(efi_invalid_parameter);
	mem_free(buffer);
	log_return(efi_success);
}

efiapi void efi_copy_mem_impl(
	void    *dst,
	void    *src,
	uintn_t len
){
	log_call(
		call_arg(ptr,dst),
		call_arg(ptr,src),
		call_arg(unsigned,len),
		NULL
	);
	memcpy(dst,src,(size_t)len);
	log_return_void();
}

efiapi void efi_set_mem_impl(
	void    *buffer,
	uintn_t size,
	uint8_t value
){
	log_call(
		call_arg(ptr,buffer),
		call_arg(unsigned,size),
		call_arg(hex,value),
		NULL
	);
	memset(buffer,value,(size_t)size);
	log_return_void();
}

efiapi efi_status efi_calculate_crc32_impl(
	void     *data,
	uintn_t  size,
	uint32_t *val
){
	log_call(
		call_arg(ptr,data),
		call_arg(unsigned,size),
		call_arg(ptr,val),
		NULL
	);
	if(!data||size<=0||!val)
		log_return(efi_invalid_parameter);
	*val=s_crc32(data,(size_t)size);
	log_return(efi_success);
}
