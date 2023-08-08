//
// Created by bigfoot on 2023/4/3.
//

#ifndef EFI_WRAPPER_LOGGER_H
#define EFI_WRAPPER_LOGGER_H
#include"defines.h"
#include"efi/efi_status.h"
#include"efi/efi_string.h"
typedef enum log_type{
	LOG_ERROR = 0x01,
	LOG_DUMP  = 0x02,
	LOG_CALL  = 0x04,
	LOG_INFO  = 0x08,
}log_type;
typedef enum log_call_arg_type{
	arg_signed,
	arg_unsigned,
	arg_hex,
	arg_bool,
	arg_guid,
	arg_ptr,
	arg_str8,
	arg_str16,
	arg_status,
	arg_handle,
	arg_device_path,
}log_call_arg_type;
typedef union value_collection{
	void*ptr;
	char*str;
	char8*str8;
	char16*str16;
	size_t size;
	ssize_t ssize;
	efi_status status;
	bool boolean;
	int8_t int8;
	int16_t int16;
	int32_t int32;
	int64_t int64;
	uint8_t uint8;
	uint16_t uint16;
	uint32_t uint32;
	uint64_t uint64;
	float f;
	double d;
	int i;
	long l;
	long long ll;
	unsigned int ui;
	unsigned long ul;
	unsigned long long ull;
}value_collection;
static_assert(sizeof(value_collection)==8,"value_collection size mismatch");
typedef struct log_call_arg{
	log_call_arg_type type;
	char name[64];
	size_t length;
	value_collection value;
}log_call_arg;
extern uint8_t log_call_level;
#define call_arg(_type,_arg)&(log_call_arg){.type=arg_##_type,.name=#_arg,.length=sizeof(_arg),.value.uint64=(uint64_t)_arg}
extern void stack_backtrace(log_type type,int level);
extern int xlog_open(const char*log);
extern void xlog_call_stack(int level);
extern void xlog_set_verbose(bool verbose);
extern void xlog_set_print_call_stack(bool print);
extern void xlog_set_type(log_type type,bool enabled);
extern void xlog_add_skip_func(const char*func);
extern void xlog_add_skip_file(const char*file);
extern bool xlog_type_enabled(log_type type);
extern int xlog_call_ex(const char*file,const char*func,int line,...);
extern int xlog_print(const char*file,const char*func,int line,log_type type,const char*str);
extern int xlog_printf(const char*file,const char*func,int line,log_type type,const char*fmt,...) cdecl_attr_printf(5,6);
#define log_return(st)do{\
	efi_status _st=(st);\
	log_call_level--;\
	xlog(\
		LOG_CALL,"%s return with 0x%zx (%s)",\
		__func__,_st,efi_status_to_string(_st)\
	);\
	return _st;\
}while(0)
#define log_return_ptr(ptr)do{\
	void*_ptr=(ptr);\
	log_call_level--;\
	if(_ptr)xlog(\
		LOG_CALL,"%s return pointer 0x%012zx",\
		__func__,(size_t)_ptr\
	);\
	else xlog(LOG_CALL,"%s return null pointer",__func__);\
	return (typeof (ptr))_ptr;\
}while(0)
#define log_return_bool(boolean)do{\
	bool _boolean=(boolean);\
	log_call_level--;\
	xlog(\
		LOG_CALL,"%s return boolean %s",\
		__func__,_boolean?"true":"false"\
	);\
	return _boolean;\
}while(0)
#define log_return_void()do{\
	log_call_level--;\
	xlog(\
		LOG_CALL,"%s return none",\
		__func__\
	);\
	return;\
}while(0)
#define log_return_signed(type,num)do{\
	type _num=(num);\
	log_call_level--;\
	xlog(\
		LOG_CALL,"%s return number %lld (0x%012llx)",\
		__func__,\
		(long long)_num,\
		(long long)_num\
	);\
	return _num;\
}while(0)
#define log_return_unsigned(type,num)do{\
	type _num=(num);\
	log_call_level--;\
	xlog(\
		LOG_CALL,"%s return number %llu (0x%012llx)",\
		__func__,\
		(unsigned long long)_num,\
		(unsigned long long)_num\
	);\
	return _num;\
}while(0)
#define log_call(args...)xlog_call_ex(__FILE__,__func__,__LINE__,args)
#define xlog(type,fmt...)xlog_printf(__FILE__,__func__,__LINE__,type,fmt)
#define xerror(fmt...)xlog(LOG_ERROR,fmt)
#endif //EFI_WRAPPER_LOGGER_H
