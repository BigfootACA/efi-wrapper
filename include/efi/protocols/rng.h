#ifndef EFI_WRAPPER_RNG_H
#define EFI_WRAPPER_RNG_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_RNG_PROTOCOL_GUID                    {0x3152bca5,0xeade,0x433d,{0x86,0x2e,0xc0,0x1c,0xdc,0x29,0x1f,0x44}}
#define EFI_RNG_ALGORITHM_SP800_90_HASH_256_GUID {0xa7af67cb,0x603b,0x4d42,{0xba,0x21,0x70,0xbf,0xb6,0x29,0x3f,0x96}}
#define EFI_RNG_ALGORITHM_SP800_90_HMAC_256_GUID {0xc5149b43,0xae85,0x4f53,{0x99,0x82,0xb9,0x43,0x35,0xd3,0xa9,0xe7}}
#define EFI_RNG_ALGORITHM_SP800_90_CTR_256_GUID  {0x44f0de6e,0x4d8c,0x4045,{0xa8,0xc7,0x4d,0xd1,0x68,0x85,0x6b,0x9e}}
#define EFI_RNG_ALGORITHM_X9_31_3DES_GUID        {0x63c4785a,0xca34,0x4012,{0xa3,0xc8,0x0b,0x6a,0x32,0x4f,0x55,0x46}}
#define EFI_RNG_ALGORITHM_X9_31_AES_GUID         {0xacd03321,0x777e,0x4d3d,{0xb1,0xc8,0x20,0xcf,0xd8,0x88,0x20,0xc9}}
#define EFI_RNG_ALGORITHM_RAW                    {0xe43176d7,0xb6e8,0x4827,{0xb7,0x84,0x7f,0xfd,0xc4,0xb6,0x85,0x61}}
typedef struct efi_rng_protocol efi_rng_protocol;
typedef efi_guid efi_rng_algorithm;
typedef efi_status(efiapi*efi_rng_get_info)(
	efi_rng_protocol  *this,
	uintn_t           *size,
	efi_rng_algorithm *list
);
typedef efi_status(efiapi*efi_rng_get_rng)(
	efi_rng_protocol  *this,
	efi_rng_algorithm *algo,
	uintn_t           len,
	uint8_t           *value
);
struct efi_rng_protocol{
	efi_rng_get_info get_info;
	efi_rng_get_rng  get_rng;
};
extern const efi_guid gEfiRngProtocolGuid;
extern const efi_guid gEfiRngAlgorithmSp80090Hash256Guid;
extern const efi_guid gEfiRngAlgorithmSp80090Hmac256Guid;
extern const efi_guid gEfiRngAlgorithmSp80090Ctr256Guid;
extern const efi_guid gEfiRngAlgorithmX9313DesGuid;
extern const efi_guid gEfiRngAlgorithmX931AesGuid;
extern const efi_guid gEfiRngAlgorithmRaw;
#endif
