#include<iconv.h>
#include<string.h>
#include<termios.h>
#include<sys/ioctl.h>
#include"internal.h"

extern struct winsize ws_size;
typedef struct efi_simple_text_input_protocol_private_posix_fd{
	efi_simple_text_input_protocol_private data;
	struct termios restore;
	int fd;
}efi_simple_text_input_protocol_private_posix_fd;

typedef struct efi_simple_text_output_protocol_private_posix_fd{
	efi_simple_text_output_protocol_private data;
	efi_simple_text_output_mode mode;
	struct termios restore;
	iconv_t iconv;
	bool tty;
	int fd;
}efi_simple_text_output_protocol_private_posix_fd;

static const struct winsize def_size={
	.ws_col    = 80,
	.ws_row    = 25,
	.ws_xpixel = 0,
	.ws_ypixel = 0,
};

static const efi_simple_text_output_mode def_mode={
	.max_mode          = 1,
	.mode              = 0,
	.attribute.data.fg = EFI_LIGHTGRAY,
	.attribute.data.bg = EFI_BLACK,
	.cursor_col     = 0,
	.cursor_row        = 0,
	.cursor_visible    = true,
};

static bool write_buffer(int fd,const char*buff,size_t length){
	ssize_t r;
	size_t off;
	for(off=0;off<length;){
		r=write(fd,buff,length-off);
		if(r<=0)return false;
		off+=(size_t)r;
	}
	return true;
}

static int ansi_fg_color_convert(uint16_t in){
	switch(in){
		case EFI_BLACK:return 30;
		case EFI_RED:return 31;
		case EFI_GREEN:return 32;
		case EFI_BROWN:return 33;
		case EFI_BLUE:return 34;
		case EFI_MAGENTA:return 35;
		case EFI_CYAN:return 36;
		case EFI_LIGHTGRAY:return 37;
		case EFI_BRIGHT:return 90;
		case EFI_LIGHTRED:return 91;
		case EFI_LIGHTGREEN:return 92;
		case EFI_YELLOW:return 93;
		case EFI_LIGHTBLUE:return 94;
		case EFI_LIGHTMAGENTA:return 95;
		case EFI_LIGHTCYAN:return 96;
		case EFI_WHITE:return 97;
		default:return 37;
	}
}
static int ansi_bg_color_convert(uint16_t bg){
	switch(bg){
		case EFI_BACKGROUND_BLACK:return 40;
		case EFI_BACKGROUND_RED:return 41;
		case EFI_BACKGROUND_GREEN:return 42;
		case EFI_BACKGROUND_BROWN:return 43;
		case EFI_BACKGROUND_BLUE:return 44;
		case EFI_BACKGROUND_MAGENTA:return 45;
		case EFI_BACKGROUND_CYAN:return 46;
		case EFI_BACKGROUND_LIGHTGRAY:return 47;
		default:return 40;
	}
}

static efiapi efi_status efi_input_reset_impl(
	efi_simple_text_input_protocol*proto,
	bool ext_verify
){
	log_call(
		call_arg(ptr,proto),
		call_arg(bool,ext_verify),
		NULL
	);
	efi_simple_text_input_protocol_private_posix_fd*p=
		(efi_simple_text_input_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextInProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	fsync(p->fd);
	log_return(efi_success);
}

static bool parse_char(efi_input_key*key,char*escape,char c){
	memset(key,0,sizeof(efi_input_key));
	switch(c){
		case 0x7F:key->unicode_char=CHAR_BACKSPACE;break;
		case 0x0A:key->unicode_char=CHAR_CARRIAGE_RETURN;break;
		case 0x1B:
			key->scan_code=SCAN_ESC;
			escape[0]=c;
			return true;
		default:key->unicode_char=(char16)c;
	}
	return false;
}

static bool map_escape(char*escape,efi_input_key*key){
	efi_input_key_escape_map*m;
	memset(key,0,sizeof(efi_input_key));
	for(size_t i=0;(m=&escape_map[i])->escape[0];i++){
		if(memcmp(escape,m->escape,sizeof(m->escape))!=0)continue;
		memcpy(key,&m->map,sizeof(efi_input_key));
		return true;
	}
	return false;
}

static efiapi efi_status efi_input_read_key_impl(
	efi_simple_text_input_protocol*proto,
	efi_input_key*key
){
	log_call(
		call_arg(ptr,proto),
		call_arg(ptr,key),
		NULL
	);
	int r;
	char c=0;
	size_t off=1;
	bool cont=false;
	char escape[32];
	struct pollfd fds;
	efi_simple_text_input_protocol_private_posix_fd*p=
		(efi_simple_text_input_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextInProtocolGuid)||!key)
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	fds.fd=p->fd,fds.events=POLLIN,fds.revents=0;
	memset(escape,0,sizeof(escape));
	if((r=poll(&fds,1,0))==1)do{
		if((r=(int)read(p->fd,&c,1))!=1)break;
		if(escape[0]){
			escape[off++]=c;
			if(map_escape(escape,key))log_return(efi_success);
			if(off>=sizeof(escape))break;
		}else cont=parse_char(key,escape,c);
	}while(cont&&(r=poll(&fds,1,50))==1);
	if(escape[0])parse_char(key,escape,escape[0]);
	if(r<0){
		xerror("read fd failed: %m");
		log_return(efi_errno_to_status(errno)?:efi_device_error);
	}
	log_return(r==1?efi_success:efi_not_ready);
}

static efiapi void efi_text_wait_for_key(
	efi_event event,
	void*data
){
	struct pollfd fds;
	efi_simple_text_input_protocol_private_posix_fd*p=data;
	if(!efi_current_ctx||!p||!efi_guid_equals(
		&p->data.magic,&gEfiWrapperPosixFDSimpleTextInProtocolGuid
	)||!event||p->fd<0)return;
	fds.fd=p->fd,fds.events=POLLIN,fds.revents=0;
	if(poll(&fds,1,0)==1)efi_current_ctx->bs->signal_event(event);
}

static efiapi efi_status efi_output_reset_impl(
	efi_simple_text_output_protocol*proto,
	bool ext_verify
){
	log_call(
		call_arg(ptr,proto),
		call_arg(bool,ext_verify),
		NULL
	);
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	if(p->tty)tcsetattr(p->fd,TCSANOW,&p->restore);
	proto->enable_cursor(proto,true);
	fsync(p->fd);
	log_return(efi_success);
}

static efiapi efi_status efi_text_string_impl(
	efi_simple_text_output_protocol*proto,
	char16*string
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,string),
		NULL
	);
	size_t out;
	char buff[4096];
	struct winsize ws;
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid)||!string)
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	memset(&ws,0,sizeof(ws));
	if(p->fd==STDOUT_FILENO||p->fd==STDERR_FILENO)
		memcpy(&ws,&ws_size,sizeof(ws));
	else if(p->tty)ioctl(p->fd,TIOCGWINSZ,&ws);
	if(ws.ws_row==0||ws.ws_col==0)
		memcpy(&ws,&def_size,sizeof(struct winsize));
	for(size_t i=0;string[i];i++)switch(string[i]){
		case L'\n':p->mode.cursor_row++;break;
		case L'\r':p->mode.cursor_col=0;break;
		case L'\t':p->mode.cursor_col=round_down(p->mode.cursor_col+8,8)-1;//fallthrough
		default:
			p->mode.cursor_col++;
			if(p->mode.cursor_col>=ws.ws_col){
				p->mode.cursor_col=0;
				p->mode.cursor_row++;
			}
		break;
	}
	p->mode.cursor_row=MIN(p->mode.cursor_row,ws.ws_row-1);
	do{
		out=ustr2astr(buff,sizeof(buff),string);
		if(out<=0){
			xerror("convert failed: %m");
			log_return(efi_device_error);
		}
		write_buffer(p->fd,buff,out);
		string+=out;
	}while(*string);
	fsync(p->fd);
	p->mode.mode=def_mode.mode;
	p->mode.max_mode=def_mode.max_mode;
	log_return(efi_success);
}

static efiapi efi_status efi_test_string_impl(
	efi_simple_text_output_protocol*proto,
	char16*string
){
	log_call(
		call_arg(ptr,proto),
		call_arg(str16,string),
		NULL
	);
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid)||!string)
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	for(size_t i=0;string[i];i++)
		if(string[i]>UINT8_MAX)log_return(efi_unsupported);
	log_return(efi_success);
}

static efiapi efi_status efi_text_query_mode_impl(
	efi_simple_text_output_protocol*proto,
	uintn_t mode,
	uintn_t*cols,
	uintn_t*rows
){
	log_call(
		call_arg(ptr,proto),
		call_arg(unsigned,mode),
		call_arg(ptr,cols),
		call_arg(ptr,rows),
		NULL
	);
	struct winsize ws;
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	if(mode!=0)log_return(efi_unsupported);
	memset(&ws,0,sizeof(ws));
	if(p->fd==STDOUT_FILENO||p->fd==STDERR_FILENO)
		memcpy(&ws,&ws_size,sizeof(ws));
	else if(p->tty)ioctl(p->fd,TIOCGWINSZ,&ws);
	if(ws.ws_row==0||ws.ws_col==0)
		memcpy(&ws,&def_size,sizeof(struct winsize));
	if(cols)*cols=ws.ws_col;
	if(rows)*rows=ws.ws_row;
	log_return(efi_success);
}

static efiapi efi_status efi_text_set_mode_impl(
	efi_simple_text_output_protocol*proto,
	uintn_t mode
){
	log_call(
		call_arg(ptr,proto),
		call_arg(unsigned,mode),
		NULL
	);
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	if(mode!=0)log_return(efi_unsupported);
	log_return(efi_success);
}

static efiapi efi_status efi_text_set_attribute_impl(
	efi_simple_text_output_protocol*proto,
	uintn_t attribute
){
	char buff[64];
	log_call(
		call_arg(ptr,proto),
		call_arg(hex,attribute),
		NULL
	);
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	proto->mode->attribute.num=(int32_t)attribute;
	if(p->tty){
		int fg=ansi_fg_color_convert(p->mode.attribute.data.fg);
		int bg=ansi_bg_color_convert(p->mode.attribute.data.bg);
		memset(buff,0,sizeof(buff));
		snprintf(buff,sizeof(buff)-1,"\033[0;%d;%dm",fg,bg);
		write_buffer(p->fd,buff,strlen(buff));
		fsync(p->fd);
	}
	log_return(efi_success);
}

static efiapi efi_status efi_text_clear_screen_impl(
	efi_simple_text_output_protocol*proto
){
	log_call(call_arg(ptr,proto),NULL);
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	if(p->tty)tcsetattr(p->fd,TCSANOW,&p->restore);
	write_buffer(p->fd,"\033[H\033[2J\033[3J",11);
	p->mode.mode=def_mode.mode;
	p->mode.max_mode=def_mode.max_mode;
	proto->set_cursor_position(proto,0,0);
	fsync(p->fd);
	log_return(efi_success);
}

static efiapi efi_status efi_text_set_cursor_position_impl(
	efi_simple_text_output_protocol*proto,
	uintn_t col,
	uintn_t row
){
	log_call(
		call_arg(ptr,proto),
		call_arg(signed,col),
		call_arg(signed,row),
		NULL
	);
	char buff[64];
	struct winsize ws;
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	memset(&ws,0,sizeof(ws));
	if(p->fd==STDOUT_FILENO||p->fd==STDERR_FILENO)
		memcpy(&ws,&ws_size,sizeof(ws));
	else if(p->tty&&ioctl(p->fd,TIOCGWINSZ,&ws)!=0)
		log_return(efi_device_error);
	if(ws.ws_col==0||ws.ws_row==0)
		memcpy(&ws,&def_size,sizeof(ws));
	if(col>=ws.ws_col||row>=ws.ws_row)log_return(efi_unsupported);
	p->mode.cursor_col=(int32_t)col,p->mode.cursor_row=(int32_t)row;
	if(p->tty){
		memset(buff,0,sizeof(buff));
		snprintf(buff,sizeof(buff)-1,"\033[%zd;%zdH",row+1,col+1);
		write_buffer(p->fd,buff,strlen(buff));
		fsync(p->fd);
	}
	log_return(efi_success);
}

static efiapi efi_status efi_text_enable_cursor_impl(
	efi_simple_text_output_protocol*proto,
	bool visible
){
	log_call(
		call_arg(ptr,proto),
		call_arg(bool,visible),
		NULL
	);
	static char buff[6]="\033[?25\0";
	efi_simple_text_output_protocol_private_posix_fd*p=
		(efi_simple_text_output_protocol_private_posix_fd*)proto;
	if(!p||!efi_guid_equals(&p->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid))
		log_return(efi_invalid_parameter);
	if(p->fd<0)log_return(efi_device_error);
	if(p->tty){
		buff[5]=visible?'h':'l';
		write_buffer(p->fd,buff,sizeof(buff));
		fsync(p->fd);
	}
	p->mode.cursor_visible=visible;
	log_return(efi_success);
}

static const efi_simple_text_input_protocol in_protocol={
	.reset           = efi_input_reset_impl,
	.read_key_stroke = efi_input_read_key_impl,
	.wait_for_key    = NULL,
};

static const efi_simple_text_output_protocol out_protocol={
	.reset               = efi_output_reset_impl,
	.output_string       = efi_text_string_impl,
	.test_string         = efi_test_string_impl,
	.query_mode          = efi_text_query_mode_impl,
	.set_mode            = efi_text_set_mode_impl,
	.set_attribute       = efi_text_set_attribute_impl,
	.clear_screen        = efi_text_clear_screen_impl,
	.set_cursor_position = efi_text_set_cursor_position_impl,
	.enable_cursor       = efi_text_enable_cursor_impl,
	.mode                = NULL,
};

efi_status efi_new_simple_text_input_from_posix_fd(int fd,efi_handle*handle,efi_simple_text_input_protocol**proto){
	struct termios tio;
	static const size_t size=sizeof(efi_simple_text_input_protocol_private_posix_fd);
	efi_simple_text_input_protocol_private_posix_fd*ret;
	if(!efi_current_ctx)return efi_not_ready;
	if(fd<0||!handle)return efi_invalid_parameter;
	if(!(ret=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,size)))return efi_out_of_resources;
	memcpy(&ret->data.protocol,&in_protocol,sizeof(ret->data.protocol));
	memcpy(&ret->data.magic,&gEfiWrapperPosixFDSimpleTextInProtocolGuid,sizeof(efi_guid));
	if(isatty(fd)&&tcgetattr(fd,&tio)==0){
		memcpy(&ret->restore,&tio,sizeof(struct termios));
		tio.c_lflag&=~(ECHO|ICANON);
		tcsetattr(fd,TCSAFLUSH,&tio);
	}
	efi_current_ctx->bs->create_event(
		EVT_NOTIFY_WAIT,TPL_NOTIFY,
		efi_text_wait_for_key,ret,
		&ret->data.protocol.wait_for_key
	);
	ret->fd=fd;
	if(proto)*proto=&ret->data.protocol;
	return efi_current_ctx->bs->install_proto_interface(
		handle,&gEfiSimpleTextInProtocolGuid,
		efi_native_interface,ret
	);
}

efi_status efi_new_simple_text_output_from_posix_fd(
	int fd,
	efi_handle*handle,
	efi_simple_text_output_protocol**proto
){
	static const size_t size=sizeof(efi_simple_text_output_protocol_private_posix_fd);
	efi_simple_text_output_protocol_private_posix_fd*ret;
	if(!efi_current_ctx)return efi_not_ready;
	if(fd<0||!handle)return efi_invalid_parameter;
	if(!(ret=mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,size)))return efi_out_of_resources;
	memcpy(&ret->data.protocol,&out_protocol,sizeof(ret->data.protocol));
	ret->data.protocol.mode=&ret->mode;
	memcpy(&ret->data.magic,&gEfiWrapperPosixFDSimpleTextOutProtocolGuid,sizeof(efi_guid));
	ret->fd=fd;
	memcpy(&ret->mode,&def_mode,sizeof(ret->mode));
	ret->iconv=iconv_open("UTF-8//TRANSLIT","UCS-2");
	if(!ret->iconv){
		xerror("open iconv failed");
		free(ret);
		return efi_unsupported;
	}
	ret->tty=isatty(ret->fd);
	if(ret->tty)tcgetattr(ret->fd,&ret->restore);
	out_protocol.reset(&ret->data.protocol,false);
	if(proto)*proto=&ret->data.protocol;
	return efi_current_ctx->bs->install_proto_interface(
		handle,&gEfiSimpleTextOutProtocolGuid,
		efi_native_interface,ret
	);
}

static void update_system_table_by_fd(
	int fd,
	efi_system_table*st,
	efi_handle hand,
	void*proto
){
	switch(fd){
		case STDIN_FILENO:
			if(st->con_stdin||st->con_stdin_handle)return;
			st->con_stdin=proto,st->con_stdin_handle=hand;
		break;
		case STDOUT_FILENO:
			if(st->con_stdout||st->con_stdout_handle)return;
			st->con_stdout=proto,st->con_stdout_handle=hand;
		break;
		case STDERR_FILENO:
			if(st->con_stderr||st->con_stderr_handle)return;
			st->con_stderr=proto,st->con_stderr_handle=hand;
		break;
		default:return;
	}
	efi_table_calc_sum(st,sizeof(efi_system_table));
}

static efi_status svc_start_in(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st;
	if(!ctx||!ins||!ins->cfg)
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	st=efi_new_simple_text_input_from_posix_fd(
		ins->cfg->config.int32,&ins->handle,
		(efi_simple_text_input_protocol**)&ins->cust_data
	);
	if(efi_error(st))return st;
	update_system_table_by_fd(
		ins->cfg->config.int32,
		ctx->st,ins->handle,
		ins->cust_data
	);
	return efi_success;
}

static efi_status svc_start_out(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	efi_status st;
	if(!ctx||!ins||!ins->cfg)
		return efi_invalid_parameter;
	if(ctx!=efi_current_ctx)return efi_not_ready;
	st=efi_new_simple_text_output_from_posix_fd(
		ins->cfg->config.int32,&ins->handle,
		(efi_simple_text_output_protocol**)&ins->cust_data
	);
	if(efi_error(st))return st;
	update_system_table_by_fd(
		ins->cfg->config.int32,
		ctx->st,ins->handle,
		ins->cust_data
	);
	return efi_success;
}

static const efi_service efi_posix_fd_simple_text_in_service={
	.guid     = &gEfiWrapperPosixFDSimpleTextInProtocolGuid,
	.name     = "PosixFDSimpleTextInProtocol",
	.provides = {
		&gEfiSimpleTextInProtocolGuid,
		&gEfiSimpleTextInputExProtocolGuid,
		NULL
	},
	.depends  = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start    = svc_start_in,
};

static const efi_service efi_posix_fd_simple_text_out_service={
	.guid     = &gEfiWrapperPosixFDSimpleTextOutProtocolGuid,
	.name     = "PosixFDSimpleTextOutProtocol",
	.provides = {
		&gEfiSimpleTextOutProtocolGuid,
		NULL
	},
	.depends  = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start    = svc_start_out,
};

static const efi_service_cfg efi_posix_fd_simple_text_in_service_stdin={
	.name         = "PosixStdIn",
	.service      = &efi_posix_fd_simple_text_in_service,
	.config.int32 = STDIN_FILENO,
};

static const efi_service_cfg efi_posix_fd_simple_text_out_service_stdout={
	.name         = "PosixStdOut",
	.service      = &efi_posix_fd_simple_text_out_service,
	.config.int32 = STDOUT_FILENO,
};

static const efi_service_cfg efi_posix_fd_simple_text_out_service_stderr={
	.name         = "PosixStdErr",
	.service      = &efi_posix_fd_simple_text_out_service,
	.config.int32 = STDERR_FILENO,
};

DECLARE_EFI_SERVICE_CONFIG(efi_posix_fd_simple_text_in_service_stdin)
DECLARE_EFI_SERVICE_CONFIG(efi_posix_fd_simple_text_out_service_stdout)
DECLARE_EFI_SERVICE_CONFIG(efi_posix_fd_simple_text_out_service_stderr)
