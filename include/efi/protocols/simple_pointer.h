//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_SIMPLE_POINTER_H
#define EFI_WRAPPER_SIMPLE_POINTER_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_SIMPLE_POINTER_PROTOCOL_GUID {0x31878c87,0x0b75,0x11d5,{0x9a,0x4f,0x00,0x90,0x27,0x3f,0xc1,0x4d}}
typedef struct efi_simple_pointer_protocol efi_simple_pointer_protocol;
typedef struct efi_simple_pointer_state{
	int32_t rel_move_x;
	int32_t rel_move_y;
	int32_t rel_move_z;
	bool left_btn;
	bool right_btn;
}efi_simple_pointer_state;
typedef struct efi_simple_pointer_mode{
	uint64_t res_x;
	uint64_t res_y;
	uint64_t res_z;
	bool left_btn;
	bool right_btn;
}efi_simple_pointer_mode;
typedef efi_status(efiapi*efi_simple_pointer_reset)(
	efi_simple_pointer_protocol*proto,
	bool ext_verify
);
typedef efi_status(efiapi*efi_simple_pointer_get_state)(
	efi_simple_pointer_protocol*proto,
	efi_simple_pointer_state*state
);
struct efi_simple_pointer_protocol{
	efi_simple_pointer_reset      reset;
	efi_simple_pointer_get_state  get_state;
	efi_event                     wait_for_input;
	efi_simple_pointer_mode       *mode;
};
extern const efi_guid gEfiSimplePointerProtocolGuid;
#endif //EFI_WRAPPER_SIMPLE_POINTER_H
