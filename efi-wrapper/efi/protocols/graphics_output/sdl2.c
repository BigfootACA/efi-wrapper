//
// Created by bigfoot on 2023/5/3.
//
#include"internal.h"
#include"efi/protocols/absolute_pointer.h"
#include"efi/protocols/simple_pointer.h"
#include"efi/protocols/simple_text_input.h"
#include"efi/protocols/simple_text_input_ex.h"
#ifdef SDL2_FOUND
#include<SDL.h>
#include<semaphore.h>
#include<pthread.h>

typedef struct efi_graphics_output_protocol_private_sdl2{
	efi_graphics_output_protocol_private data;
	struct{
		struct{
			efi_simple_text_input_protocol protocol;
			efi_guid magic;
		}kbd;
		struct{
			efi_simple_text_input_ex_protocol protocol;
			efi_guid magic;
		}kbd_ex;
		struct{
			efi_absolute_pointer_protocol protocol;
			efi_guid magic;
			efi_absolute_pointer_mode mode;
			efi_absolute_pointer_state state;
		}abs;
		struct{
			efi_simple_pointer_protocol protocol;
			efi_guid magic;
			efi_simple_pointer_mode mode;
			efi_simple_pointer_state state;
		}simple;
	}in;
	bool show;
	sem_t update;
	pthread_mutex_t fb_lock;
	pthread_t update_tid;
	SDL_Window*window;
	SDL_Renderer*renderer;
	SDL_Texture*texture;
	list*event_queue;
}efi_graphics_output_protocol_private_sdl2;

static struct sdl2_device_path{
	vendor_device_path vendor;
	efi_device_path end;
}dp={
	{
		{
			HARDWARE_DEVICE_PATH,
			HW_VENDOR_DP,
			(uint16_t)(sizeof(vendor_device_path)),
		},
		EFI_WRAPPER_SDL2_GRAPHICS_OUTPUT_PROTOCOL_GUID
	},{
		END_DEVICE_PATH_TYPE,
		END_ENTIRE_DEVICE_PATH_SUBTYPE,
		(uint16_t)sizeof(efi_device_path),
	}
};

static efiapi efi_status sdl2_get_driver_name(
	efi_component_name_protocol *proto,
	char8                       *lang,
	char16                      **name
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str8,lang),
		call_arg(str16,name),
		NULL
	);
	static char16*def_name=(char16*)L"SDL2 Graphics";
	if(name)*name=def_name;
	log_return(efi_success);
}

static efiapi efi_status sdl2_get_controller_name(
	efi_component_name_protocol *proto,
	efi_handle                  controller,
	efi_handle                  child,
	char8                       *lang,
	char16                      **name
){
	log_call(
		call_arg(ptr,proto),
		call_arg(handle,controller),
		call_arg(handle,child),
		call_arg(str8,lang),
		call_arg(str16,name),
		NULL
	);
	static char16*def_name=(char16*)L"SDL2 Graphics";
	if(name)*name=def_name;
	log_return(efi_success);
}

static efiapi efi_status sdl2_gop_set_mode(
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
	efi_graphics_output_protocol_private_sdl2*p=(void*)proto-
		offsetof(efi_graphics_output_protocol_private_sdl2,data.protocol);
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!proto||num>=efi_gop_mode.max_mode)
		log_return(efi_invalid_parameter);
	if(!efi_guid_equals(&p->data.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid))
		log_return(efi_invalid_parameter);
	if(!p->data.reallocate)log_return(efi_unsupported);
	pthread_mutex_lock(&p->fb_lock);
	mode=&efi_gop_modes[num];
	st=efi_update_size(&p->data,mode,p->data.cur,NULL);
	if(!efi_error(st)){
		p->data.mode.mode=num;
		p->data.mode.info=mode;
		p->data.cur=mode;
	}
	p->in.simple.mode.res_x=p->data.cur->width;
	p->in.simple.mode.res_y=p->data.cur->height;
	SDL_SetRenderTarget(p->renderer,NULL);
	SDL_SetWindowSize(
		p->window,
		p->data.cur->width,
		p->data.cur->height
	);
	p->texture=SDL_CreateTexture(
		p->renderer,
		SDL_PIXELFORMAT_XBGR8888,
		SDL_TEXTUREACCESS_STATIC,
		p->data.cur->width,
		p->data.cur->height
	);
	SDL_SetRenderTarget(p->renderer,p->texture);
	SDL_SetTextureBlendMode(
		p->texture,
		SDL_BLENDMODE_BLEND
	);
	if(!p->show){
		p->show=true;
		SDL_ShowWindow(p->window);
		SDL_RaiseWindow(p->window);
		SDL_StartTextInput();
	}
	pthread_mutex_unlock(&p->fb_lock);
	log_return(st);
}

static efiapi efi_status sdl2_gop_blt(
	efi_graphics_output_protocol  *proto,
	efi_graphics_output_blt_pixel *buffer,
	efi_graphics_output_blt_operation oper,
	uintn_t src_x,uintn_t src_y,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height,
	uintn_t delta
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
	int v=0;
	efi_status st;
	efi_graphics_output_protocol_private_sdl2*p=(void*)proto-
		offsetof(efi_graphics_output_protocol_private_sdl2,data.protocol);
	if(!efi_current_ctx)log_return(efi_not_ready);
	if(!proto||!efi_guid_equals(&p->data.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid))
		log_return(efi_invalid_parameter);
	if(!p->data.cfg)log_return(efi_not_ready);
	pthread_mutex_lock(&p->fb_lock);
	if(!p->show){
		p->show=true;
		SDL_ShowWindow(p->window);
		SDL_RaiseWindow(p->window);
		SDL_StartTextInput();
	}
	st=fb_blt(
		p->data.cfg,buffer,oper,
		src_x,src_y,dst_x,dst_y,
		width,height,delta
	);
	if(!efi_error(st)&&sem_getvalue(&p->update,&v)==0&&v==0)
		sem_post(&p->update);
	pthread_mutex_unlock(&p->fb_lock);
	log_return(st);
}

#define DECL_PROTO_GET(_name)\
	efi_graphics_output_protocol_private_sdl2*p=(void*)proto-\
		offsetof(efi_graphics_output_protocol_private_sdl2,in._name.protocol);\
	if(!efi_current_ctx)log_return(efi_not_ready);\
	if(\
		!proto||\
		!efi_guid_equals(&p->data.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid)||\
		!efi_guid_equals(&p->in._name.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid)\
	)log_return(efi_invalid_parameter);


#define DECL_RESET(_type,_name)\
	static efiapi efi_status sdl2_##_type##_reset(\
		efi_##_type##_protocol*proto,\
		bool ext_verify\
	){\
		log_call(\
			call_arg(ptr,proto),\
			call_arg(bool,ext_verify),\
			NULL\
		);\
		DECL_PROTO_GET(_name)\
		pthread_mutex_lock(&p->fb_lock);\
		list_free_all_def(p->event_queue);\
		pthread_mutex_unlock(&p->fb_lock);\
		log_return(efi_success);\
	}

DECL_RESET(simple_pointer,simple)
DECL_RESET(absolute_pointer,abs)
DECL_RESET(simple_text_input,kbd)
DECL_RESET(simple_text_input_ex,kbd_ex)

static efiapi void sdl2_wait_for_event(
	efi_event event,
	void*data
){
	list*l;
	efi_graphics_output_protocol_private_sdl2*p=data;
	if(!efi_current_ctx||!data||!event)return;
	pthread_mutex_lock(&p->fb_lock);
	if((l=list_first(p->event_queue)))do{
		LIST_DATA_DECLARE(ev,l,SDL_Event*);
		if(ev)switch(ev->type){
			case SDL_MOUSEBUTTONUP:case SDL_MOUSEBUTTONDOWN:case SDL_MOUSEMOTION:
				efi_current_ctx->bs->signal_event(p->in.simple.protocol.wait_for_input);
			break;
			case SDL_FINGERUP:case SDL_FINGERDOWN:case SDL_FINGERMOTION:
				efi_current_ctx->bs->signal_event(p->in.abs.protocol.wait_for_input);
			break;
			case SDL_KEYDOWN:case SDL_KEYUP:
				efi_current_ctx->bs->signal_event(p->in.kbd_ex.protocol.wait_for_key_ex);
				efi_current_ctx->bs->signal_event(p->in.kbd.protocol.wait_for_key);
			break;
			default:continue;
		}
		break;
	}while((l=l->next));
	pthread_mutex_unlock(&p->fb_lock);
}

static efiapi efi_status sdl2_simple_pointer_get_state(
	efi_simple_pointer_protocol*proto,
	efi_simple_pointer_state*state
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,state),
		NULL
	);
	list*l;
	bool v;
	efi_status st=efi_not_ready;
	if(!state)log_return(efi_invalid_parameter);
	else memset(state,0,sizeof(efi_simple_pointer_state));
	DECL_PROTO_GET(simple)
	p->in.simple.protocol.wait_for_input->signaled=false;
	pthread_mutex_lock(&p->fb_lock);
	if((l=list_first(p->event_queue)))do{
		LIST_DATA_DECLARE(event,l,SDL_Event*);
		if(!event)continue;
		switch(event->type){
			case SDL_MOUSEBUTTONUP:case SDL_MOUSEBUTTONDOWN:
				v=event->button.state==SDL_PRESSED;
				switch(event->button.button){
					case SDL_BUTTON_LEFT:p->in.simple.state.left_btn=v;break;
					case SDL_BUTTON_RIGHT:p->in.simple.state.right_btn=v;break;
					default:continue;
				}
			break;
			case SDL_MOUSEMOTION:
				p->in.simple.state.rel_move_x=event->motion.xrel;
				p->in.simple.state.rel_move_y=event->motion.yrel;
			break;
			default:continue;
		}
		memcpy(state,&p->in.simple.state,sizeof(efi_simple_pointer_state));
		p->in.simple.state.rel_move_x=0;
		p->in.simple.state.rel_move_y=0;
		p->in.simple.state.rel_move_z=0;
		list_obj_del(&p->event_queue,l,list_default_free);
		st=efi_success;
		break;
	}while((l=l->next));
	pthread_mutex_unlock(&p->fb_lock);
	log_return(st);
}

static efiapi efi_status sdl2_absolute_pointer_get_state(
	efi_absolute_pointer_protocol*proto,
	efi_absolute_pointer_state*state
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,state),
		NULL
	);
	list*l;
	efi_status st=efi_not_ready;
	if(!state)log_return(efi_invalid_parameter);
	else memset(state,0,sizeof(efi_absolute_pointer_state));
	DECL_PROTO_GET(abs)
	p->in.abs.protocol.wait_for_input->signaled=false;
	pthread_mutex_lock(&p->fb_lock);
	if((l=list_first(p->event_queue)))do{
		LIST_DATA_DECLARE(event,l,SDL_Event*);
		if(!event)continue;
		switch(event->type){
			case SDL_FINGERUP:
				p->in.abs.state.active_buttons|=EFI_ABSP_TouchActive;
				p->in.abs.state.cur_z=p->in.abs.mode.abs_min_z;
			break;
			case SDL_FINGERDOWN:
				p->in.abs.state.active_buttons&=~EFI_ABSP_TouchActive;
			break;
			case SDL_FINGERMOTION:
				p->in.abs.state.cur_x=p->in.abs.mode.abs_max_x*event->tfinger.x;
				p->in.abs.state.cur_y=p->in.abs.mode.abs_max_y*event->tfinger.y;
				p->in.abs.state.cur_z=p->in.abs.mode.abs_max_z*event->tfinger.pressure;
			break;
			default:continue;
		}
		memcpy(state,&p->in.abs.state,sizeof(efi_absolute_pointer_state));
		list_obj_del(&p->event_queue,l,list_default_free);
		st=efi_success;
		break;
	}while((l=l->next));
	pthread_mutex_unlock(&p->fb_lock);
	log_return(st);
}

static void sdl2_keysym_to_efi_key(SDL_Keysym*sym,efi_input_key*key){
	switch(sym->sym){
		case SDLK_F1:key->scan_code=SCAN_F1;break;
		case SDLK_F2:key->scan_code=SCAN_F2;break;
		case SDLK_F3:key->scan_code=SCAN_F3;break;
		case SDLK_F4:key->scan_code=SCAN_F4;break;
		case SDLK_F5:key->scan_code=SCAN_F5;break;
		case SDLK_F6:key->scan_code=SCAN_F6;break;
		case SDLK_F7:key->scan_code=SCAN_F7;break;
		case SDLK_F8:key->scan_code=SCAN_F8;break;
		case SDLK_F9:key->scan_code=SCAN_F9;break;
		case SDLK_F10:key->scan_code=SCAN_F10;break;
		case SDLK_F11:key->scan_code=SCAN_F11;break;
		case SDLK_F12:key->scan_code=SCAN_F12;break;
		case SDLK_ESCAPE:key->scan_code=SCAN_ESC;break;
		case SDLK_UP:key->scan_code=SCAN_UP;break;
		case SDLK_DOWN:key->scan_code=SCAN_DOWN;break;
		case SDLK_RIGHT:key->scan_code=SCAN_RIGHT;break;
		case SDLK_LEFT:key->scan_code=SCAN_LEFT;break;
		case SDLK_HOME:key->scan_code=SCAN_HOME;break;
		case SDLK_END:key->scan_code=SCAN_END;break;
		case SDLK_INSERT:key->scan_code=SCAN_INSERT;break;
		case SDLK_DELETE:key->scan_code=SCAN_DELETE;break;
		case SDLK_PAGEUP:key->scan_code=SCAN_PAGE_UP;break;
		case SDLK_PAGEDOWN:key->scan_code=SCAN_PAGE_DOWN;break;
		case SDLK_PAUSE:key->scan_code=SCAN_PAUSE;break;
		case SDLK_F13:key->scan_code=SCAN_F13;break;
		case SDLK_F14:key->scan_code=SCAN_F14;break;
		case SDLK_F15:key->scan_code=SCAN_F15;break;
		case SDLK_F16:key->scan_code=SCAN_F16;break;
		case SDLK_F17:key->scan_code=SCAN_F17;break;
		case SDLK_F18:key->scan_code=SCAN_F18;break;
		case SDLK_F19:key->scan_code=SCAN_F19;break;
		case SDLK_F20:key->scan_code=SCAN_F20;break;
		case SDLK_F21:key->scan_code=SCAN_F21;break;
		case SDLK_F22:key->scan_code=SCAN_F22;break;
		case SDLK_F23:key->scan_code=SCAN_F23;break;
		case SDLK_F24:key->scan_code=SCAN_F24;break;
		case SDLK_MUTE:key->scan_code=SCAN_MUTE;break;
		case SDLK_VOLUMEUP:key->scan_code=SCAN_VOLUME_UP;break;
		case SDLK_VOLUMEDOWN:key->scan_code=SCAN_VOLUME_DOWN;break;
		case SDLK_BRIGHTNESSUP:key->scan_code=SCAN_BRIGHTNESS_UP;break;
		case SDLK_BRIGHTNESSDOWN:key->scan_code=SCAN_BRIGHTNESS_DOWN;break;
		case SDLK_SLEEP:key->scan_code=SCAN_SUSPEND;break;
		case SDLK_DISPLAYSWITCH:key->scan_code=SCAN_TOGGLE_DISPLAY;break;
		case SDLK_EJECT:key->scan_code=SCAN_EJECT;break;
		case SDLK_BACKSPACE:key->unicode_char=CHAR_BACKSPACE;break;
		case SDLK_TAB:key->unicode_char=CHAR_TAB;break;
		case SDLK_RETURN:key->unicode_char=CHAR_CARRIAGE_RETURN;break;
		default:key->unicode_char=sym->sym;break;
	}
}

static efiapi efi_status sdl2_simple_text_in_read_key(
	efi_simple_text_input_protocol*proto,
	efi_input_key*key
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,key),
		NULL
	);
	list*l;
	efi_status st=efi_not_ready;
	if(!key)log_return(efi_invalid_parameter);
	else memset(key,0,sizeof(efi_input_key));
	DECL_PROTO_GET(kbd)
	p->in.kbd.protocol.wait_for_key->signaled=false;
	pthread_mutex_lock(&p->fb_lock);
	if((l=list_first(p->event_queue)))do{
		LIST_DATA_DECLARE(event,l,SDL_Event*);
		if(!event)continue;
		switch(event->type){
			case SDL_KEYDOWN:
				sdl2_keysym_to_efi_key(&event->key.keysym,key);
			break;
			default:continue;
		}
		list_obj_del(&p->event_queue,l,list_default_free);
		st=efi_success;
		break;
	}while((l=l->next));
	pthread_mutex_unlock(&p->fb_lock);
	log_return(st);
}

static efiapi efi_status sdl2_simple_text_input_ex_read_key(
	efi_simple_text_input_ex_protocol*proto,
	efi_key_data*data
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,data),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status sdl2_simple_text_input_ex_set_state(
	efi_simple_text_input_ex_protocol*proto,
	efi_key_toggle_state*state
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,state),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status sdl2_simple_text_input_ex_register_keystroke_notify(
	efi_simple_text_input_ex_protocol*proto,
	efi_key_data*data,
	efi_key_notify_function func,
	void**handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,data),
		call_arg(ptr,func),
		call_arg(ptr,handle),
		NULL
	);
	log_return(efi_unsupported);
}

static efiapi efi_status sdl2_simple_text_input_ex_unregister_keystroke_notify(
	efi_simple_text_input_ex_protocol*proto,
	void*handle
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,handle),
		NULL
	);
	log_return(efi_unsupported);
}

static const efi_component_name_protocol comp_name={
	.get_driver_name     = sdl2_get_driver_name,
	.get_controller_name = sdl2_get_controller_name,
	.supported_languages = (char8*)"eng",
};

static const efi_component_name2_protocol comp_name2={
	.get_driver_name     = sdl2_get_driver_name,
	.get_controller_name = sdl2_get_controller_name,
	.supported_languages = (char8*)"en",
};

static const efi_graphics_output_protocol protocol={
	.query_mode = efi_gop_query_mode,
	.set_mode   = sdl2_gop_set_mode,
	.blt        = sdl2_gop_blt,
	.mode       = NULL,
};

static const efi_simple_pointer_protocol simple_pointer_protocol={
	.reset          = sdl2_simple_pointer_reset,
	.get_state      = sdl2_simple_pointer_get_state,
	.wait_for_input = NULL,
	.mode           = NULL,
};

static const efi_absolute_pointer_protocol absolute_pointer_protocol={
	.reset          = sdl2_absolute_pointer_reset,
	.get_state      = sdl2_absolute_pointer_get_state,
	.wait_for_input = NULL,
	.mode           = NULL,
};

static const efi_simple_text_input_protocol simple_text_input_protocol={
	.reset           = sdl2_simple_text_input_reset,
	.read_key_stroke = sdl2_simple_text_in_read_key,
	.wait_for_key    = NULL,
};

static const efi_simple_text_input_ex_protocol simple_text_input_ex_protocol={
	.reset                 = sdl2_simple_text_input_ex_reset,
	.read_key_stroke_ex    = sdl2_simple_text_input_ex_read_key,
	.wait_for_key_ex       = NULL,
	.set_state             = sdl2_simple_text_input_ex_set_state,
	.register_key_notify   = sdl2_simple_text_input_ex_register_keystroke_notify,
	.unregister_key_notify = sdl2_simple_text_input_ex_unregister_keystroke_notify,
};

static _Noreturn void*update_thread(void*d){
	struct timespec ts;
	bool update;
	SDL_Event event;
	efi_graphics_output_protocol_private_sdl2*p=d;
	while(true){
		ts.tv_sec=0,ts.tv_nsec=20000000;
		errno=0;
		update=sem_timedwait(&p->update,&ts)==0;
		if(efi_current_ctx&&!efi_current_ctx->timer)update=true;
		if(update){
			pthread_mutex_lock(&p->fb_lock);
			SDL_UpdateTexture(
				p->texture,NULL,p->data.buffer,
				p->data.cur->width*sizeof(uint32_t)
			);
			SDL_RenderClear(p->renderer);
			SDL_RenderCopy(p->renderer,p->texture,NULL,NULL);
			SDL_RenderPresent(p->renderer);
			pthread_mutex_unlock(&p->fb_lock);
		}
		pthread_mutex_lock(&p->fb_lock);
		while(SDL_PollEvent(&event))switch(event.type){
			case SDL_QUIT:exit(0);
			case SDL_KEYDOWN:case SDL_KEYUP:
			case SDL_FINGERMOTION:case SDL_FINGERUP:case SDL_FINGERDOWN:
			case SDL_MOUSEBUTTONDOWN:case SDL_MOUSEBUTTONUP:case SDL_MOUSEMOTION:
				list_obj_add_new_dup(
					p->data.pool,&p->event_queue,
					&event,sizeof(event)
				);
			break;
			case SDL_WINDOWEVENT:switch(event.window.event){
				#if SDL_VERSION_ATLEAST(2,0,5)
				case SDL_WINDOWEVENT_TAKE_FOCUS:
				#endif
				case SDL_WINDOWEVENT_EXPOSED:
					sem_post(&p->update);
				break;
				default:break;
			}
			break;
			case SDL_TEXTEDITING:case SDL_TEXTINPUT:
			case SDL_KEYMAPCHANGED:case SDL_TEXTEDITING_EXT:continue;
			default:xlog(LOG_INFO,"got unknown sdl2 event %x",event.type);break;
		}
		pthread_mutex_unlock(&p->fb_lock);
		usleep(10000);
	}
}

static bool init_sdl2(efi_graphics_output_protocol_private_sdl2*gop){
	sem_init(&gop->update,0,0);
	pthread_mutex_init(&gop->fb_lock,NULL);
	SDL_Init(SDL_INIT_VIDEO);
	gop->window=SDL_CreateWindow(
		"efi-wrapper",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		gop->data.cur->width,
		gop->data.cur->height,
		SDL_WINDOW_HIDDEN
	);
	if(!gop->window)DONE(xerror("create sdl2 window failed"));
	gop->renderer=SDL_CreateRenderer(gop->window,-1,SDL_RENDERER_SOFTWARE);
	if(!gop->renderer)DONE(xerror("create sdl2 renderer failed"));
	gop->texture=SDL_CreateTexture(
		gop->renderer,
		SDL_PIXELFORMAT_XBGR8888,
		SDL_TEXTUREACCESS_STATIC,
		gop->data.cur->width,
		gop->data.cur->height
	);
	if(!gop->texture)DONE(xerror("create sdl2 texture failed"));
	SDL_SetTextureBlendMode(
		gop->texture,
		SDL_BLENDMODE_BLEND
	);
	pthread_create(&gop->update_tid,NULL,update_thread,gop);
	return true;
	done:return false;
}

static efi_status init_protocol(
	efi_handle*handle,
	efi_graphics_output_protocol_private_sdl2*gop
){
	efi_status st=efi_success;
	efi_create_event cr=efi_current_ctx->bs->create_event;
	memcpy(&gop->in.abs.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid,sizeof(efi_guid));
	memcpy(&gop->in.kbd.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid,sizeof(efi_guid));
	memcpy(&gop->in.kbd_ex.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid,sizeof(efi_guid));
	memcpy(&gop->in.simple.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid,sizeof(efi_guid));
	memcpy(&gop->in.simple.protocol,&simple_pointer_protocol,sizeof(simple_pointer_protocol));
	memcpy(&gop->in.abs.protocol,&absolute_pointer_protocol,sizeof(absolute_pointer_protocol));
	memcpy(&gop->in.kbd.protocol,&simple_text_input_protocol,sizeof(simple_text_input_protocol));
	memcpy(&gop->in.kbd_ex.protocol,&simple_text_input_ex_protocol,sizeof(simple_text_input_ex_protocol));
	if(!efi_error(st))st=cr(EVT_NOTIFY_WAIT,TPL_NOTIFY,sdl2_wait_for_event,gop,&gop->in.abs.protocol.wait_for_input);
	if(!efi_error(st))st=cr(EVT_NOTIFY_WAIT,TPL_NOTIFY,sdl2_wait_for_event,gop,&gop->in.simple.protocol.wait_for_input);
	if(!efi_error(st))st=cr(EVT_NOTIFY_WAIT,TPL_NOTIFY,sdl2_wait_for_event,gop,&gop->in.kbd_ex.protocol.wait_for_key_ex);
	if(!efi_error(st))st=cr(EVT_NOTIFY_WAIT,TPL_NOTIFY,sdl2_wait_for_event,gop,&gop->in.kbd.protocol.wait_for_key);
	if(efi_error(st))return st;
	gop->in.abs.mode.attrs=EFI_ABSP_SupportsPressureAsZ;
	gop->in.abs.mode.abs_max_x=gop->data.cur->width;
	gop->in.abs.mode.abs_max_y=gop->data.cur->height;
	gop->in.abs.mode.abs_max_z=128;
	gop->in.simple.mode.res_x=1;
	gop->in.simple.mode.res_y=1;
	gop->in.simple.mode.left_btn=true;
	gop->in.simple.mode.right_btn=true;
	gop->in.simple.protocol.mode=&gop->in.simple.mode;
	gop->in.abs.protocol.mode=&gop->in.abs.mode;
	return efi_current_ctx->bs->install_multiple_proto_interfaces(
		handle,
		&gEfiDevicePathProtocolGuid,&dp,
		&gEfiComponentNameProtocolGuid,&comp_name,
		&gEfiComponentName2ProtocolGuid,&comp_name2,
		&gEfiGraphicsOutputProtocolGuid,&gop->data.protocol,
		&gEfiAbsolutePointerProtocolGuid,&gop->in.abs.protocol,
		&gEfiSimplePointerProtocolGuid,&gop->in.simple.protocol,
		&gEfiSimpleTextInProtocolGuid,&gop->in.kbd.protocol,
		&gEfiSimpleTextInputExProtocolGuid,&gop->in.kbd_ex.protocol,
		NULL
	);
}

efi_status efi_new_graphics_output_from_sdl2(
	efi_handle*handle,
	efi_graphics_output_protocol**proto
){
	efi_status st;
	static const size_t size=sizeof(efi_graphics_output_protocol_private_sdl2);
	efi_graphics_output_protocol_private_sdl2*ret;
	if(!efi_current_ctx)return efi_not_ready;
	if(!handle)return efi_invalid_parameter;
	if(!(ret=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,size)))return efi_out_of_resources;
	memset(ret,0,size);
	memcpy(&ret->data.protocol,&protocol,sizeof(ret->data.protocol));
	memcpy(&ret->data.mode,&efi_gop_mode,sizeof(efi_gop_mode));
	memcpy(&ret->data.magic,&gEfiWrapperSDL2GraphicsOutputProtocolGuid,sizeof(efi_guid));
	ret->data.pool=efi_current_ctx->mem[efi_bs_data].pool;
	ret->data.cur=&efi_gop_modes[0];
	ret->data.reallocate=true;
	ret->data.protocol.mode=&ret->data.mode;
	st=efi_update_size(&ret->data,ret->data.cur,NULL,NULL);
	if(efi_error(st))goto fail;
	st=efi_device_error;
	if(!init_sdl2(ret))goto fail;
	st=init_protocol(handle,ret);
	if(efi_error(st))goto fail;
	if(proto)*proto=(efi_graphics_output_protocol*)ret;
	return efi_success;
	fail:
	if(ret->data.cfg)mem_free(ret->data.cfg);
	if(ret->data.buffer)mem_free(ret->data.buffer);
	mem_free(ret);
	return st;
}

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	if(!ctx||!ins)
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	return efi_new_graphics_output_from_sdl2(
		&ins->handle,
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
	#if defined(__linux__)
	if(!getenv("DISPLAY")&&!getenv("WAYLAND_DISPLAY")){
		static bool warn=false;
		if(!warn)xlog(LOG_ERROR,"no available display for sdl2");
		warn=true;
		return efi_not_ready;
	}
	#endif
	return efi_success;
}

static const efi_service efi_sdl2_graphics_output_service={
	.guid     = &gEfiWrapperSDL2GraphicsOutputProtocolGuid,
	.name     = "SDL2GraphicsOutputProtocol",
	.provides = {
		&gEfiGraphicsOutputProtocolGuid,
		&gEfiAbsolutePointerProtocolGuid,
		&gEfiSimplePointerProtocolGuid,
		&gEfiSimpleTextInProtocolGuid,
		&gEfiSimpleTextInputExProtocolGuid,
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

DECLARE_EFI_SERVICE(efi_sdl2_graphics_output_service)

#else
efi_status efi_new_graphics_output_from_sdl2(
	efi_handle*handle cdecl_attr_unused,
	efi_graphics_output_protocol**proto cdecl_attr_unused
){
	return efi_unsupported;
}
#endif