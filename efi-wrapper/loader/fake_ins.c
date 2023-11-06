//
// Created by bigfoot on 2023/10/25.
//

#include<signal.h>
#include"defines.h"
#include"efi/efi_file.h"
#include"efi/efi_string.h"

weak_func bool proc_fake_ins(int sig,siginfo_t*info,ucontext_t*uc){
	(void)sig;
	(void)info;
	(void)uc;
	return false;
}
