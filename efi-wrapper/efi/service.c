//
// Created by bigfoot on 2023/5/12.
//

#include"efi/efi_context.h"

static list*efi_services=NULL;
static list*efi_service_cfgs=NULL;

static bool svc_valid(const efi_service*svc){
	return svc&&svc->name[0]&&svc->guid;
}

static bool svc_cfg_valid(const efi_service_cfg*cfg){
	return cfg&&cfg->name[0]&&svc_valid(cfg->service);
}

static bool svc_ins_valid(const efi_service_instance*ins){
	return
		ins&&ins->cfg&&ins->service&&ins->service==ins->cfg->service&&
		svc_valid(ins->service)&&svc_cfg_valid(ins->cfg);
}

static bool svc_is_guid(const efi_service*svc,const efi_guid*g){
	if(!svc_valid(svc)||!g)return false;
	if(efi_guid_equals(g,svc->guid))return true;
	for(size_t i=0;svc->provides[i]&&i<ARRAYLEN(svc->provides);i++)
		if(efi_guid_equals(g,svc->provides[i]))return true;
	return false;
}

static bool svc_ins_is_guid(const efi_service_instance*ins,const efi_guid*g){
	if(!svc_ins_valid(ins)||!g)return false;
	return svc_is_guid(ins->service,g);
}

static bool svc_cmp(const efi_service*svc1,const efi_service*svc2){
	if(!svc_valid(svc1)||!svc_valid(svc2))return false;
	if(svc1==svc2)return true;
	if(efi_guid_equals(svc1->guid,svc2->guid))return true;
	if(strncmp(svc1->name,svc2->name,sizeof(svc1->name))==0)return true;
	return false;
}

static bool svc_cfg_cmp(const efi_service_cfg*cfg1,const efi_service_cfg*cfg2){
	if(!svc_cfg_valid(cfg1)||!svc_cfg_valid(cfg2))return false;
	if(cfg1==cfg2)return true;
	if(!svc_cmp(cfg1->service,cfg2->service))return false;
	if(strncmp(cfg1->name,cfg2->name,sizeof(cfg1->name))!=0)return false;
	return true;
}

static bool svc_ins_cmp(const efi_service_instance*ins1,const efi_service_instance*ins2){
	if(!svc_ins_valid(ins1)||!svc_ins_valid(ins2))return false;
	if(ins1==ins2)return true;
	if(!svc_cmp(ins1->service,ins2->service))return false;
	if(!svc_cfg_cmp(ins1->cfg,ins2->cfg))return false;
	return true;
}

static bool lst_svc_cmp(list*l,void*d){return svc_cmp(LIST_DATA(l,efi_service*),d);}
static bool lst_svc_cfg_cmp(list*l,void*d){return svc_cfg_cmp(LIST_DATA(l,efi_service_cfg*),d);}
static bool lst_svc_ins_cmp(list*l,void*d){return svc_ins_cmp(LIST_DATA(l,efi_service_instance*),d);}
static bool lst_svc_guid_cmp(list*l,void*d){return svc_is_guid(LIST_DATA(l,efi_service*),d);}
static bool lst_svc_ins_guid_cmp(list*l,void*d){return svc_ins_is_guid(LIST_DATA(l,efi_service_instance*),d);}

bool efi_service_is_svc_started(
	efi_run_context*ctx,
	const efi_guid*guid
){
	list*l;
	if(!ctx||!guid)return false;
	if((l=list_first(ctx->svc_ins)))do{
		LIST_DATA_DECLARE(ins,l,efi_service_instance*);
		if(!svc_ins_valid(ins)||!lst_svc_ins_guid_cmp(l,(void*)guid))continue;
		switch(ins->status){
			case service_started:return true;
			default:continue;
		}
	}while((l=l->next));
	return false;
}

bool efi_service_is_svc_tried(
	efi_run_context*ctx,
	const efi_guid*guid
){
	list*l;
	if(!ctx||!guid)return false;
	if((l=list_first(ctx->svc_ins)))do{
		LIST_DATA_DECLARE(ins,l,efi_service_instance*);
		if(!svc_ins_valid(ins)||!lst_svc_ins_guid_cmp(l,(void*)guid))continue;
		switch(ins->status){
			case service_skip:
			case service_failed:
			case service_conditioning:
			case service_stopping:
			case service_starting:
			case service_started:return true;
			default:continue;
		}
	}while((l=l->next));
	return false;
}

bool efi_service_is_svc_depends_done(
	efi_run_context*ctx,
	const efi_service*svc
){
	if(!ctx||!svc)return false;
	for(size_t i=0;svc->depends[i]&&i<ARRAYLEN(svc->depends);i++)
		if(!efi_service_is_svc_started(ctx,svc->depends[i]))return false;
	for(size_t i=0;svc->after[i]&&i<ARRAYLEN(svc->after);i++)
		if(!efi_service_is_svc_tried(ctx,svc->after[i]))return false;
	return true;
}

efi_status efi_service_run_instance_condition(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st;
	if(!ctx||!svc_ins_valid(ins))return efi_invalid_parameter;
	if(!ins->service->condition)return efi_success;
	switch(ins->status){
		case service_skip:
		case service_failed:
		case service_stopped:break;
		default:return efi_not_ready;
	}
	ins->status=service_conditioning;
	st=ins->service->condition(ctx,ins);
	ins->status=efi_error(st)?service_skip:service_stopped;
	return st;
}

efi_status efi_service_run_instance_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st=efi_success;
	if(!ctx||!svc_ins_valid(ins))return efi_invalid_parameter;
	switch(ins->status){
		case service_stopped:break;
		case service_failed:
			if(ins->service->restart)break;
		//fallthrough
		case service_started:return efi_already_started;
		default:return efi_not_ready;
	}
	ins->status=service_starting;
	if(ins->service->start)
		st=ins->service->start(ctx,ins);
	ins->status=efi_error(st)?service_failed:service_started;
	return st;
}

efi_status efi_service_run_instance_stop(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st=efi_success;
	if(!ctx||!svc_ins_valid(ins))return efi_invalid_parameter;
	switch(ins->status){
		case service_started:break;
		default:return efi_not_ready;
	}
	ins->status=service_stopping;
	if(ins->service->stop)
		st=ins->service->stop(ctx,ins);
	ins->status=efi_error(st)?service_failed:service_stopped;
	return st;
}

bool efi_service_can_instance_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st;
	if(!ctx||!svc_ins_valid(ins)||ins->service->disabled)return false;
	if(!efi_service_is_svc_depends_done(ctx,ins->service))return false;
	st=efi_service_run_instance_condition(ctx,ins);
	if(efi_error(st))return false;
	switch(ins->status){
		case service_failed:return ins->service->restart;
		case service_stopped:return true;
		default:return false;
	}
}

bool efi_service_can_instance_stop(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	list*l;
	if(!ctx||!svc_ins_valid(ins)||ins->service->disabled)return false;
	if(ins->status!=service_started)return false;
	if((l=list_first(ctx->svc_ins)))do{
		LIST_DATA_DECLARE(x,l,efi_service_instance*);
		if(!svc_ins_valid(x)||x->status!=service_started||x->service->disabled)continue;
		if(efi_guid_equals(x->service->guid,ins->service->guid))continue;
		for(size_t i=0;x->service->depends[i]&&i<ARRAYLEN(x->service->depends);i++)
			if(svc_is_guid(ins->service,x->service->depends[i]))return true;
	}while((l=l->next));
	return false;
}

efi_service*efi_service_lookup_service(const efi_guid*guid){
	list*l;
	efi_service*svc=NULL;
	if(!guid)return NULL;
	if((l=list_search_one(
		efi_services,lst_svc_guid_cmp,(void*)guid
	)))svc=LIST_DATA(l,efi_service*);
	return svc;
}

efi_service_instance*efi_service_lookup_instance(
	efi_run_context*ctx,
	const efi_guid*guid
){
	list*l;
	efi_service_instance*ins=NULL;
	if(!ctx||!guid)return NULL;
	if((l=list_search_one(
		ctx->svc_ins,lst_svc_ins_guid_cmp,(void*)guid
	)))ins=LIST_DATA(l,efi_service_instance*);
	return ins;
}

void efi_service_install_all(efi_run_context*ctx){
	list*l;
	efi_service_instance ins;
	if(!ctx)return;
	if((l=list_first(efi_service_cfgs)))do{
		LIST_DATA_DECLARE(cfg,l,efi_service_cfg*);
		if(!svc_cfg_valid(cfg))continue;
		memset(&ins,0,sizeof(ins));
		ins.service=cfg->service,ins.cfg=cfg;
		if(list_search_one(ctx->svc_ins,lst_svc_ins_cmp,&ins))continue;
		if(ins.service->cust_len>0){
			if(!(ins.cust_data=mem_aligned_allocate(
				ctx->mem[efi_bs_data].pool,
				ins.service->cust_align,
				ins.service->cust_len
			)))continue;
			memset(ins.cust_data,0,ins.service->cust_len);
			if(ins.service->cust_init)memcpy(
				ins.cust_data,ins.service->cust_init,
				ins.service->cust_len
			);
		}
		ins.status=service_stopped;
		list_obj_add_new_dup(
			ctx->mem[efi_bs_data].pool,
			&ctx->svc_ins,
			&ins,sizeof(ins)
		);
	}while((l=l->next));
}

void efi_service_auto_start(efi_run_context*ctx){
	list*l;
	efi_status st;
	bool switch_context=false;
	efi_run_context*old=NULL;
	size_t cnt=0,round;
	efi_service_install_all(ctx);
	xlog(LOG_INFO,"starting all service for efi context");
	if(ctx!=efi_current_ctx)switch_context=true;
	if(switch_context)old=efi_current_ctx,efi_current_ctx=ctx;
	do{
		round=0;
		if((l=list_first(ctx->svc_ins)))do{
			LIST_DATA_DECLARE(ins,l,efi_service_instance*);
			if(!efi_service_can_instance_start(ctx,ins))continue;
			if(!ins->handle)ins->handle=efi_new_handle(ctx);
			xlog(LOG_INFO,"starting service %s@%s",ins->service->name,ins->cfg->name);
			st=efi_service_run_instance_start(ctx,ins),round++,cnt++;
			if(efi_error(st))xerror(
				"start service %s@%s failed: %s",
				ins->service->name,ins->cfg->name,
				efi_status_to_string(st)
			);
		}while((l=l->next));
	}while(round>0);
	if(switch_context&&efi_current_ctx==ctx)efi_current_ctx=old;
	xlog(LOG_INFO,"started %zu services",cnt);
}

void efi_service_auto_stop(efi_run_context*ctx){
	list*l;
	efi_status st;
	bool switch_context=false;
	efi_run_context*old=NULL;
	size_t cnt=0,round;
	efi_service_install_all(ctx);
	xlog(LOG_INFO,"stopping all service for efi context");
	if(ctx!=efi_current_ctx)switch_context=true;
	if(switch_context)old=efi_current_ctx,efi_current_ctx=ctx;
	do{
		round=0;
		if((l=list_first(ctx->svc_ins)))do{
			LIST_DATA_DECLARE(ins,l,efi_service_instance*);
			if(!efi_service_can_instance_stop(ctx,ins))continue;
			xlog(LOG_INFO,"stopping service %s@%s",ins->service->name,ins->cfg->name);
			st=efi_service_run_instance_stop(ctx,ins),round++,cnt++;
			if(efi_error(st))xerror(
				"stop service %s@%s failed: %s",
				ins->service->name,ins->cfg->name,
				efi_status_to_string(st)
			);
		}while((l=l->next));
	}while(round>0);
	if(switch_context&&efi_current_ctx==ctx)efi_current_ctx=old;
	xlog(LOG_INFO,"stopped %zu services",cnt);
}

void*efi_service_get_instance_cust_data(
	efi_run_context*ctx,
	const efi_guid*guid
){
	efi_service_instance*ins=efi_service_lookup_instance(ctx,guid);
	return ins?ins->cust_data:NULL;
}

void efi_service_config_register(const efi_service_cfg*cfg){
	list*l;
	efi_service*svc;
	if(!svc_cfg_valid(cfg))EDONE(xerror("invalid service config"));
	if(!(l=list_search_one(efi_services,lst_svc_cmp,(void*)cfg->service))){
		svc=mem_memdup(NULL,cfg->service,sizeof(efi_service));
		if(!svc)EDONE(xerror("dup service failed"));
		list_obj_add_new(NULL,&efi_services,svc);
	}else svc=LIST_DATA(l,efi_service*);
	if(!list_search_one(efi_service_cfgs,lst_svc_cfg_cmp,(void*)cfg)){
		efi_service_cfg*dup=mem_memdup(NULL,cfg,sizeof(efi_service_cfg));
		if(!dup)EDONE(xerror("dup service config failed"));
		dup->service=svc;
		list_obj_add_new(NULL,&efi_service_cfgs,dup);
	}
	return;
	done:abort();
}
