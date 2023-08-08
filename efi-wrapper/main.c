#include<unistd.h>
#include<getopt.h>
#include<signal.h>
#include<termios.h>
#include<sys/ioctl.h>
#include"defines.h"
#include"logger.h"
#include"efi/efi_file.h"
#include"efi/efi_context.h"
#include"efi/efi_template.h"
#include"efi/protocols/graphics_output.h"

struct winsize ws_size;
static struct termios old;
static bool need_restore=false;

static int usage(){
	puts(
		"Usage: efi-wrapper [-o <FILE>] EFI...\n"
		"Options:\n"
		"\t-o, --output <FILE>    Write log to file\n"
		"\t-u, --skip-func <FUNC> Skip log print by func\n"
		"\t-f, --skip-file <FILE> Skip log print by file\n"
		"\t-v, --verbose          Print addition time, function info\n"
		"\t-b, --backtrace        Print call stack backtrace\n"
		"\t-p, --permissive       Memory permissive mode (all RWX)\n"
		"\t-s, --sdl2             Add SDL2 for UEFI Graphics Output (GOP)\n"
		"\t-c, --call             Print all EFI calls\n"
		"\t-h, --help             Display this help and exit\n"
	);
	return 0;
}

static void winch_hand(int sig cdecl_attr_unused){
	ioctl(STDIN_FILENO,TIOCGWINSZ,&ws_size);
}

void restore_terminal(){
	if(!need_restore)return;
	tcsetattr(STDIN_FILENO,TCSANOW,&old);
	winch_hand(SIGWINCH);
}


int main(int argc,char**argv){
	int o;
	efi_file*f=NULL;
	efi_run_context*ctx=NULL;
	efi_service*svc;
	bool permissive=false;
	static const struct option lo[]={
		{"output",     required_argument, NULL, 'o'},
		{"skip-func",  required_argument, NULL, 'u'},
		{"skip-file",  required_argument, NULL, 'f'},
		{"backtrace",  no_argument,       NULL, 'b'},
		{"permissive", no_argument,       NULL, 'p'},
		{"verbose",    no_argument,       NULL, 'v'},
		{"call",       no_argument,       NULL, 'c'},
		{"sdl2",       no_argument,       NULL, 's'},
		{"help",       no_argument,       NULL, 'h'},
		{NULL,0,NULL,0}
	};
	xlog_set_verbose(false);
	xlog_set_print_call_stack(false);
	xlog_set_type(LOG_CALL,false);
	while((o=getopt_long(argc,argv,"o:u:f:vschbp",lo,NULL))>0)switch(o){
		case 'o':
			if(xlog_open(optarg)<0)
				fprintf(stderr,"open %s failed: %m\n",optarg);
		break;
		case 's':
			if((svc=efi_service_lookup_service(
				&gEfiWrapperSDL2GraphicsOutputProtocolGuid
			)))svc->disabled=false;
		break;
		case 'p':permissive=true;break;
		case 'u':xlog_add_skip_func(optarg);break;
		case 'f':xlog_add_skip_file(optarg);break;
		case 'c':xlog_set_type(LOG_CALL,true);break;
		case 'v':xlog_set_verbose(true);break;
		case 'b':xlog_set_print_call_stack(true);break;
		case 'h':return usage();
		default:return 1;
	}
	if(optind>=argc){
		fprintf(stderr,"no file specified to run\n");
		return 1;
	}
	atexit(restore_terminal);
	signal(SIGWINCH,winch_hand);
	ws_size.ws_col=80,ws_size.ws_row=24;
	if(isatty(STDIN_FILENO)){
		if(tcgetattr(STDIN_FILENO,&old)==0)
			need_restore=true;
		winch_hand(SIGWINCH);
	}
	if(!(ctx=efi_new_run_context()))return -1;
	for(int i=optind;i<argc;i++){
		if(!f&&!(f=efi_file_new(ctx)))continue;
		f->permissive=permissive;
		if(!efi_load_by_path(f,argv[i]))continue;
		efi_run(f,false);
		f=NULL;
	}
	efi_destroy_context(ctx);
	return 0;
}
