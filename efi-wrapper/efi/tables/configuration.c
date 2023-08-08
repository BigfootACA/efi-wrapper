#include"internal.h"

efiapi efi_status efi_install_configuration_table_impl(
	const efi_guid *guid,
	void     *table
){
	void*ptr;
	uintn_t id;
	char buff[64];
	size_t size,os;
	efi_system_table*st;
	efi_configuration_table*cfg;
	log_call(
		call_arg(guid,guid),
		call_arg(ptr,table),
		NULL
	);
	if(!guid)log_return(efi_invalid_parameter);
	if(!efi_current_ctx||!(st=efi_current_ctx->st))log_return(efi_not_ready);
	for(id=0;(cfg=&st->cfg_table[id])&&id<st->entries;id++)
		if(efi_guid_equals(guid,&cfg->guid))break;
	if(id>=st->entries){
		if(!table)log_return(efi_not_found);
		os=efi_current_ctx->cfg_table_size;
		size=(st->entries+1)*sizeof(efi_configuration_table);
		if(size>os){
			ptr=os<=0?
				mem_allocate(efi_current_ctx->mem[efi_bs_data].pool,size):
				mem_reallocate(st->cfg_table,size);
			if(!ptr)log_return(efi_out_of_resources);
			efi_current_ctx->cfg_table_size=size;
			st->entries++,st->cfg_table=ptr;
		}
		cfg=&st->cfg_table[id];
		xlog(
			LOG_INFO,"insert cfg table %zu guid %s value %p",
			id,efi_guid_to_readable_string(guid,buff,sizeof(buff)),
			table
		);
		memcpy(&cfg->guid,guid,sizeof(efi_guid));
		cfg->table=table;
	}else if(!table){
		cfg=&st->cfg_table[id];
		xlog(
			LOG_INFO,"remove cfg table %zu guid %s",
			id,efi_guid_to_readable_string(&cfg->guid,buff,sizeof(buff))
		);
		st->entries--;
		memcpy(
			&st->cfg_table[id],&st->cfg_table[id+1],
			(st->entries-id)*sizeof(efi_configuration_table)
		);
	}else{
		cfg=&st->cfg_table[id];
		xlog(
			LOG_INFO,"replace cfg table %zu guid %s value from %p to %p",
			id,efi_guid_to_readable_string(&cfg->guid,buff,sizeof(buff)),
			cfg->table,table
		);
		cfg->table=table;
	}
	efi_table_calc_sum(st,sizeof(efi_system_table));
	log_return(efi_success);
}
