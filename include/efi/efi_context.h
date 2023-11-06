#ifndef EFI_WRAPPER_EFI_CONTEXT_H
#define EFI_WRAPPER_EFI_CONTEXT_H
#include"mem.h"
#include"list.h"
#include"logger.h"
#include"tlsf.h"
#include"efi/efi_spec.h"
#include"efi/protocols/simple_filesystem.h"

#define EFI_WRAPPER_SYSTEM_TABLE_GUID          {0xeca4a901,0x1ef3,0x4080,{0xb1,0x24,0x98,0x57,0xc6,0xe5,0x4c,0x80}}
#define EFI_WRAPPER_BOOT_SERVICE_TABLE_GUID    {0x967269ef,0x6da4,0x083b,{0x27,0x0f,0xa3,0x81,0x38,0xe0,0x95,0x7f}}
#define EFI_WRAPPER_RUNTIME_SERVICE_TABLE_GUID {0x2ed316fe,0x2843,0x107d,{0xaf,0x73,0xd6,0xb9,0x08,0x77,0x6e,0x66}}
#define EFI_CONV_POOL_SIZE    SIZE_128MB
#define EFI_BS_CODE_POOL_SIZE SIZE_128MB
#define EFI_BS_DATA_POOL_SIZE SIZE_128MB
#define EFI_LD_CODE_POOL_SIZE SIZE_128MB
#define EFI_LD_DATA_POOL_SIZE SIZE_128MB
#define DECLARE_EFI_SERVICE_CONFIG(_cfg)\
	cdecl_attr(constructor,used)\
	static void efi_service_register_##_cfg(){\
		efi_service_config_register(&_cfg);\
	}
#define DECLARE_EFI_SERVICE(_name)\
	static const efi_service_cfg _name##_cfg={\
		.name    = "default",\
		.service = &_name,\
	};\
	DECLARE_EFI_SERVICE_CONFIG(_name##_cfg)
extern const efi_guid gEfiWrapperBootServiceTableGuid;
extern const efi_guid gEfiWrapperRuntimeServiceTableGuid;
extern const efi_guid gEfiWrapperSystemTableGuid;
typedef enum efi_service_status efi_service_status;
typedef struct efi_service efi_service;
typedef struct efi_service_cfg efi_service_cfg;
typedef struct efi_service_instance efi_service_instance;
typedef struct efi_run_context efi_run_context;
typedef struct efi_context_mem efi_context_mem;
typedef struct efi_interface_entry efi_interface_entry;
typedef struct efi_register_notify efi_register_notify;
typedef efi_status(*efi_service_run)(efi_run_context*ctx,efi_service_instance*ins);
struct efi_context_mem{
	efi_memory_descriptor map;
	mem_pool*pool;
	tlsf_t tlsf_ctl;
	pool_t tlsf_pool;
	void*pointer;
	size_t size;
};
struct efi_run_context{
	list*handles;
	void*fake_reg;
	efi_context_mem mem[efi_max_memory_type];
	efi_system_table*st;
	efi_boot_services*bs;
	efi_dxe_services*ds;
	efi_runtime_services*rt;
	size_t cfg_table_size;
	efi_tpl tpl;
	bool timer;
	list*svc_ins;
	list*event_queue;
	list*events;
	list*notifies;
	list*vars;
};
enum efi_service_status{
	service_unknown,
	service_starting,
	service_started,
	service_stopping,
	service_stopped,
	service_conditioning,
	service_skip,
	service_failed,
};
struct efi_service_instance{
	efi_service_status status;
	const efi_service_cfg*cfg;
	const efi_service*service;
	efi_handle handle;
	void*cust_data;
};
struct efi_service{
	const efi_guid*guid;
	char name[64];
	const efi_guid*provides[16];
	const efi_guid*depends[16];
	const efi_guid*after[16];
	efi_service_run condition;
	efi_service_run start;
	efi_service_run stop;
	bool disabled;
	bool restart;
	void*data;
	size_t cust_len;
	const void*cust_init;
	size_t cust_align;
};
struct efi_service_cfg{
	const char name[48];
	const efi_service*service;
	value_collection config;
};
struct efi_interface_entry{
	efi_guid guid;
	list*open_list;
	void*interface;
};
struct efi_register_notify{
	efi_event event;
	efi_guid protocol;
	list*handles;
};
struct efi_event{
	uint64_t sig;
        uint32_t type;
        uintn_t tpl;
        efi_event_notify func;
        void*context;
        const efi_guid*group;
        uint64_t next;
        uint64_t time;
        efi_timer_delay delay;
        bool signaled;
};
struct efi_handle{
	uint64_t sig;
	uint64_t type;
	void*userdata;
	list*interface;
};
extern efi_run_context*efi_current_ctx;
extern void efi_destroy_context(efi_run_context*ctx);
extern efi_run_context*efi_new_run_context();
extern void efi_table_calc_sum(void*table,size_t len);
extern bool efi_valid_handle(efi_handle hand);
extern bool efi_valid_event(efi_event event);
extern efi_handle efi_new_handle(efi_run_context*ctx);
extern bool efi_service_is_svc_started(efi_run_context*ctx,const efi_guid*guid);
extern efi_service*efi_service_lookup_service(const efi_guid*guid);
extern efi_service_instance*efi_service_lookup_instance(efi_run_context*ctx,const efi_guid*guid);
extern void*efi_service_get_instance_cust_data(efi_run_context*ctx,const efi_guid*guid);
extern void efi_service_config_register(const efi_service_cfg*cfg);
extern efi_service_cfg*efi_service_config_lookup(const char*name);
extern void efi_service_config_unregister(const efi_service_cfg*cfg);
extern void efi_service_config_unregister_by_name(const char*name);
extern void efi_service_install_all(efi_run_context*ctx);
extern void efi_service_auto_start(efi_run_context*ctx);
extern void efi_service_auto_stop(efi_run_context*ctx);

#endif //EFI_WRAPPER_EFI_CONTEXT_H
