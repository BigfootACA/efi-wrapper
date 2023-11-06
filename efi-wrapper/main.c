#include<elf.h>
#include<link.h>
#include<unistd.h>
#include<getopt.h>
#include<signal.h>
#include<termios.h>
#include<sys/ioctl.h>
#include"defines.h"
#include"logger.h"
#include"list.h"
#include"efi/efi_file.h"
#include"efi/efi_context.h"
#include"efi/efi_wrapper.h"
#include"efi/efi_template.h"
#include"efi/protocols/graphics_output.h"

struct winsize ws_size;
static struct termios old;
static bool need_restore=false;
list*efi_wrapper_code_self=NULL;

static int usage(){
	puts(
		"Usage: efi-wrapper [-o <FILE>] EFI...\n"
		"Options:\n"
		"\t-r, --root <ROOT>      Map path to UEFI filesystem\n"
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

static void add_fs(efi_run_context*ctx,list*root){
	list*l;
	efi_status st;
	efi_handle hand;
	efi_run_context*cur;
	if((l=list_first(root)))do{
		bool writable=false;
		const char*str;
		LIST_DATA_DECLARE(path,l,const char*);
		if(path[0]=='R'&&path[2]==':')switch(path[1]){
			case 'O':writable=false,path+=3;break;
			case 'W':writable=true,path+=3;break;
			default:fprintf(stderr,"invalid path %s\n",path);continue;
		}
		str=writable?"writable":"read-only";
		xlog(LOG_INFO,"add %s file system %s",str,path);
		cur=efi_current_ctx,efi_current_ctx=ctx;
		st=efi_new_simple_file_system_from_posix_path(path,writable,&hand,NULL);
		if(efi_current_ctx==ctx)efi_current_ctx=cur;
		if(efi_error(st))fprintf(
			stderr,"add %s file system %s failed: %s\n",
			str,path,efi_status_to_string(st)
		);
	}while((l=l->next));
}

static int fill_self_data(
	struct dl_phdr_info*info,
	size_t size cdecl_attr_unused,
	void*data cdecl_attr_unused
){
	size_t s=sizeof(struct efi_wrapper_code);
	struct efi_wrapper_code*code=malloc(s);
	if(!code)return -1;
	memset(code,0,s);
	list_obj_add_new(NULL,&efi_wrapper_code_self,code);
	code->base=info->dlpi_addr;
	for(ElfW(Half) i=0;i<info->dlpi_phnum;i++)list_obj_add_new_dup(
		NULL,&code->phdr,(void*)&info->dlpi_phdr[i],sizeof(ElfW(Phdr))
	);
	return 0;
}

int main(int argc,char**argv){
	int o;
	efi_file*f=NULL;
	list*root=NULL;
	efi_run_context*ctx=NULL;
	efi_service*svc;
	bool permissive=false;
	static const struct option lo[]={
		{"root",       required_argument, NULL, 'r'},
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
	dl_iterate_phdr(fill_self_data,NULL);
	xlog_set_verbose(false);
	xlog_set_print_call_stack(false);
	xlog_set_type(LOG_CALL,false);
	while((o=getopt_long(argc,argv,"r:o:u:f:vschbp",lo,NULL))>0)switch(o){
		case 'o':
			if(xlog_open(optarg)<0)
				fprintf(stderr,"open %s failed: %m\n",optarg);
		break;
		case 's':
			#ifndef SDL2_FOUND
			fprintf(stderr,"sdl2 not enabled\n");
			return 1;
			#endif
			if((svc=efi_service_lookup_service(
				&gEfiWrapperSDL2GraphicsOutputProtocolGuid
			)))svc->disabled=false;
		break;
		case 'r':
			if(!list_search_case_string(root,optarg))
				list_obj_add_new_strdup(NULL,&root,optarg);
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
	if(root)efi_service_config_unregister_by_name("UnixRoot");
	if(!(ctx=efi_new_run_context()))return -1;
	add_fs(ctx,root);
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
