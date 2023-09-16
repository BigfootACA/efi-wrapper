#include<unistd.h>
#include<sys/mman.h>
#include"mem.h"
#include"defines.h"
#include"efi/efi_hii.h"
#include"efi/efi_context.h"
#include"efi/efi_template.h"

efi_run_context*efi_current_ctx=NULL;

static inline bool tslf_mem_check(mem_pool*pool){
	if(!pool)return false;
	if(pool->userdata[2]!=pool)return false;
	return true;
}

static void*tlsf_mem_allocate(mem_pool*pool,size_t len){
	if(!tslf_mem_check(pool)||len<=0)return NULL;
	return tlsf_malloc(pool->userdata[1],len);
}

static void*tlsf_mem_reallocate(mem_pool*pool,void*ptr,size_t len){
	if(!tslf_mem_check(pool)||len<=0)return NULL;
	if(!ptr)return tlsf_mem_allocate(pool,len);
	return tlsf_realloc(pool->userdata[1],ptr,len);
}

static void tlsf_mem_free(mem_pool*pool,void*ptr){
	if(!tslf_mem_check(pool)||!ptr)return;
	tlsf_free(pool->userdata[1],ptr);
}

static efi_status init_pool(
	mem_pool*parent cdecl_attr_unused,
	efi_run_context*ctx,
	efi_memory_type type,
	size_t length
){
	char buff[64];
	efi_status st;
	if(!ctx||type>=efi_max_memory_type)
		return efi_invalid_parameter;
	efi_context_mem*mem=&ctx->mem[type];
	memset(mem,0,sizeof(efi_context_mem));
	mem->size=round_up(length,PAGE_SIZE);
	if(!(mem->pool=mem_new_pool())){
		xerror("create memory pool failed");
		EDONE(st=efi_out_of_resources);
	}
	if(!(mem->pointer=mmap(
		NULL,mem->size,
		PROT_READ|PROT_WRITE|PROT_EXEC,
		MAP_PRIVATE|MAP_32BIT|MAP_ANONYMOUS,-1,0
	))||mem->pointer==MAP_FAILED){
		xerror(
			"allocate %zu bytes memory region failed: %s",
			mem->size,strerror(errno)
		);
		EDONE(st=efi_out_of_resources);
	}
	if(!(mem->tlsf_ctl=tlsf_create(mem->pointer))){
		xerror("create tlsf memory pool failed");
		EDONE(st=efi_out_of_resources);
	}
	if(!(mem->tlsf_pool=tlsf_add_pool(
		mem->tlsf_ctl,
		mem->pointer+tlsf_size(),
		mem->size-tlsf_size()
	))){
		xerror("add tlsf memory pool failed");
		EDONE(st=efi_out_of_resources);
	}
	mem->map.type=type;
	mem->map.physical_start.ptr=mem->pointer;
	mem->map.pages=mem->size/PAGE_SIZE;
	mem->map.attribute=0xF;
	mem->pool->userdata[0]=(void*)(uintptr_t)type;
	mem->pool->userdata[1]=mem->tlsf_ctl;
	mem->pool->userdata[2]=mem->pool;
	mem->pool->allocate=tlsf_mem_allocate;
	mem->pool->reallocate=tlsf_mem_reallocate;
	mem->pool->free=tlsf_mem_free;
	mprotect(
		mem->pointer,mem->size,
		PROT_READ|PROT_WRITE|PROT_EXEC
	);
	xlog(LOG_INFO,
		"Allocate %zu bytes (%s) %s memory pool at %p",
		mem->size,make_readable_str_buf(buff,sizeof(buff),mem->size,1,0),
		efi_memory_type_to_string(type),mem->pointer
	);
	return efi_success;
	done:
	if(mem->tlsf_pool)tlsf_remove_pool(mem->tlsf_ctl,mem->tlsf_pool);
	if(mem->tlsf_ctl)tlsf_destroy(mem->tlsf_ctl);
	if(mem->pointer)munmap(mem->pointer,mem->size);
	return st;
}

static efi_status init_pools(efi_run_context*ctx){
	efi_status st=efi_success;
	if(!efi_error(st))st=init_pool(NULL,ctx,efi_bs_code,EFI_BS_CODE_POOL_SIZE);
	if(!efi_error(st))st=init_pool(NULL,ctx,efi_bs_data,EFI_BS_DATA_POOL_SIZE);
	if(!efi_error(st))st=init_pool(NULL,ctx,efi_loader_code,EFI_LD_CODE_POOL_SIZE);
	if(!efi_error(st))st=init_pool(NULL,ctx,efi_loader_data,EFI_LD_DATA_POOL_SIZE);
	if(!efi_error(st))st=init_pool(NULL,ctx,efi_conventional_memory,EFI_CONV_POOL_SIZE);
	return st;
}

efi_run_context*efi_new_run_context(){
	mem_pool*pool;
	efi_run_context template,*ctx=NULL;
	memset(&template,0,sizeof(efi_run_context));
	if(efi_error(init_pools(&template)))goto done;
	if(!(pool=template.mem[efi_bs_data].pool))goto done;
	if(!(ctx=mem_allocate(pool,sizeof(template))))goto done;
	memcpy(ctx,&template,sizeof(template));
	ctx->tpl=TPL_APPLICATION;
	xlog(LOG_INFO,"Create new efi context %p",ctx);
	efi_service_auto_start(ctx);
	if(!ctx->bs||!ctx->rt||!ctx->st||!ctx->ds)
		EDONE(xerror("efi runtime not initialized"));
	return ctx;
	done:
	if(ctx)mem_free(ctx);
	return NULL;
}

void efi_destroy_context(efi_run_context*ctx){
	if(!ctx)return;
	efi_service_auto_stop(ctx);
	if(ctx==efi_current_ctx)efi_current_ctx=NULL;
}