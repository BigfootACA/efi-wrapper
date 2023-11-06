//
// Created by bigfoot on 2023/10/25.
//

#include<stdio.h>
#include<stddef.h>
#include<stdbool.h>
#include<udisasm.h>
#ifdef __aarch64__
#include"aarch64.h"
#define udisasm_available true
#else
#define udisasm_available false
uint64_t disasm(uint64_t addr,char*str){
	(void)addr;
	(void)str;
	return addr;
}
#endif
bool udisasm_is_available(void){return udisasm_available;}
