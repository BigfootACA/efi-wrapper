//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_DEFINES_H
#define EFI_WRAPPER_DEFINES_H
#include<errno.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include"config.h"
#define PAGE_SIZE 4096
#if defined(__SIZEOF_POINTER__)
#define WORD_SIZE __SIZEOF_POINTER__
#elif defined(__WORDSIZE)
#define WORD_SIZE __WORDSIZE/8
#endif
#define EGO(expr,tag)do{expr;goto tag;}while(0)
#define EDONE(expr) EGO(expr,done)
#define ENUM(err) -(errno=(err))
#define ERET(err) do{return ENUM(err);}while(0)
#define EPRET(err) do{errno=(err);return NULL;}while(0)
#define RET(e) do{return (errno=(e));}while(0)
#define DONE(e) EDONE(errno=(e))
#define XDONE(e) EDONE(if(errno==0)errno=(e))
#define XRET(e,d) do{return (errno=((e)?:(d)));}while(0)
#define EXRET(e) do{return errno?:e}while(0)
#define EXPRET(e) do{if(errno==0)errno=e;return NULL;}while(0)
#define BOOL2STR(b) ((b)?"true":"false")
#define ARRAYLEN(a) (sizeof(a)/sizeof((a)[0]))
#ifndef MIN
#define MIN(a,b)({\
	typeof(a)_a=(a);\
	typeof(b)_b=(b);\
	(_b)>(_a)?(_a):(_b);\
})
#endif
#ifndef MAX
#define MAX(a,b)({\
	typeof(a)_a=(a);\
	typeof(b)_b=(b);\
	(_b)<(_a)?(_a):(_b);\
})
#endif
#ifndef IS_BETWEEN
#define IS_BETWEEN(val,min,max)({\
	uint64_t _val=(uint64_t)(val);\
	uint64_t _min=(uint64_t)(min);\
	uint64_t _max=(uint64_t)(max);\
	(_val)>(_min)&&(_val)<(_max);\
})
#endif
#ifndef IS_BETWEEN_MEM
#define IS_BETWEEN_MEM(addr,base,size)({\
	uint64_t _addr=(uint64_t)(uintptr_t)(addr);\
	uint64_t _base=(uint64_t)(uintptr_t)(base);\
	uint64_t _size=(uint64_t)(size);\
	IS_BETWEEN(_addr,_base,_base+_size);\
})

#endif
#define round_up(val,alg) (((val)+(alg)-1)&~((alg)-1))
#define round_down(val,alg) ((val)&~((alg)-1))
#define cdecl_attr(attr...) __attribute__((attr))
#define cdecl_attr_msabi cdecl_attr(ms_abi)
#define cdecl_attr_used cdecl_attr(used)
#define cdecl_attr_unused cdecl_attr(unused)
#define cdecl_attr_packed cdecl_attr(packed)
#define cdecl_attr_constructor cdecl_attr(constructor)
#define cdecl_attr_printf(fmt,va) cdecl_attr(format(printf,fmt,va))
#define header_func static inline cdecl_attr_unused
#define weak_func cdecl_attr(__weak__)
typedef int runnable_t(void*);
header_func size_t s_strlcpy(char *buf,const char*src,size_t len){
        char*d0=buf;
        if(!len--)goto finish;
        for(;len&&(*buf=*src);len--,src++,buf++);
        *buf=0;
	finish:
        return buf-d0+strlen(src);
}
header_func size_t s_strlcat(char*buf,const char*src,size_t len){
        size_t slen=strnlen(buf,len);
        return slen==len?
		(slen+strlen(src)):
		(slen+s_strlcpy(buf+slen,src,len-slen));
}
#undef strlcat
#undef strlcpy
#undef memdup
#define strlcat s_strlcat
#define strlcpy s_strlcpy
#define memdup s_memdup
extern const char*make_readable_str_buf(
	char*buf,size_t len,
	unsigned long long val,
	unsigned long block_size,
	unsigned long display
);
extern uint32_t s_crc32(void*buffer,size_t length);
extern const char*sigreason(int sig,int code,const char*def);
extern void restore_terminal();
#endif //EFI_WRAPPER_DEFINES_H
