//
// Created by bigfoot on 2023/5/12.
//

#include"efi/efi_context.h"
#include"efi/guids/hob_list.h"

static const struct efi_hob_data{
	uint8_t _start[0];
	union{
		efi_hob_handoff_info_table table;
		uint8_t _pad0[0x1000] cdecl_attr_unused;
	};
	union{
		struct{
			efi_hob_generic_header end;
			uint8_t _free_start[0];
		};
		uint8_t _pad1[0x1000] cdecl_attr_unused;
	};
	uint8_t _pad2[0x6000] cdecl_attr_unused;
	uint8_t _free_end[0];
	uint8_t _end[0];
}hob_data={
	.table={
		.header={
			.type=efi_hob_type_handoff,
			.length=sizeof(hob_data.table),
		},
		.version             = EFI_HOB_HANDOFF_TABLE_VERSION,
		.boot_mode           = boot_with_min_cfg,
		.mem_top.ptr         = (void*)&hob_data._start,
		.mem_bottom.ptr      = (void*)&hob_data._end,
		.free_mem_top.ptr    = (void*)&hob_data._free_start,
		.free_mem_bottom.ptr = (void*)&hob_data._free_end,
		.end_of_hob.ptr      = (void*)&hob_data.end,
	},
	.end={
		.type=efi_hob_list_end,
		.length=sizeof(hob_data.end),
	}
};

static efi_status svc_start(
	efi_run_context*ctx,
	efi_service_instance*ins
){
	struct efi_hob_data*hob;
	if(!ctx||!ins||!(hob=ins->cust_data))
		return efi_invalid_parameter;
	hob->table.mem_top.ptr=(void*)&hob->_start;
	hob->table.mem_bottom.ptr=(void*)&hob->_end;
	hob->table.free_mem_top.ptr=(void*)&hob->_free_start;
	hob->table.free_mem_bottom.ptr=(void*)&hob->_free_end;
	hob->table.end_of_hob.ptr=(void*)&hob->end;
	return ctx->bs->install_cfg_table(
		&gEfiHobListGuid,
		ins->cust_data
	);
}

static const efi_service efi_hob_list_service={
	.guid       = &gEfiHobListGuid,
	.name       = "HobList",
	.depends    = {
		&gEfiWrapperBootServiceTableGuid,
		&gEfiWrapperSystemTableGuid,
		NULL
	},
	.start      = svc_start,
	.cust_len   = sizeof(hob_data),
	.cust_init  = &hob_data,
	.cust_align = 0x1000,
};

DECLARE_EFI_SERVICE(efi_hob_list_service)
