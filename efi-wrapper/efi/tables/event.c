#include"internal.h"

static void efi_run_event_queue(efi_run_context*ctx){
	list*l,*n;
	uintn_t old_tpl;
	if((l=list_first(ctx->event_queue)))do{
		n=l->next;
		LIST_DATA_DECLARE(event,l,efi_event);
		if(!efi_valid_event(event))continue;
		if(ctx->tpl>=event->tpl)return;
		old_tpl=ctx->tpl,ctx->tpl=event->tpl;
		event->func(event,event->context);
		if(ctx->tpl==event->tpl)ctx->tpl=old_tpl;
		if(event->signaled)list_obj_del(&ctx->event_queue,l,NULL);
		if(event->type==EVT_NOTIFY_SIGNAL)event->signaled=false;
	}while((l=n));
}

static void efi_add_event_queue(efi_run_context*ctx,efi_event event){
	list*l,*a;
        if(!efi_valid_event(event)||!event->func||list_search_one(
		ctx->event_queue,list_pointer_comparator,event
	)||!(a=list_new(efi_current_ctx->mem[efi_bs_data].pool,event)))return;
	if(ctx->event_queue&&(l=list_first(ctx->event_queue)))do{
		LIST_DATA_DECLARE(i,l,efi_event);
		if(!i||i->tpl>=event->tpl)continue;
		list_insert(l,a);
		a=NULL;
		break;
	}while((l=l->next));
	if(a)list_obj_add(&ctx->event_queue,a);
	efi_run_event_queue(ctx);
}

static inline uint64_t get_cur_us(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000000+tv.tv_usec;
}

static void efi_timer_run(efi_run_context*ctx){
	list*l;
	uint64_t now=get_cur_us();
	if(!ctx)return;
	if(ctx->timer&&(l=list_first(ctx->events)))do{
		LIST_DATA_DECLARE(e,l,efi_event);
		if(!e||!(e->type&EVT_TIMER))continue;
		if(now<e->next)continue;
		switch(e->delay){
			case timer_periodic:e->next+=e->time;break;
			case timer_relative:e->delay=timer_cancel;break;
			default:continue;
		}
		e->signaled=false;
		efi_signal_event_impl(e);
	}while((l=l->next));
        efi_run_event_queue(ctx);
}

static inline bool check_tpl(efi_tpl tpl){
	switch(tpl){
	        case TPL_APPLICATION:
	        case TPL_CALLBACK:
		case TPL_NOTIFY:return true;
	        default:return false;
        }
}
static inline bool check_event_type(uint32_t type){
        switch(type){
	        case 0:
	        case EVT_TIMER:
	        case EVT_NOTIFY_SIGNAL:
	        case EVT_TIMER|EVT_NOTIFY_SIGNAL:
	        case EVT_NOTIFY_WAIT:
	        case EVT_TIMER|EVT_NOTIFY_WAIT:
	        case EVT_SIGNAL_EXIT_BOOT_SERVICES:
	        case EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE:return true;
	        default:return false;
        }
}

efiapi efi_status efi_create_event_ex_impl(
	uint32_t         type,
	efi_tpl          tpl,
	efi_event_notify func,
	const void       *context,
	const efi_guid   *group,
	efi_event        *event
){
	log_call(
		call_arg(hex,type),
		call_arg(hex,tpl),
		call_arg(ptr,func),
		call_arg(ptr,context),
		call_arg(guid,group),
		call_arg(ptr,event),
		NULL
	);
	if(event)*event=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!event||!check_event_type(type))
		log_return(efi_invalid_parameter);
	if(
		(type&(EVT_NOTIFY_WAIT|EVT_NOTIFY_SIGNAL))&&
		(!func||!check_tpl(tpl)||tpl==TPL_APPLICATION)
	)log_return(efi_invalid_parameter);
	switch(type){
		case EVT_SIGNAL_EXIT_BOOT_SERVICES:
			if(group)log_return(efi_invalid_parameter);
			group=&gEfiEventExitBootServicesGuid;
		break;
		case EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE:
			if(group)log_return(efi_invalid_parameter);
			group=&gEfiEventVirtualAddressChangeGuid;
		break;
	}
	if(!(*event=mem_allocate(
		efi_current_ctx->mem[efi_bs_data].pool,
		sizeof(struct efi_event)
	)))log_return(efi_out_of_resources);
	memset(*event,0,sizeof(struct efi_event));
	(*event)->sig=EFI_EVENT_SIGN;
	(*event)->type=type;
	(*event)->tpl=tpl;
	(*event)->func=func;
	(*event)->group=group;
	(*event)->context=(void*)context;
	(*event)->next=-1;
	list_obj_add_new(
		efi_current_ctx->mem[efi_bs_data].pool,
		&efi_current_ctx->events,
		*event
	);
	log_return(efi_success);
}

efiapi efi_status efi_create_event_impl(
	uint32_t         type,
	efi_tpl          tpl,
	efi_event_notify func,
	void             *context,
	efi_event        *event
){
	log_call(
		call_arg(hex,type),
		call_arg(hex,tpl),
		call_arg(ptr,func),
		call_arg(ptr,context),
		call_arg(ptr,event),
		NULL
	);
	log_return(efi_create_event_ex_impl(
		type,tpl,func,context,NULL,event
	));
}

efiapi efi_status efi_signal_event_impl(
	efi_event event
){
	log_call(
		call_arg(ptr,event),
		NULL
	);
	list*l;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_event(event))
		log_return(efi_invalid_parameter);
	if(event->signaled)log_return(efi_success);
	if(event->group){
		if((l=list_first(efi_current_ctx->events)))do{
			LIST_DATA_DECLARE(e,l,efi_event);
			if(e&&e->group&&efi_guid_equals(
				e->group,event->group
			))e->signaled=true;
		}while((l=l->next));
		if((l=list_first(efi_current_ctx->events)))do{
			LIST_DATA_DECLARE(e,l,efi_event);
			if(e&&e->group&&efi_guid_equals(
				e->group,event->group
			))efi_add_event_queue(
				efi_current_ctx,e
			);
		}while((l=l->next));
	}else{
		event->signaled=true;
		efi_add_event_queue(efi_current_ctx,event);
	}
	log_return(efi_success);
}

efiapi efi_status efi_wait_for_event_impl(
	uintn_t   events,
	efi_event *event,
	uintn_t   *index
){
	log_call(
		call_arg(unsigned,events),
		call_arg(ptr,event),
		call_arg(ptr,index),
		NULL
	);
	if(index)*index=0;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(events<=0||!event)log_return(efi_invalid_parameter);
	if(efi_current_ctx->tpl!=TPL_APPLICATION)
		log_return(efi_unsupported);
	for(uintn_t i=0;i<events;i++){
		if(!efi_valid_event(event[i]))
			log_return(efi_invalid_parameter);
		if(event[i]->type==0||event[i]->type&EVT_NOTIFY_SIGNAL)
			log_return(efi_invalid_parameter);
		if(!event[i]->signaled)efi_add_event_queue(
			efi_current_ctx,event[i]
		);
	}
	while(true){
                for(uintn_t i=0;i<events;i++){
                        if(!event[i]->signaled)continue;
			event[i]->signaled=false;
			if(index)*index=i;
	                log_return(efi_success);
                }
                usleep(10000);
		efi_timer_run(efi_current_ctx);
        }
	log_return(efi_not_ready);
}

static bool notify_event_cmp(list*l,void*d){
	LIST_DATA_DECLARE(n,l,efi_register_notify*);
	return n&&d&&n->event==d;
}

efiapi efi_status efi_close_event_impl(
	efi_event event
){
	log_call(
		call_arg(ptr,event),
		NULL
	);
	list*l,*n;
	if(efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_event(event))log_return(efi_invalid_parameter);
	if((l=list_first(efi_current_ctx->notifies)))do{
		n=l->next;
		LIST_DATA_DECLARE(notify,l,efi_register_notify*);
		if(!notify||notify->event!=event)continue;
		list_free_all(notify->handles,NULL);
		list_obj_del(&efi_current_ctx->notifies,l,NULL);
		memset(notify,0,sizeof(efi_register_notify));
		mem_free(notify);
	}while((l=n));
	list_obj_del_data(&efi_current_ctx->events,event,NULL);
	list_obj_del_data(&efi_current_ctx->event_queue,event,NULL);
	memset(event,0,sizeof(struct efi_event));
	mem_free(event);
	log_return(efi_success);
}

efiapi efi_status efi_check_event_impl(
	efi_event event
){
	log_call(
		call_arg(ptr,event),
		NULL
	);
	if(efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_event(event)||event->type&EVT_NOTIFY_SIGNAL)
		log_return(efi_invalid_parameter);
	if(!event->signaled)efi_add_event_queue(efi_current_ctx,event);
	if(event->signaled){
		event->signaled=false;
		log_return(efi_success);
	}
	log_return(efi_not_ready);
}

efiapi efi_status efi_set_timer_impl(
	efi_event       event,
	efi_timer_delay type,
	uint64_t        time
){
	log_call(
		call_arg(ptr,event),
		call_arg(hex,type),
		call_arg(unsigned,time),
		NULL
	);
	if(efi_current_ctx)log_return(efi_not_ready);
	if(!efi_valid_event(event)||!(event->type&EVT_TIMER))
		log_return(efi_invalid_parameter);
	time/=10;
        switch(type){
		case timer_cancel:event->next=-1;break;
		case timer_periodic:
		case timer_relative:
			event->next=get_cur_us()+time;
		break;
		default:log_return(efi_invalid_parameter);
        }
	event->delay=type;
	event->time=time;
	event->signaled=false;
	log_return(efi_success);
}

efiapi efi_tpl efi_raise_tpl_impl(
	efi_tpl new_tpl
){
	log_call(
		call_arg(hex,new_tpl),
		NULL
	);
	uintn_t tpl;
	if(!efi_current_ctx)log_return_unsigned(efi_tpl,0);
	tpl=efi_current_ctx->tpl;
	if(new_tpl<tpl)xerror("raise_tpl: new tpl < current tpl");
	if(new_tpl>TPL_HIGH_LEVEL)new_tpl=TPL_HIGH_LEVEL;
	xlog(LOG_CALL,"change tpl from 0x%02zX to 0x%02zX",tpl,new_tpl);
	efi_current_ctx->tpl=new_tpl;
	log_return_unsigned(efi_tpl,tpl);
}

efiapi void efi_restore_tpl_impl(
	efi_tpl old_tpl
){
	log_call(
		call_arg(hex,old_tpl),
		NULL
	);
	efi_tpl tpl;
	if(!efi_current_ctx)return;
	tpl=efi_current_ctx->tpl;
	if(old_tpl>tpl)xerror("restore_tpl: old tpl > current tpl");
	if(old_tpl>TPL_HIGH_LEVEL)old_tpl=TPL_HIGH_LEVEL;
	xlog(LOG_CALL,"change tpl from 0x%02zX to 0x%02zX",tpl,old_tpl);
	efi_current_ctx->tpl=old_tpl;
	efi_timer_run(efi_current_ctx);
	log_return_void();
}

efiapi efi_status efi_register_protocol_notify_impl(
	const efi_guid*protocol,
	efi_event event,
	void**registration
){
	log_call(
		call_arg(guid,protocol),
		call_arg(ptr,event),
		call_arg(ptr,registration),
		NULL
	);
	efi_register_notify*item;
	if(registration)*registration=NULL;
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!protocol||!efi_valid_event(event)||!registration)
		log_return(efi_invalid_parameter);
	if(!(item=mem_allocate(
		efi_current_ctx->mem[efi_bs_data].pool,
		sizeof(efi_register_notify)
	)))log_return(efi_out_of_resources);
	memset(item,0,sizeof(efi_register_notify));
	memcpy(&item->protocol,protocol,sizeof(efi_guid));
	item->event=event;
	list_obj_add_new(
		efi_current_ctx->mem[efi_bs_data].pool,
		&efi_current_ctx->notifies,item
	);
	*registration=item;
	log_return(efi_success);
}

efiapi efi_status efi_stall_impl(
	uintn_t microseconds
){
	log_call(
		call_arg(unsigned,microseconds),
		NULL
	);
	usleep(microseconds);
	efi_timer_run(efi_current_ctx);
	log_return(efi_success);
}
