//
// Created by bigfoot on 2023/5/7.
//

const char*sigreason(int sig,int code,const char*def){
	switch(code){
		case -60/*SI_ASYNCNL*/:return "(SI_ASYNCNL) Sent by asynch name lookup completion";
		case -7/*SI_DETHREAD*/:return "(SI_DETHREAD) Sent by execve killing subsidiary threads";
		case -6/*SI_TKILL*/:return "(SI_TKILL) Sent by tkill";
		case -5/*SI_SIGIO*/:return "(SI_SIGIO) Sent by queued SIGIO";
		case -4/*SI_ASYNCIO*/:return "(SI_ASYNCIO) Sent by AIO completion";
		case -3/*SI_MESGQ*/:return "(SI_MESGQ) Sent by real time mesq state change";
		case -2/*SI_TIMER*/:return "(SI_TIMER) Sent by timer expiration";
		case -1/*SI_QUEUE*/:return "(SI_QUEUE) Sent by sigqueue";
		case 0/*SI_USER*/:return "(SI_USER) Sent by kill, sigsend";
		case 128/*SI_KERNEL*/:return "(SI_KERNEL) Send by kernel";
	}
	switch(sig){
		case 4/*SIGILL*/:switch(code){
			case 1/*ILL_ILLOPC*/:return "(ILL_ILLOPC) Illegal opcode";
			case 2/*ILL_ILLOPN*/:return "(ILL_ILLOPN) Illegal operand";
			case 3/*ILL_ILLADR*/:return "(ILL_ILLADR) Illegal addressing mode";
			case 4/*ILL_ILLTRP*/:return "(ILL_ILLTRP) Illegal trap";
			case 5/*ILL_PRVOPC*/:return "(ILL_PRVOPC) Privileged opcode";
			case 6/*ILL_PRVREG*/:return "(ILL_PRVREG) Privileged register";
			case 7/*ILL_COPROC*/:return "(ILL_COPROC) Coprocessor error";
			case 8/*ILL_BADSTK*/:return "(ILL_BADSTK) Internal stack error";
			case 9/*ILL_BADIADDR*/:return "(ILL_BADIADDR) Unimplemented instruction address";
			case 10/*ILL_BREAK*/:return "(ILL_BREAK) Illegal break";
			case 11/*ILL_BNDMOD*/:return "(ILL_BNDMOD) Bundle-update (modification) in progress";
		}break;
		case 5/*SIGTRAP*/:switch(code){
			case 1/*TRAP_BRKPT*/:return "(TRAP_BRKPT) Process breakpoint";
			case 2/*TRAP_TRACE*/:return "(TRAP_TRACE) Process trace trap";
			case 3/*TRAP_BRANCH*/:return "(TRAP_BRANCH) Process taken branch trap";
			case 4/*TRAP_HWBKPT*/:return "(TRAP_HWBKPT) Hardware breakpoint/watchpoint";
			case 5/*TRAP_UNK*/:return "(TRAP_UNK) Undiagnosed trap";
			case 6/*TRAP_PERF*/:return "(TRAP_PERF) Perf event with sigtrap=1";
		}break;
		case 7/*SIGBUS*/:switch(code){
			case 1/*BUS_ADRALN*/:return "(BUS_ADRALN) Invalid address alignment";
			case 2/*BUS_ADRERR*/:return "(BUS_ADRERR) Non-existent physical address";
			case 3/*BUS_OBJERR*/:return "(BUS_OBJERR) Object specific hardware error";
			case 4/*BUS_MCEERR_AR*/:return "(BUS_MCEERR_AR) Hardware memory error consumed on a machine check";
			case 5/*BUS_MCEERR_AO*/:return "(BUS_MCEERR_AO) Hardware memory error detected in process but not consumed";
		}break;
		case 8/*SIGFPE*/:switch(code){
			case 1/*FPE_INTDIV*/:return "(FPE_INTDIV) Integer divide by zero";
			case 2/*FPE_INTOVF*/:return "(FPE_INTOVF) Integer overflow";
			case 3/*FPE_FLTDIV*/:return "(FPE_FLTDIV) Floating point divide by zero";
			case 4/*FPE_FLTOVF*/:return "(FPE_FLTOVF) Floating point overflow";
			case 5/*FPE_FLTUND*/:return "(FPE_FLTUND) Floating point underflow";
			case 6/*FPE_FLTRES*/:return "(FPE_FLTRES) Floating point inexact result";
			case 7/*FPE_FLTINV*/:return "(FPE_FLTINV) Floating point invalid operation";
			case 8/*FPE_FLTSUB*/:return "(FPE_FLTSUB) Subscript out of range";
			case 9/*FPE_DECOVF*/:return "(FPE_DECOVF) Decimal overflow";
			case 10/*FPE_DECDIV*/:return "(FPE_DECDIV) Decimal division by zero";
			case 11/*FPE_DECERR*/:return "(FPE_DECERR) Packed decimal error";
			case 12/*FPE_INVASC*/:return "(FPE_INVASC) Invalid ASCII digit";
			case 13/*FPE_INVDEC*/:return "(FPE_INVDEC) Invalid decimal digit";
			case 14/*FPE_FLTUNK*/:return "(FPE_FLTUNK) Undiagnosed floating-point exception";
			case 15/*FPE_CONDTRAP*/:return "(FPE_CONDTRAP) Trap on condition";
		}break;
		case 11/*SIGSEGV*/:switch(code){
			case 1/*SEGV_MAPERR*/:return "(SEGV_MAPERR) Address not mapped to object";
			case 2/*SEGV_ACCERR*/:return "(SEGV_ACCERR) Invalid permissions for mapped object";
			case 3/*SEGV_BNDERR*/:return "(SEGV_BNDERR) Failed address bound checks";
			case 4/*SEGV_PKUERR*/:return "(SEGV_PKUERR) Failed protection key checks";
			case 5/*SEGV_ACCADI*/:return "(SEGV_ACCADI) ADI not enabled for mapped object";
			case 6/*SEGV_ADIDERR*/:return "(SEGV_ADIDERR) Disrupting MCD error";
			case 7/*SEGV_ADIPERR*/:return "(SEGV_ADIPERR) Precise MCD exception";
			case 8/*SEGV_MTEAERR*/:return "(SEGV_MTEAERR) Asynchronous ARM MTE error";
			case 9/*SEGV_MTESERR*/:return "(SEGV_MTESERR) Synchronous ARM MTE exception";
		}break;
		case 17/*SIGCHLD*/:switch(code){
			case 1/*CLD_EXITED*/:return "(CLD_EXITED) Child has exited";
			case 2/*CLD_KILLED*/:return "(CLD_KILLED) Child was killed";
			case 3/*CLD_DUMPED*/:return "(CLD_DUMPED) Child terminated abnormally";
			case 4/*CLD_TRAPPED*/:return "(CLD_TRAPPED) Traced child has trapped";
			case 5/*CLD_STOPPED*/:return "(CLD_STOPPED) Child has stopped";
			case 6/*CLD_CONTINUED*/:return "(CLD_CONTINUED) Stopped child has continued";
		}break;
		case 29/*SIGPOLL*/:switch(code){
			case 1/*POLL_IN*/:return "(POLL_IN) Data input available";
			case 2/*POLL_OUT*/:return "(POLL_OUT) Output buffers available";
			case 3/*POLL_MSG*/:return "(POLL_MSG) Input message available";
			case 4/*POLL_ERR*/:return "(POLL_ERR) I/O error";
			case 5/*POLL_PRI*/:return "(POLL_PRI) High priority input available";
			case 6/*POLL_HUP*/:return "(POLL_HUP) Device disconnected";
		}break;
		case 31/*SIGSYS*/:switch(code){
			case 1/*SYS_SECCOMP*/:return "(SYS_SECCOMP) Seccomp triggered";
			case 2/*SYS_USER_DISPATCH*/:return "(SYS_USER_DISPATCH) Syscall user dispatch triggered";
		}break;
	}
	return def;
}