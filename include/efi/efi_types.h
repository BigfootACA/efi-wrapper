//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_EFI_TYPES_H
#define EFI_WRAPPER_EFI_TYPES_H
#include<assert.h>
#include<stdarg.h>
#include<stddef.h>
#include<stdint.h>
#include<stdbool.h>
#include<sys/types.h>
#include"defines.h"
#ifndef INT8_MIN
#define INT8_MIN   (-128)
#endif
#ifndef INT16_MIN
#define INT16_MIN  (-32767-1)
#endif
#ifndef INT32_MIN
#define INT32_MIN  (-2147483647-1)
#endif
#ifndef INT64_MIN
#define INT64_MIN  (-(9223372036854775807L)-1)
#endif
#ifndef INT8_MAX
#define INT8_MAX   (127)
#endif
#ifndef INT16_MAX
#define INT16_MAX  (32767)
#endif
#ifndef INT32_MAX
#define INT32_MAX  (2147483647)
#endif
#ifndef INT64_MAX
#define INT64_MAX  (9223372036854775807L)
#endif
#ifndef UINT8_MAX
#define UINT8_MAX  (255)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (65535)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif
#ifndef UINT64_MAX
#define UINT64_MAX (18446744073709551615UL)
#endif
#if WORD_SIZE == 4
typedef uint32_t uintn_t;
typedef int32_t intn_t;
#ifndef UINTN_MAX
#define UINTN_MAX UINT32_MAX
#endif
#ifndef INTN_MAX
#define INTN_MAX INT32_MAX
#endif
#elif WORD_SIZE == 8
typedef uint64_t uintn_t;
typedef int64_t intn_t;
#ifndef UINTN_MAX
#define UINTN_MAX UINT64_MAX
#endif
#ifndef INTN_MAX
#define INTN_MAX INT64_MAX
#endif
#else
#error word size unknown
#endif
#if defined(__x86_64__)||defined(__amd64__)
#define efiapi cdecl_attr_msabi
typedef __builtin_ms_va_list VA_LIST;
#define VA_START(va,param)      __builtin_ms_va_start(va,param)
#define VA_ARG(va,type)         ((sizeof(type)<sizeof(uintn_t))?(type)(__builtin_va_arg(va,uintn_t)):(type)(__builtin_va_arg(va,type)))
#define VA_END(va)              __builtin_ms_va_end(va)
#define VA_COPY(dest,start)     __builtin_ms_va_copy(dest, start)
#else
#define efiapi
typedef va_list VA_LIST;
#define VA_START(va,param)      va_start(va,param)
#define VA_ARG(va,type)         va_arg(va,type)
#define VA_END(va)              va_end(va)
#define VA_COPY(dest,start)     va_copy(dest,start)
#endif
#define SIGNATURE_16(A,B)((A)|(B<<8))
#define SIGNATURE_32(A,B,C,D)(SIGNATURE_16(A,B)|(SIGNATURE_16(C,D)<<16))
#define SIGNATURE_64(A,B,C,D,E,F,G,H)(SIGNATURE_32(A,B,C,D)|((uint64_t)(SIGNATURE_32(E,F,G,H))<<32))
#define EFI_HANDLE_SIGN SIGNATURE_64('E','F','I','_','H','A','N','D')
#define EFI_EVENT_SIGN SIGNATURE_64('E','F','I','E','V','E','N','T')
#define EVT_TIMER                               0x80000000
#define EVT_RUNTIME                             0x40000000
#define EVT_NOTIFY_WAIT                         0x00000100
#define EVT_NOTIFY_SIGNAL                       0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES           0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE       0x60000202
#define TPL_APPLICATION         0x04
#define TPL_CALLBACK            0x08
#define TPL_NOTIFY              0x10
#define TPL_HIGH_LEVEL          0x1F
#define EFI_MEMORY_UC               0x0000000000000001ULL
#define EFI_MEMORY_WC               0x0000000000000002ULL
#define EFI_MEMORY_WT               0x0000000000000004ULL
#define EFI_MEMORY_WB               0x0000000000000008ULL
#define EFI_MEMORY_UCE              0x0000000000000010ULL
#define EFI_MEMORY_WP               0x0000000000001000ULL
#define EFI_MEMORY_RP               0x0000000000002000ULL
#define EFI_MEMORY_XP               0x0000000000004000ULL
#define EFI_MEMORY_RO               0x0000000000020000ULL
#define EFI_MEMORY_NV               0x0000000000008000ULL
#define EFI_MEMORY_MORE_RELIABLE    0x0000000000010000ULL
#define EFI_MEMORY_SP               0x0000000000040000ULL
#define EFI_MEMORY_CPU_CRYPTO       0x0000000000080000ULL
#define EFI_MEMORY_RUNTIME          0x8000000000000000ULL
#define EFI_CACHE_ATTRIBUTE_MASK (EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB | EFI_MEMORY_UCE | EFI_MEMORY_WP)
#define EFI_MEMORY_ACCESS_MASK (EFI_MEMORY_RP | EFI_MEMORY_XP | EFI_MEMORY_RO)
#define EFI_MEMORY_ATTRIBUTE_MASK (EFI_MEMORY_ACCESS_MASK | EFI_MEMORY_SP | EFI_MEMORY_CPU_CRYPTO)
#define EFI_VARIABLE_NON_VOLATILE                            0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                      0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                          0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                   0x00000008
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS   0x00000020
#define EFI_VARIABLE_APPEND_WRITE                            0x00000040
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS              0x00000010
#define EFI_VARIABLE_ATTRIBUTE_NV_BS (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)
#define EFI_VARIABLE_ATTRIBUTE_BS_RT (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS)
#define EFI_VARIABLE_ATTRIBUTE_BS_RT_AT (VARIABLE_ATTRIBUTE_BS_RT | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS)
#define EFI_VARIABLE_ATTRIBUTE_NV_BS_RT (VARIABLE_ATTRIBUTE_BS_RT | EFI_VARIABLE_NON_VOLATILE)
#define EFI_VARIABLE_ATTRIBUTE_NV_BS_RT_HR (VARIABLE_ATTRIBUTE_NV_BS_RT | EFI_VARIABLE_HARDWARE_ERROR_RECORD)
#define EFI_VARIABLE_ATTRIBUTE_NV_BS_RT_AT (VARIABLE_ATTRIBUTE_NV_BS_RT | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS)
#define EFI_VARIABLE_ATTRIBUTE_AT EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS
#define EFI_VARIABLE_ATTRIBUTE_NV_BS_RT_HR_AT (VARIABLE_ATTRIBUTE_NV_BS_RT_HR | VARIABLE_ATTRIBUTE_AT)
#define EFI_VARIABLE_ATTRIBUTE_AT_AW (EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS | EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS)
#define EFI_VARIABLE_ATTRIBUTE_NV_BS_RT_AW (VARIABLE_ATTRIBUTE_NV_BS_RT | EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS)
#define EFI_VARIABLE_ATTRIBUTE_NV_BS_RT_HR_AT_AW (VARIABLE_ATTRIBUTE_NV_BS_RT_HR | VARIABLE_ATTRIBUTE_AT_AW)
#define EFI_VARIABLE_ATTRIBUTES_MASK (\
	EFI_VARIABLE_NON_VOLATILE|\
	EFI_VARIABLE_BOOTSERVICE_ACCESS|\
	EFI_VARIABLE_RUNTIME_ACCESS|\
	EFI_VARIABLE_HARDWARE_ERROR_RECORD|\
	EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS|\
	EFI_VARIABLE_APPEND_WRITE\
)
typedef int8_t char8;
typedef int16_t char16;
typedef uint16_t uchar16;
typedef uintn_t efi_tpl;
typedef uint64_t efi_lba;
typedef char*string8;
typedef unsigned short*string16;
typedef struct efi_event*efi_event;
typedef struct efi_handle*efi_handle;
static_assert(sizeof(wchar_t)==sizeof(char16),"wchar_t not 16-bits");
typedef struct guid{
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint8_t  data4[8];
}guid;
static_assert(sizeof(guid)==16,"efi guid size mismatch");
typedef struct efi_time{
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
	uint8_t  pad1;
	uint32_t nanosecond;
	int16_t  timezone;
	uint8_t  daylight;
	uint8_t  pad2;
}efi_time;
static_assert(sizeof(efi_time)==16,"efi time size mismatch");
typedef struct efi_table_header{
	uint64_t sign;
	uint32_t revision;
	uint32_t header_size;
	uint32_t crc32;
	uint32_t reserved;
}efi_table_header;
static_assert(sizeof(efi_table_header)==24,"efi table header size mismatch");
typedef enum efi_memory_type{
	efi_reserved_memory_type,
	efi_loader_code,
	efi_loader_data,
	efi_bs_code,
	efi_bs_data,
	efi_rs_code,
	efi_rs_data,
	efi_conventional_memory,
	efi_unusable_memory,
	efi_acpi_reclaim_memory,
	efi_acpi_memory_nvs,
	efi_mmio,
	efi_mmio_port_space,
	efi_pal_code,
	efi_persistent_memory,
	efi_max_memory_type,
}efi_memory_type;
typedef enum efi_allocate_type{
	allocate_any_pages,
	allocate_max_address,
	allocate_address,
	allocate_max
}efi_allocate_type;
typedef union efi_address{
	uintn_t   uintn;
	uintptr_t uintptr;
	intptr_t  intptr;
	void      *ptr;
}efi_address;
static_assert(sizeof(efi_address)==WORD_SIZE,"efi address size mismatch");
typedef struct efi_memory_descriptor{
	efi_memory_type type:32;
	efi_address     physical_start;
	efi_address     virtual_start;
	uint64_t        pages;
	uint64_t        attribute;
}efi_memory_descriptor;
#if WORD_SIZE == 8
static_assert(sizeof(efi_memory_descriptor)==40,"efi memory descriptor size mismatch");
#elif WORD_SIZE == 4
static_assert(sizeof(efi_memory_descriptor)==32,"efi memory descriptor size mismatch");
#else
#error unknown word size
#endif
typedef struct efi_mac_address{
	uint8_t addr[32];
}efi_mac_address;
typedef struct efi_ipv4_address{
	uint8_t addr[4];
}efi_ipv4_address;
typedef struct efi_ipv6_address{
	uint8_t addr[16];
}efi_ipv6_address;
typedef union efi_ip_address{
	uint32_t            addr[4];
	efi_ipv4_address  v4;
	efi_ipv6_address  v6;
}efi_ip_address;
typedef guid efi_guid;
typedef struct efi_guid_name{
	const char*name;
	const efi_guid*guid;
}efi_guid_name;
typedef struct efi_variable_item{
	efi_guid guid;
	char16*name;
	uint64_t attrs;
	void*data;
	size_t len;
}efi_variable_item;
extern const efi_guid_name efi_guid_table[];
#endif //EFI_WRAPPER_EFI_TYPES_H
