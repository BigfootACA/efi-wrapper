#define _GNU_SOURCE
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include"list.h"
#include"logger.h"
#include"defines.h"
#include"efi/efi_spec.h"
#include"efi/efi_string.h"

uint8_t log_call_level=0;
static int fd=STDOUT_FILENO;
static bool verbose_info=false;
static bool print_call_stack=false;
static list*skip_func=NULL;
static list*skip_file=NULL;
static uint64_t log_mask=0xFFFFFFFFFFFFFFFF;

static const char*xlog_type_str(log_type type){
	switch(type){
		case LOG_ERROR:return "ERROR";
		case LOG_DUMP:return "DUMP";
		case LOG_CALL:return "CALL";
		case LOG_INFO:return "INFO";
		default:return "UNKNOWN";
	}
}

int xlog_print(
	const char*file,const char*func,int line,
	log_type type,const char*str
){
	int err=errno;
	static struct timeval old={0,0};
	struct timeval tv,off;
	if(!xlog_type_enabled(type))return 0;
	if(list_search_case_string(skip_func,func))return 0;
	if(list_search_case_string(skip_file,file))return 0;
	for(int i=0;i<log_call_level;i++)dprintf(fd,"  ");
	if(verbose_info){
		gettimeofday(&tv,NULL);
		off.tv_sec=tv.tv_sec-old.tv_sec;
		off.tv_usec=tv.tv_usec-old.tv_usec;
		memcpy(&old,&tv,sizeof(tv));
		errno=err;
		dprintf(
			fd," <%s@%s:%d %d +%ld.%ld [%s]> \n",
			file,func,line,gettid(),off.tv_sec,
			off.tv_usec,xlog_type_str(type)
		);
	}
	errno=err;
	dprintf(fd,"%s\n",str);
	errno=err;
	return 0;
}

int xlog_printf(
	const char*file,const char*func,int line,
	log_type type,const char*fmt,...
){
	int r;
	char*str;
	va_list va;
	int err=errno;
	if(!xlog_type_enabled(type))return 0;
	if(list_search_case_string(skip_func,func))return 0;
	if(list_search_case_string(skip_file,file))return 0;
	va_start(va,fmt);
	errno=err;
	r=vasprintf(&str,fmt,va);
	va_end(va);
	if(r<0)return r;
	errno=err;
	r=xlog_print(file,func,line,type,str);
	free(str);
	errno=err;
	return r;
}

int xlog_call_ex(const char*file,const char*func,int line,...){
	int r=0;
	va_list va;
	size_t len,i;
	log_call_arg*arg;
	char buff[8192],var[256],*ptr;
	memset(buff,0,sizeof(buff));
	if(!xlog_type_enabled(LOG_CALL))goto done;
	if(list_search_case_string(skip_func,func))goto done;
	if(list_search_case_string(skip_file,file))goto done;
	strlcat(buff,func,sizeof(buff)-1);
	strlcat(buff," ",sizeof(buff)-1);
	va_start(va,line);
	while((arg=va_arg(va,log_call_arg*))){
		memset(var,0,sizeof(var));
		strncpy(var,arg->name,sizeof(var)-1);
		strlcat(var,"=",sizeof(var)-1);
		i=strlen(var),ptr=var+i,len=sizeof(var)-i-1;
		switch(arg->type){
			case arg_signed:snprintf(ptr,len,"%lld",arg->value.ll);break;
			case arg_unsigned:snprintf(ptr,len,"%llu",arg->value.ull);break;
			case arg_hex:snprintf(ptr,len,"%llx",arg->value.ull);break;
			case arg_bool:snprintf(ptr,len,"%s",BOOL2STR(arg->value.boolean));break;
			case arg_guid:{
				guid*guid=arg->value.ptr;
				if(!guid)snprintf(ptr,len,"NULL");
				else efi_guid_to_readable_string(guid,ptr,len);
			}break;
			case arg_str16:
				if(!arg->value.ptr)snprintf(ptr,len,"NULL");
				else ustr2xstr(ptr,len,arg->value.str16);
			break;
			case arg_handle:
			case arg_device_path:
			case arg_ptr:snprintf(ptr,len,"0x%llx",arg->value.ull);break;
			case arg_str8:
				if(!arg->value.ptr)snprintf(ptr,len,"NULL");
				else snprintf(ptr,len,"%s",arg->value.str8);
			break;
			case arg_status:snprintf(
				ptr,len,"0x%llx (%s)",
				arg->value.ull,
				efi_status_to_string(arg->value.status)
			);break;
			default:snprintf(
				ptr,len,"0x%llx (unknown)",
				arg->value.ull
			);break;
		}
		strlcat(var," ",sizeof(var)-1);
		strlcat(buff,var,sizeof(buff)-1);
	}
	va_end(va);
	r=xlog_print(file,func,line,LOG_CALL,buff);
	xlog_call_stack(3);
	done:log_call_level++;
	return r;
}

void xlog_call_stack(int level){
	if(print_call_stack)stack_backtrace(LOG_CALL,level);
}

int xlog_open(const char*log){
	int r;
	r=open(log,O_CREAT|O_TRUNC|O_WRONLY,0644);
	if(r<0)return r;
	if(fd>2)close(fd);
	return (fd=r);
}

void xlog_set_verbose(bool verbose){
	verbose_info=verbose;
}

void xlog_set_print_call_stack(bool print){
	print_call_stack=print;
}

void xlog_set_type(log_type type,bool enabled){
	if(enabled)log_mask|=type;
	else log_mask&=~type;
}

void xlog_add_skip_func(const char*func){
	if(func&&*func&&!list_search_case_string(skip_func,func))
		list_obj_add_new_strdup(NULL,&skip_func,func);
}

void xlog_add_skip_file(const char*file){
	if(file&&*file&&!list_search_case_string(skip_file,file))
		list_obj_add_new_strdup(NULL,&skip_file,file);
}

bool xlog_type_enabled(log_type type){
	return (log_mask&type)==type;
}
