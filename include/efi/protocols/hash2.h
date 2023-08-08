#ifndef EFI_WRAPPER_HASH2_H
#define EFI_WRAPPER_HASH2_H
#include"../efi_types.h"
#include"../efi_status.h"
#include"hash.h"
#define EFI_HASH2_SERVICE_BINDING_PROTOCOL_GUID {0xda836f8d,0x217f,0x4ca0,{0x99,0xc2,0x1c,0xa4,0xe1,0x60,0x77,0xea}}
#define EFI_HASH2_PROTOCOL_GUID                 {0x55b1d734,0xc5e1,0x49db,{0x96,0x47,0xb1,0x6a,0xfb,0x0e,0x30,0x5b}}
typedef struct efi_hash2_protocol efi_hash2_protocol;
typedef uint8_t efi_md5_hash2[16];
typedef uint8_t efi_sha1_hash2[20];
typedef uint8_t efi_sha224_hash2[28];
typedef uint8_t efi_sha256_hash2[32];
typedef uint8_t efi_sha384_hash2[48];
typedef uint8_t efi_sha512_hash2[64];
typedef union efi_hash2_output{
	efi_md5_hash2     md5;
	efi_sha1_hash2    sha1;
	efi_sha224_hash2  sha224;
	efi_sha256_hash2  sha256;
	efi_sha384_hash2  sha384;
	efi_sha512_hash2  sha512;
}efi_hash2_output;
typedef efi_status(efiapi*efi_hash2_get_hash_size)(
	const efi_hash2_protocol *this,
	const efi_guid*algo,
	uintn_t*size
);
typedef efi_status(efiapi*efi_hash2_hash)(
	const efi_hash2_protocol*this,
	const efi_guid*algo,
	const uint8_t*msg,
	uintn_t size,
        efi_hash2_output*hash
);
typedef efi_status(efiapi*efi_hash2_hash_init)(
	const efi_hash2_protocol*this,
	const efi_guid*algo
);
typedef efi_status(efiapi*efi_hash2_hash_update)(
	const efi_hash2_protocol*this,
	const uint8_t*msg,
	uintn_t size
);
typedef efi_status(efiapi*efi_hash2_hash_final)(
	const efi_hash2_protocol*this,
        efi_hash2_output*hash
);
struct efi_hash2_protocol{
	efi_hash2_get_hash_size get_hash_size;
	efi_hash2_hash          hash;
	efi_hash2_hash_init     hash_init;
	efi_hash2_hash_update   hash_update;
	efi_hash2_hash_final    hash_final;
};
extern const efi_guid gEfiHash2ServiceBindingProtocolGuid;
extern const efi_guid gEfiHash2ProtocolGuid;
#endif
