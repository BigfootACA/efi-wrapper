//
// Created by bigfoot on 2023/5/6.
//

#ifndef EFI_WRAPPER_ABSOLUTE_POINTER_H
#define EFI_WRAPPER_ABSOLUTE_POINTER_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_ABSOLUTE_POINTER_PROTOCOL_GUID {0x8d59d32b,0xc655,0x4ae9,{0x9b,0x15,0xf2,0x59,0x04,0x99,0x2a,0x43}}
typedef struct efi_absolute_pointer_protocol efi_absolute_pointer_protocol;
typedef struct efi_absolute_pointer_mode{
	uint64_t abs_min_x;
	uint64_t abs_min_y;
	uint64_t abs_min_z;
	uint64_t abs_max_x;
	uint64_t abs_max_y;
	uint64_t abs_max_z;
	uint32_t attrs;
}cdecl_attr_packed efi_absolute_pointer_mode;
static_assert(sizeof(efi_absolute_pointer_mode)==52,"efi_absolute_pointer_mode size mismatch");
#define EFI_ABSP_SupportsAltActive    0x00000001
#define EFI_ABSP_SupportsPressureAsZ  0x00000002
typedef efi_status(efiapi*efi_absolute_pointer_reset)(
	efi_absolute_pointer_protocol*proto,
	bool ext_verify
);
#define EFI_ABSP_TouchActive  0x00000001
#define EFI_ABS_AltActive     0x00000002
typedef struct efi_absolute_pointer_state{
	uint64_t cur_x;
	uint64_t cur_y;
	uint64_t cur_z;
	uint32_t active_buttons;
}cdecl_attr_packed efi_absolute_pointer_state;
static_assert(sizeof(efi_absolute_pointer_state)==28,"efi_absolute_pointer_state size mismatch");
typedef efi_status(efiapi*efi_absolute_pointer_get_state)(
	efi_absolute_pointer_protocol*proto,
	efi_absolute_pointer_state*state
);
struct efi_absolute_pointer_protocol{
	efi_absolute_pointer_reset      reset;
	efi_absolute_pointer_get_state  get_state;
	efi_event                       wait_for_input;
	efi_absolute_pointer_mode       *mode;
};
#if WORD_SIZE == 8
static_assert(sizeof(efi_absolute_pointer_protocol)==32,"efi_absolute_pointer_protocol size mismatch");
#elif WORD_SIZE == 4
static_assert(sizeof(efi_absolute_pointer_protocol)==16,"efi_absolute_pointer_protocol size mismatch");
#else
#error unknown word size
#endif
extern const efi_guid gEfiAbsolutePointerProtocolGuid;
#endif //EFI_WRAPPER_ABSOLUTE_POINTER_H
