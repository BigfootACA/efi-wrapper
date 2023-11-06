#ifndef EFI_WRAPPER_UDISASM_H
#define EFI_WRAPPER_UDISASM_H
#include<stdbool.h>
#include<stdint.h>
extern bool udisasm_is_available(void);
extern uint64_t disasm(uint64_t addr,char*str);
#endif
