#ifndef EFI_WRAPPER_HASH_H
#define EFI_WRAPPER_HASH_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_HASH_SERVICE_BINDING_PROTOCOL_GUID {0x42881c98,0xa4f3,0x44b0,{0xa3,0x9d,0xdf,0xa1,0x86,0x67,0xd8,0xcd}}
#define EFI_HASH_PROTOCOL_GUID                 {0xc5184932,0xdba5,0x46db,{0xa5,0xba,0xcc,0x0b,0xda,0x9c,0x14,0x35}}
#define EFI_HASH_ALGORITHM_SHA1_GUID           {0x2ae9d80f,0x3fb2,0x4095,{0xb7,0xb1,0xe9,0x31,0x57,0xb9,0x46,0xb6}}
#define EFI_HASH_ALGORITHM_SHA224_GUID         {0x8df01a06,0x9bd5,0x4bf7,{0xb0,0x21,0xdb,0x4f,0xd9,0xcc,0xf4,0x5b}}
#define EFI_HASH_ALGORITHM_SHA256_GUID         {0x51aa59de,0xfdf2,0x4ea3,{0xbc,0x63,0x87,0x5f,0xb7,0x84,0x2e,0xe9}}
#define EFI_HASH_ALGORITHM_SHA384_GUID         {0xefa96432,0xde33,0x4dd2,{0xae,0xe6,0x32,0x8c,0x33,0xdf,0x77,0x7a}}
#define EFI_HASH_ALGORITHM_SHA512_GUID         {0xcaa4381e,0x750c,0x4770,{0xb8,0x70,0x7a,0x23,0xb4,0xe4,0x21,0x30}}
#define EFI_HASH_ALGORTIHM_MD5_GUID            {0x0af7c79c,0x65b5,0x4319,{0xb0,0xae,0x44,0xec,0x48,0x4e,0x4a,0xd7}}
#define EFI_HASH_ALGORITHM_SHA1_NOPAD_GUID     {0x24c5dc2f,0x53e2,0x40ca,{0x9e,0xd6,0xa5,0xd9,0xa4,0x9f,0x46,0x3b}}
#define EFI_HASH_ALGORITHM_SHA256_NOPAD_GUID   {0x8628752a,0x6cb7,0x4814,{0x96,0xfc,0x24,0xa8,0x15,0xac,0x22,0x26}}
typedef struct efi_hash_protocol efi_hash_protocol;
typedef uint8_t efi_md5_hash[16];
typedef uint8_t efi_sha1_hash[20];
typedef uint8_t efi_sha224_hash[28];
typedef uint8_t efi_sha256_hash[32];
typedef uint8_t efi_sha384_hash[48];
typedef uint8_t efi_sha512_hash[64];
typedef union efi_hash_output{
	efi_md5_hash    *md5;
	efi_sha1_hash   *sha1;
	efi_sha224_hash *sha224;
	efi_sha256_hash *sha256;
	efi_sha384_hash *sha384;
	efi_sha512_hash *sha512;
}efi_hash_output;
typedef efi_status(efiapi*efi_hash_get_hash_size)(
	const efi_hash_protocol*this,
	const efi_guid*algo,
	uintn_t*size
);
typedef efi_status(efiapi*efi_hash_hash)(
	const efi_hash_protocol*this,
	const efi_guid*algo,
	bool extend,
	const uint8_t*msg,
	uint64_t msg_size,
	efi_hash_output*hash
);
struct efi_hash_protocol{
	efi_hash_get_hash_size get_hash_size;
	efi_hash_hash hash;
};
extern const efi_guid gEfiHashServiceBindingProtocolGuid;
extern const efi_guid gEfiHashProtocolGuid;
extern const efi_guid gEfiHashAlgorithmSha1Guid;
extern const efi_guid gEfiHashAlgorithmSha224Guid;
extern const efi_guid gEfiHashAlgorithmSha256Guid;
extern const efi_guid gEfiHashAlgorithmSha384Guid;
extern const efi_guid gEfiHashAlgorithmSha512Guid;
extern const efi_guid gEfiHashAlgorithmMD5Guid;
extern const efi_guid gEfiHashAlgorithmSha1NoPadGuid;
extern const efi_guid gEfiHashAlgorithmSha256NoPadGuid;
#endif
