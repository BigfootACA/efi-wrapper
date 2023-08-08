#ifndef EFI_WRAPPER_TLS_H
#define EFI_WRAPPER_TLS_H
#include"../efi_types.h"
#include"../efi_status.h"
#define EFI_TLS_SERVICE_BINDING_PROTOCOL_GUID {0x952cb795,0xff36,0x48cf,{0xa2,0x49,0x4d,0xf4,0x86,0xd6,0xab,0x8d}}
#define EFI_TLS_PROTOCOL_GUID                 {0x00ca959f,0x6cfa,0x4db1,{0x95,0xbc,0xe4,0x6c,0x47,0x51,0x43,0x90}}
typedef struct efi_tls_protocol efi_tls_protocol;
typedef enum efi_tls_session_data_type{
	efi_tls_type_version,
	efi_tls_type_connection_end,
	efi_tls_type_cipher_list,
	efi_tls_type_compression_method,
	efi_tls_type_ext_data,
	efi_tls_type_verify_method,
	efi_tls_type_session_id,
	efi_tls_type_session_state,
	efi_tls_type_client_random,
	efi_tls_type_server_random,
	efi_tls_type_key_material,
	efi_tls_type_verify_host,
	efi_tls_type_session_data_type_maximum
}efi_tls_session_data_type;
typedef struct efi_tls_version{
	uint8_t major;
	uint8_t minor;
}efi_tls_version;
typedef enum efi_tls_connection_end{
	efi_tls_client,
	efi_tls_server,
}efi_tls_connection_end;
typedef struct efi_tls_cipher{
	uint8_t data1;
	uint8_t data2;
}cdecl_attr_packed efi_tls_cipher;
typedef uint8_t efi_tls_compression;
typedef struct efi_tls_extension{
	uint16_t ext_type;
	uint16_t length;
	uint8_t  data[0];
}cdecl_attr_packed efi_tls_extension;
typedef uint32_t efi_tls_verify;
#define EFI_TLS_VERIFY_NONE                  0x0
#define EFI_TLS_VERIFY_PEER                  0x1
#define EFI_TLS_VERIFY_FAIL_IF_NO_PEER_CERT  0x2
#define EFI_TLS_VERIFY_CLIENT_ONCE           0x4
typedef uint32_t efi_tls_verify_host_flag;
#define EFI_TLS_VERIFY_FLAG_NONE                    0x00
#define EFI_TLS_VERIFY_FLAG_ALWAYS_CHECK_SUBJECT    0x01
#define EFI_TLS_VERIFY_FLAG_NO_WILDCARDS            0x02
#define EFI_TLS_VERIFY_FLAG_NO_PARTIAL_WILDCARDS    0x04
#define EFI_TLS_VERIFY_FLAG_MULTI_LABEL_WILDCARDS   0x08
#define EFI_TLS_VERIFY_FLAG_SINGLE_LABEL_SUBDOMAINS 0x10
#define EFI_TLS_VERIFY_FLAG_NEVER_CHECK_SUBJECT     0x20
typedef struct efi_tls_verify_host{
	efi_tls_verify_host_flag flags;
	char8 *hostname;
}cdecl_attr_packed efi_tls_verify_host;
typedef struct efi_tls_random{
	uint32_t gmt_unix_time;
	uint8_t  random_bytes[28];
}cdecl_attr_packed efi_tls_random;
typedef struct efi_tls_master_secret{
	uint8_t data[48];
}cdecl_attr_packed efi_tls_master_secret;
#define MAX_TLS_SESSION_ID_LENGTH 32
typedef struct efi_tls_session_id{
	uint16_t len;
	uint8_t data[MAX_TLS_SESSION_ID_LENGTH];
}cdecl_attr_packed efi_tls_session_id;
typedef enum efi_tls_session_state{
	efi_tls_session_not_started,
	efi_tls_session_handshaking,
	efi_tls_session_data_transferring,
	efi_tls_session_closing,
	efi_tls_session_error,
	efi_tls_session_state_max
}efi_tls_session_state;
typedef struct efi_tls_fragment_data{
	uint32_t len;
	void*buf;
}efi_tls_fragment_data;
typedef enum efi_tls_crypt_mode{
	efi_tls_encrypt,
	efi_tls_decrypt,
}efi_tls_crypt_mode;
typedef efi_status(efiapi*efi_tls_set_session_data)(
	efi_tls_protocol          *this,
	efi_tls_session_data_type type,
	void                      *data,
	uintn_t                   size
);
typedef efi_status(efiapi*efi_tls_get_session_data)(
	efi_tls_protocol          *this,
	efi_tls_session_data_type type,
	void                      *data,
	uintn_t                   *size
);
typedef efi_status(efiapi*efi_tls_build_response_packet)(
	efi_tls_protocol *this,
	uint8_t          *req_buf,
	uintn_t          req_size,
	uint8_t          *buf,
	uintn_t          *buf_size
);
typedef efi_status(efiapi*efi_tls_process_packet)(
	efi_tls_protocol      *this,
	efi_tls_fragment_data **frag_table,
	uint32_t              *frag_count,
	efi_tls_crypt_mode    crypt_mode
);
struct efi_tls_protocol{
	efi_tls_set_session_data      set_session_data;
	efi_tls_get_session_data      get_session_data;
	efi_tls_build_response_packet build_rsp_pkt;
	efi_tls_process_packet        process_pkt;
};
extern const efi_guid gEfiTlsServiceBindingProtocolGuid;
extern const efi_guid gEfiTlsProtocolGuid;
#endif
