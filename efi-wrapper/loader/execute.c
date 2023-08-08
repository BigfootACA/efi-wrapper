#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<setjmp.h>
#include<sys/prctl.h>
#include<sys/wait.h>
#include"efi/efi_file.h"
#include"efi/efi_string.h"
#include"efi/device_path.h"

extern void signal_hand(int sig,siginfo_t*info,void*d);

static efi_status efi_entry_invoke(
	efi_file*file,
	efi_handle handle,
	efi_system_table*table
){
	file->st=efi_not_started;
	if(efi_running_push(file)){
		if(setjmp(file->jmp)==0)
			file->st=file->entry(handle,table);
		efi_running_remove(file);
	}
	return file->st;
}

efi_status efi_run_with(
	efi_file*file,
	bool fork_run,
	efi_handle handle,
	efi_system_table*table
){
	pid_t p;
	efi_run_context*ctx;
	efi_status st=efi_success;
	struct sigaction sa,old[10];
	int sig[]={
		SIGQUIT,SIGTRAP,SIGXCPU,SIGXFSZ,SIGABRT,
		SIGSYS,SIGSEGV,SIGILL,SIGBUS,SIGFPE,0
	};
	int fds[2]={-1,-1},rt=0;
	if(!file)return efi_invalid_parameter;
	if(!file->entry){
		xerror("entrypoint not loaded");
		return efi_invalid_parameter;
	}
	if(fork_run&&pipe(fds)!=0){
		xerror("pipe failed: %m");
		return efi_load_error;
	}
	if(file->name[0]&&file->path[0])
		xlog(LOG_INFO,"Starting efi %s (%s)",file->name,file->path);
	if(file->context)ctx=efi_current_ctx,efi_current_ctx=file->context;
	if(!table)table=efi_current_ctx->st;
	if(!table)return efi_not_ready;
	xlog(LOG_INFO,"Invoke efi entry point 0x%zx with (0x%zx, 0x%zx)...",
		(size_t)file->entry,(size_t)handle,(size_t)table);
	if(!fork_run){
		sigemptyset(&sa.sa_mask);
		sa.sa_sigaction=&signal_hand;
		sa.sa_flags=SA_SIGINFO|SA_RESETHAND;
		for(size_t i=0;sig[i];i++)sigaction(sig[i],&sa,&old[i]);
		st=efi_entry_invoke(file,handle,table);
		for(size_t i=0;sig[i];i++)sigaction(sig[i],&old[i],NULL);
	}else switch((p=fork())){
		case -1:
			xerror("fork failed: %m");
			close(fds[0]);
			close(fds[1]);
			st=efi_aborted;
		break;
		case 0:
			close(fds[0]);
			prctl(PR_SET_NAME,file->name?:"[Unknown EFI]");
			st=efi_entry_invoke(file,handle,table);
			if(write(fds[1],&st,sizeof(st))!=sizeof(st))abort();
			close(fds[1]);
			_exit(0);
		break;
		default:
			close(fds[1]);
			do{waitpid(p,&rt,0);}while(errno==EINTR);
			if(read(fds[0],&st,sizeof(st))!=sizeof(st)){
				if(errno==0)errno=EIO;
				xerror("failed to read status");
			}
			close(fds[0]);
			if(WIFSIGNALED(rt)){
				xerror("efi runner crash with %d",WTERMSIG(rt));
				st=efi_aborted;
			}
		break;
	}
	if(file->context)efi_current_ctx=ctx;
	xlog(LOG_INFO,"efi return 0x%zx (%s)",st,efi_status_to_string(st));
	return st;
}

efi_status efi_run(efi_file*file,bool fork_run){
	efi_handle hand;
	if(!file||!file->entry||!file->context)return efi_invalid_parameter;
	if(!(hand=efi_file_get_image_handle(file)))return efi_load_error;
	return efi_run_with(file,fork_run,hand,NULL);
}
