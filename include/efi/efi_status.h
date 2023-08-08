//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_STATUS_H
#define EFI_WRAPPER_STATUS_H
#include"efi_types.h"
#include<assert.h>
#define return_error(code) (code!=return_success)
#define efi_error(code) (code!=efi_success)
#define MAX_BIT ((uintn_t)((1ULL<<(__SIZEOF_POINTER__*8-1))))
typedef enum return_status{
	return_success               = 0,
	return_load_error            = MAX_BIT|1,
	return_invalid_parameter     = MAX_BIT|2,
	return_unsupported           = MAX_BIT|3,
	return_bad_buffer_size       = MAX_BIT|4,
	return_buffer_too_small      = MAX_BIT|5,
	return_not_ready             = MAX_BIT|6,
	return_device_error          = MAX_BIT|7,
	return_write_protected       = MAX_BIT|8,
	return_out_of_resources      = MAX_BIT|9,
	return_volume_corrupted      = MAX_BIT|10,
	return_volume_full           = MAX_BIT|11,
	return_no_media              = MAX_BIT|12,
	return_media_changed         = MAX_BIT|13,
	return_not_found             = MAX_BIT|14,
	return_access_denied         = MAX_BIT|15,
	return_no_response           = MAX_BIT|16,
	return_no_mapping            = MAX_BIT|17,
	return_timeout               = MAX_BIT|18,
	return_not_started           = MAX_BIT|19,
	return_already_started       = MAX_BIT|20,
	return_aborted               = MAX_BIT|21,
	return_icmp_error            = MAX_BIT|22,
	return_tftp_error            = MAX_BIT|23,
	return_protocol_error        = MAX_BIT|24,
	return_incompatible_version  = MAX_BIT|25,
	return_security_violation    = MAX_BIT|26,
	return_crc_error             = MAX_BIT|27,
	return_end_of_media          = MAX_BIT|28,
	return_end_of_file           = MAX_BIT|31,
	return_invalid_language      = MAX_BIT|32,
	return_compromised_data      = MAX_BIT|33,
	return_http_error            = MAX_BIT|35,
	return_warn_unknown_glyph    = 1,
	return_warn_delete_failure   = 2,
	return_warn_write_failure    = 3,
	return_warn_buffer_too_small = 4,
	return_warn_stale_data       = 5,
	return_warn_file_system      = 6,
}return_status;
typedef enum efi_status{
	efi_success               = return_success,
	efi_load_error            = return_load_error,
	efi_invalid_parameter     = return_invalid_parameter,
	efi_unsupported           = return_unsupported,
	efi_bad_buffer_size       = return_bad_buffer_size,
	efi_buffer_too_small      = return_buffer_too_small,
	efi_not_ready             = return_not_ready,
	efi_device_error          = return_device_error,
	efi_write_protected       = return_write_protected,
	efi_out_of_resources      = return_out_of_resources,
	efi_volume_corrupted      = return_volume_corrupted,
	efi_volume_full           = return_volume_full,
	efi_no_media              = return_no_media,
	efi_media_changed         = return_media_changed,
	efi_not_found             = return_not_found,
	efi_access_denied         = return_access_denied,
	efi_no_response           = return_no_response,
	efi_no_mapping            = return_no_mapping,
	efi_timeout               = return_timeout,
	efi_not_started           = return_not_started,
	efi_already_started       = return_already_started,
	efi_aborted               = return_aborted,
	efi_icmp_error            = return_icmp_error,
	efi_tftp_error            = return_tftp_error,
	efi_protocol_error        = return_protocol_error,
	efi_incompatible_version  = return_incompatible_version,
	efi_security_violation    = return_security_violation,
	efi_crc_error             = return_crc_error,
	efi_end_of_media          = return_end_of_media,
	efi_end_of_file           = return_end_of_file,
	efi_invalid_language      = return_invalid_language,
	efi_compromised_data      = return_compromised_data,
	efi_http_error            = return_http_error,
	efi_warn_unknown_glyph    = return_warn_unknown_glyph,
	efi_warn_delete_failure   = return_warn_delete_failure,
	efi_warn_write_failure    = return_warn_write_failure,
	efi_warn_buffer_too_small = return_warn_buffer_too_small,
	efi_warn_stale_data       = return_warn_stale_data,
	efi_warn_file_system      = return_warn_file_system,
}efi_status;

static_assert(sizeof(efi_status)==sizeof(uintn_t),"efi_status size mismatch");
static_assert(sizeof(return_status)==sizeof(uintn_t),"return_status size mismatch");

#endif //EFI_WRAPPER_STATUS_H
