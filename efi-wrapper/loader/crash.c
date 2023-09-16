//
// Created by bigfoot on 2023/5/13.
//

#include<signal.h>
#include<unistd.h>
#include"defines.h"
#include"efi/efi_file.h"
#include"efi/efi_string.h"
#ifdef __GLIBC__
#include<execinfo.h>
#define BACKTRACE_SIZE 16
#endif

extern const char*audit_arch_to_string(unsigned int arch,const char*def);
#ifdef __GLIBC__
static bool dump_efi(log_type type,size_t ptr,int level){
	efi_symbol sym;
	size_t off=0,addr;
	char buff1[256],buff2[4096];
	efi_file*f=efi_running_lookup_ptr(ptr);
	if(!f)return false;
	addr=ptr-(size_t)f->image;
	memset(buff1,0,sizeof(buff1));
	memset(buff2,0,sizeof(buff2));
	if(efi_lookup_symbol(f,&sym,addr,&off)){
		snprintf(
			buff1,sizeof(buff1)-1,
			"[%s(0x%06zx) + 0x%zx]",
			sym.name,sym.address,off
		);
		if(sym.src[0])snprintf(
			buff2,sizeof(buff2)-1,
			"%s:%zu",sym.src,sym.line
		);
	}
	xlog(type,
		"  #%-2d (0x%012zx) EFI(0x%012zx) %s + 0x%06zx %s %s",
		level,(size_t)ptr,(size_t)f->image,
		f->name,addr,buff1,buff2
	);
	return true;
}

void stack_backtrace(log_type type,int level){
	void*b[BACKTRACE_SIZE];
	char**s;
	int n=backtrace(b,BACKTRACE_SIZE);
	if(!(s=backtrace_symbols(b,n))){
		xerror("backtrace_symbols failed");
		return;
	}
	for(int j=level;j<n;j++){
		if(dump_efi(type,(size_t)b[j],j-level))continue;
		xlog(type,
			"  #%-2d (0x%012zx) %s",
			j-level,(size_t)b[j],s[j]
		);
	}
	free(s);
}
#else
void stack_backtrace(log_type type,int level){
	xerror("stack backtrace not available");
}
#endif

void print_mem_pool_usage(mem_pool*pool,const char*name,...){
	va_list va;
	char buff1[64],buff2[64],buff3[128];
	if(!pool||!name)return;
	va_start(va,name);
	memset(buff3,0,sizeof(buff3));
	vsnprintf(buff3,sizeof(buff3)-1,name,va);
	va_end(va);
	xlog(
		LOG_INFO,"%s usage: %zu pools, size %zu bytes / %s, real %zu bytes / %s",
		buff3,pool->count,
		pool->alloc_size,make_readable_str_buf(buff1,sizeof(buff1),pool->alloc_size,1,0),
		pool->used_size,make_readable_str_buf(buff2,sizeof(buff2),pool->used_size,1,0)
	);
}

static void dump_address_status(size_t addr){
	size_t off1=0;
	pe_section_header*hdr;
	efi_file*f=efi_running_lookup_ptr((size_t)addr);
	if(!f)return;
	xerror("Address in EFI %s offset 0x%08zx",f->name,off1);
	if((hdr=efi_lookup_section(f,off1,NULL))){
		xerror(
			"Address in section %s offset 0x%08zx",
			hdr->name,off1-hdr->virtual_address
		);
	}else xerror("Address not in any sections");
}

void signal_hand(int sig,siginfo_t*info,void*d){
	efi_memory_type type;
	static bool catch=false;
	(void)d;
	if(catch){
		xerror("force quit while loop crash");
		_exit(-1);
	}
	restore_terminal();
	log_call_level=0;
	xerror("****************************************");
	xerror("Catch fatal exception %s",strsignal(sig));
	xerror("Signal: %d",info->si_errno);
	xerror("Code: %d",info->si_code);
	xerror("ErrNo: %d (%s)",info->si_errno,strerror(info->si_errno));
	xerror("Reason: %s",sigreason(sig,info->si_code,"(unknown)"));
	switch(sig){
		case SIGQUIT:case SIGTRAP:case SIGXCPU:case SIGXFSZ:case SIGABRT:break;
		case SIGSYS:
			xerror("Call address: 0x%012zx",(size_t)info->si_call_addr);
			xerror("Syscall number: %d (0x%04x)",info->si_syscall,info->si_syscall);
			xerror("Syscall arch: %s (0x%04x)",audit_arch_to_string(info->si_arch,"Unknown"),info->si_arch);
			dump_address_status((size_t)info->si_call_addr);

		break;
		case SIGSEGV:case SIGILL:case SIGBUS:case SIGFPE:
			xerror("Fault address: 0x%012zx",(size_t)info->si_addr);
			dump_address_status((size_t)info->si_addr);
		break;
		default:sig=SIGABRT;break;
	}
	print_mem_pool_usage(mem_get_global_pool(),"Global memory pool");
	if(efi_current_ctx)for(
		type=efi_reserved_memory_type;
		type<efi_max_memory_type;
		type++
	)print_mem_pool_usage(
		efi_current_ctx->mem[type].pool,
		"EFI context memory pool %s",
		efi_memory_type_to_string(type)
	);
	xerror("Stack backtrace:");
	stack_backtrace(LOG_ERROR,3);
	xerror("Creating core dump...");
	raise(sig);
}
