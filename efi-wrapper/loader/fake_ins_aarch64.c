//
// Created by bigfoot on 2023/10/25.
//

#if defined(__aarch64__)
#include<signal.h>
#include<unistd.h>
#include"defines.h"
#include"efi/efi_file.h"
#include"efi/efi_string.h"
#include "udisasm.h"

struct fake_reg{
	uint64_t currentel;
	uint64_t sp_el0;
	uint64_t sctlr_el1;
	uint64_t cpacr_el1;
	uint64_t ttbr0_el1;
	uint64_t ttbr1_el1;
	uint64_t tcr_el1;
	uint64_t mair_el1;
	uint64_t vbar_el1;
	uint64_t tpidr_el1;
	uint64_t pmcr_el0;
	uint64_t pmccntr_el0;
	uint64_t pmovsclr_el0;
	uint64_t pmcntenclr_el0;
	uint64_t pmccfiltr_el0;
	uint64_t pmcntenset_el0;
	uint64_t pmintenclr_el1;
	uint64_t pmuserenr_el0;
	uint64_t par_el1;
	uint64_t daif;
};

static bool decode8(struct fake_reg*reg,ucontext_t*uc,uint32_t ins){
	(void)uc;
	switch(ins&0xFFFFF0FF){
		case 0xd50340df: // msr daifset, #?
			reg->daif|=(ins&0xf00)>>8;
		break;
		case 0xd50340ff: // msr daifclr, #?
			reg->daif&=~((ins&0xf00)>>8);
		break;
		default:return false;
	}
	return true;
}

#define PERM_RO R_OK
#define PERM_WO W_OK
#define PERM_RW R_OK|W_OK

static struct sys_reg_map{
	uint32_t code;
	int perm;
	uintptr_t offset;
	int(*handler)(int mode,struct fake_reg*reg,ucontext_t*uc,uint32_t ins);
}sys_reg_maps[]={
	{0xbefe0, PERM_RW, offsetof(struct fake_reg,pmccfiltr_el0),  NULL},
	{0xb9e00, PERM_RW, offsetof(struct fake_reg,pmuserenr_el0),  NULL},
	{0xb9d00, PERM_RW, offsetof(struct fake_reg,pmccntr_el0),    NULL},
	{0xb9c60, PERM_WO, offsetof(struct fake_reg,pmovsclr_el0),   NULL},
	{0xb9c40, PERM_WO, offsetof(struct fake_reg,pmcntenclr_el0), NULL},
	{0xb9c20, PERM_WO, offsetof(struct fake_reg,pmcntenset_el0), NULL},
	{0xb9c00, PERM_RW, offsetof(struct fake_reg,pmcr_el0),       NULL},
	{0xb4220, PERM_RW, offsetof(struct fake_reg,daif),           NULL},
	{0x8d080, PERM_RW, offsetof(struct fake_reg,tpidr_el1),      NULL},
	{0x8c000, PERM_RW, offsetof(struct fake_reg,vbar_el1),       NULL},
	{0x8a200, PERM_RW, offsetof(struct fake_reg,mair_el1),       NULL},
	{0x89e40, PERM_RW, offsetof(struct fake_reg,pmintenclr_el1), NULL},
	{0x87400, PERM_RO, offsetof(struct fake_reg,par_el1),        NULL},
	{0x84240, PERM_RO, offsetof(struct fake_reg,currentel),      NULL},
	{0x84100, PERM_RW, offsetof(struct fake_reg,sp_el0),         NULL},
	{0x82040, PERM_RW, offsetof(struct fake_reg,tcr_el1),        NULL},
	{0x82020, PERM_RW, offsetof(struct fake_reg,ttbr1_el1),      NULL},
	{0x82000, PERM_RW, offsetof(struct fake_reg,ttbr0_el1),      NULL},
	{0x81040, PERM_RW, offsetof(struct fake_reg,cpacr_el1),      NULL},
	{0x81000, PERM_RW, offsetof(struct fake_reg,sctlr_el1),      NULL},
};

static bool decode_sysreg(struct fake_reg*reg,ucontext_t*uc,uint32_t ins){
	int mode,r;
	struct sys_reg_map*map=NULL;
	switch((ins&0xFFF00000)>>20){
		case 0xd51:mode=W_OK;break;// msr
		case 0xd53:mode=R_OK;break;// mrs
		default:return false;
	}
	uint32_t code=ins&0x000FFFE0;
	for(size_t i=0;i<ARRAYLEN(sys_reg_maps);i++)
		if(sys_reg_maps[i].code==code)
			map=&sys_reg_maps[i];
	if(!map)return false;
	if(mode==W_OK&&!(map->perm&W_OK))return false;
	if(mode==R_OK&&!(map->perm&R_OK))return false;
	uint64_t*dest=(void*)reg+map->offset;
	r=map->handler?map->handler(mode,reg,uc,ins):1;
	if(r==0)return true;
	if(r==1)switch(mode){
		case R_OK:uc->uc_mcontext.regs[ins&0x1F]=*dest;return true;
		case W_OK:*dest=uc->uc_mcontext.regs[ins&0x1F];return true;
	}
	return false;
}

static bool decodeall(struct fake_reg*reg,ucontext_t*uc,uint32_t ins){
	(void)uc;
	switch(ins){
		case 0xd5088708:break; // tlbi vmalle1
		case 0xd5087620 ... 0xd508763e:break; // dc ivac, x?
		case 0xd5087800 ... 0xd508781e: // at s1e1r, x?
			reg->par_el1=uc->uc_mcontext.regs[ins&0x1F];
		break;
		default:return false;;
	}
	return true;
}

bool proc_fake_ins(int sig,siginfo_t*info,ucontext_t*uc){
	void*ptr;
	uint32_t ins=0;
	bool found=false;
	struct fake_reg*reg;
	if(sig!=SIGILL)return false;
	ptr=info->si_addr;
	if(!efi_current_ctx)return false;
	if(!efi_running_lookup_ptr((size_t)ptr))return false;
	if(!(reg=efi_current_ctx->fake_reg)){
		size_t s=sizeof(struct fake_reg);
		if(!(reg=malloc(s)))return false;
		memset(reg,0,s);
		efi_current_ctx->fake_reg=reg;
		reg->currentel=0x4; // fake el1
	}
	memcpy(&ins,ptr,sizeof(ins));
	if(!found)found=decode8(reg,uc,ins);
	if(!found)found=decode_sysreg(reg,uc,ins);
	if(!found)found=decodeall(reg,uc,ins);
	if(found)uc->uc_mcontext.pc+=sizeof(ins);
	return found;

}
#endif
