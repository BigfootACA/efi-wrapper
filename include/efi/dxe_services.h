#ifndef EFI_WRAPPER_DXE_SVC_H
#define EFI_WRAPPER_DXE_SVC_H
#include"efi_types.h"
#include"efi_status.h"
#define EFI_DXE_SERVICES_TABLE_GUID {0x05AD34BA,0x6F02,0x4214,{0x95,0x2E,0x4D,0xA0,0x39,0x8E,0x2B,0xB9}}
typedef enum efi_gcd_mem_type{
	efi_gcd_mem_non_exist,
	efi_gcd_mem_reserved,
	efi_gcd_mem_sys_mem,
	efi_gcd_mem_mmio,
	efi_gcd_mem_persist,
	efi_gcd_mem_mr,
	efi_gcd_mem_max
}efi_gcd_mem_type;
typedef enum efi_gcd_io_type{
	efi_gcd_io_non_exist,
	efi_gcd_io_reserved,
	efi_gcd_io_io,
	efi_gcd_io_max
}efi_gcd_io_type;
typedef enum efi_gcd_alloc_type{
	efi_gcd_alloc_any_search_bottom_up,
	efi_gcd_alloc_max_addr_search_bottom_up,
	efi_gcd_alloc_addr,
	efi_gcd_alloc_any_search_top_down,
	efi_gcd_alloc_max_addr_search_top_down,
	efi_gcd_alloc_max
}efi_gcd_alloc_type;
typedef struct efi_gcd_mem_space_desc{
	efi_address      base;
	uint64_t         len;
	uint64_t         caps;
	uint64_t         attr;
	efi_gcd_mem_type type;
	efi_handle       image;
	efi_handle       device;
}efi_gcd_mem_space_desc;
typedef struct efi_gcd_io_space_desc{
	efi_address     base;
	uint64_t        len;
	efi_gcd_io_type type;
	efi_handle      image;
	efi_handle      device;
}efi_gcd_io_space_desc;
typedef efi_status(efiapi*efi_add_mem_space)(
	efi_gcd_mem_type type,
	efi_address      base,
	uint64_t         len,
	uint64_t         caps
);
typedef efi_status(efiapi*efi_alloc_mem_space)(
	efi_gcd_alloc_type alloc_type,
	efi_gcd_mem_type   mem_type,
	uintn_t            align,
	uint64_t           len,
	efi_address*       base,
	efi_handle         image,
	efi_handle         device
);
typedef efi_status(efiapi*efi_free_mem_space)(
	efi_address  base,
	uint64_t     len
);
typedef efi_status(efiapi*efi_remove_mem_space)(
	efi_address base,
	uint64_t    len
);
typedef efi_status(efiapi*efi_get_mem_space_desc)(
	efi_address             base,
	efi_gcd_mem_space_desc  *desc
);
typedef efi_status(efiapi*efi_set_mem_space_attrs)(
	efi_address base,
	uint64_t    len,
	uint64_t    attrs
);
typedef efi_status(efiapi*efi_set_mem_space_caps)(
	efi_address base,
	uint64_t    len,
	uint64_t    caps
);
typedef efi_status(efiapi*efi_get_mem_space_map)(
	uintn_t                *num,
	efi_gcd_mem_space_desc **map
);
typedef efi_status(efiapi*efi_add_io_space)(
	efi_gcd_io_type io_type,
	efi_address     base,
	uint64_t        len
);
typedef efi_status(efiapi*efi_alloc_io_space)(
	efi_gcd_alloc_type alloc_type,
	efi_gcd_io_type    io_type,
	uintn_t            align,
	uint64_t           len,
	efi_address        *base,
	efi_handle         image,
	efi_handle         device
);
typedef efi_status(efiapi*efi_free_io_space)(
	efi_address  base,
	uint64_t     len
);
typedef efi_status(efiapi*efi_remove_io_space)(
	efi_address  base,
	uint64_t     len
);
typedef efi_status(efiapi*efi_get_io_space_desc)(
	efi_address            base,
	efi_gcd_io_space_desc  *desc
);
typedef efi_status(efiapi*efi_get_io_space_map)(
	uintn_t                *cnt,
	efi_gcd_io_space_desc  **map
);
typedef efi_status(efiapi*efi_dispatch)(void);
typedef efi_status(efiapi*efi_schedule)(
	efi_handle     fv_hand,
	const efi_guid *file
);
typedef efi_status(efiapi*efi_trust)(
	efi_handle     fv_hand,
	const efi_guid *file
);
typedef efi_status(efiapi*efi_proc_fv)(
	const void *fv_hdr,
	uintn_t    size,
	efi_handle *fv_hand
);
#define EFI_DXE_SERVICES_SIGNATURE   0x565245535f455844ULL
#define EFI_DXE_SPEC_MAJOR_REVISION  1
#define EFI_DXE_SPEC_MINOR_REVISION  70
#define EFI_DXE_SERVICES_REVISION    ((EFI_DXE_SPEC_MAJOR_REVISION<<16) | (EFI_DXE_SPEC_MINOR_REVISION))
typedef struct efi_dxe_services{
	efi_table_header        header;
	efi_add_mem_space       add_mem_space;
	efi_alloc_mem_space     alloc_mem_space;
	efi_free_mem_space      free_mem_space;
	efi_remove_mem_space    remove_mem_space;
	efi_get_mem_space_desc  get_mem_space_desc;
	efi_set_mem_space_attrs set_mem_space_attrs;
	efi_get_mem_space_map   get_mem_space_map;
	efi_add_io_space        add_io_space;
	efi_alloc_io_space      alloc_io_space;
	efi_free_io_space       free_io_space;
	efi_remove_io_space     remove_io_space;
	efi_get_io_space_desc   get_io_space_desc;
	efi_get_io_space_map    get_io_space_map;
	efi_dispatch            dispatch;
	efi_schedule            schedule;
	efi_trust               trust;
	efi_proc_fv             proc_fv;
	efi_set_mem_space_caps  set_mem_space_caps;
}efi_dxe_services;
extern const efi_guid gEfiDxeServicesTableGuid;
#endif
