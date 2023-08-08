#include<errno.h>
#include<stdio.h>
#include<string.h>
#include"efi/efi_spec.h"

int efi_status_to_errno(efi_status st){
	switch(st){
		case efi_success:
			return 0;
		case efi_security_violation:
			return EPERM;
		case efi_write_protected:
			return EROFS;
		case efi_access_denied:
			return EACCES;
		case efi_no_media:
			return ENOMEDIUM;
		case efi_no_response:
			return ENODATA;
		case efi_not_found:
			return ENOENT;
		case efi_not_ready:
			return EAGAIN;
		case efi_no_mapping:
			return EFAULT;
		case efi_not_started:
			return ESRCH;
		case efi_media_changed:
			return EBUSY;
		case efi_bad_buffer_size:
		case efi_buffer_too_small:
		case efi_invalid_language:
		case efi_invalid_parameter:
			return EINVAL;
		case efi_out_of_resources:
			return ENOMEM;
		case efi_compromised_data:
		case efi_volume_corrupted:
			return EUCLEAN;
		case efi_load_error:
			return EBADF;
		case efi_icmp_error:
		case efi_tftp_error:
		case efi_http_error:
		case efi_protocol_error:
			return EPROTO;
		case efi_crc_error:
		case efi_timeout:
			return ETIMEDOUT;
		case efi_volume_full:
			return ENOSPC;
		case efi_end_of_file:
		case efi_end_of_media:
			return ENOSTR;
		case efi_already_started:
			return EALREADY;
		case efi_aborted:
			return EINTR;
		case efi_unsupported:
			return ENOSYS;
		case efi_device_error:
			return EIO;
		case efi_incompatible_version:
			return ENOTSUP;
		default:
			return 255;
	}
}

efi_status efi_errno_to_status(int err){
	switch(err){
		case 0:return efi_success;
		case EPERM:
		case EACCES:
		case ENOTDIR:
		case EISDIR:
		case ECONNREFUSED:
		case EKEYREVOKED:
		case EKEYREJECTED:return efi_access_denied;
		case ENOENT:
		case ESRCH:
		case ENXIO:
		case ECHILD:
		case EIDRM:
		case ENOCSI:
		case ENOANO:
		case ENOPKG:
		case ELIBACC:return efi_not_found;
		case EIO:
		case EXDEV:
		case ENODEV:
		case ENOTTY:
		case EMLINK:
		case EOWNERDEAD:
		case ENOTRECOVERABLE:
		case ERFKILL:
		case EDEADLK:
		case ENOLCK:
		case ENOTEMPTY:
		case ELOOP:
		case EL2NSYNC:
		case EL3HLT:
		case EL3RST:
		case EUNATCH:
		case EL2HLT:
		case EBADE:
		case ENOSTR:
		case ENOLINK:
		case ESRMNT:return efi_device_error;
		case E2BIG:
		case ENOMEM:
		case ENFILE:
		case EMFILE:
		case ENAMETOOLONG:
		case EXFULL:
		case ENOSR:
		case EOVERFLOW:
		case ELIBMAX:
		case EUSERS:
		case EMSGSIZE:
		case ENOBUFS:
		case ETOOMANYREFS:return efi_out_of_resources;
		case EBADF:
		case EFAULT:
		case ENOTBLK:
		case EINVAL:
		case EDOM:
		case ERANGE:
		case ECHRNG:
		case ELNRNG:
		case EBADR:
		case EBADRQC:
		case EBADSLT:
		case EBADFD:
		case EDESTADDRREQ:
		case ENOTNAM:return efi_invalid_parameter;
		case ENOEXEC:
		case EFBIG:
		case EBFONT:
		case ELIBSCN:
		case ELIBEXEC:
		case EILSEQ:return efi_load_error;
		case EAGAIN:
		case EBUSY:
		case ETXTBSY:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case EINPROGRESS:
		case ENOKEY:return efi_not_ready;
		case EPIPE:
		case ENOMSG:
		case EADV:
		case ECOMM:
		case EPROTO:
		case EMULTIHOP:
		case EDOTDOT:
		case EBADMSG:
		case ENOTUNIQ:
		case EREMCHG:
		case ESTRPIPE:
		case EPROTOTYPE:
		case EREMOTEIO:return efi_protocol_error;
		case EEXIST:
		case EISCONN:
		case EALREADY:return efi_already_started;
		case EDQUOT:
		case ENOSPC:return efi_volume_full;
		case ESPIPE:
		case ENODATA:return efi_end_of_file;
		case EHWPOISON:
		case ELIBBAD:
		case ESTALE:return efi_compromised_data;
		case EROFS:return efi_write_protected;
		case ENOTSUP:
		case ENOSYS:
		case ENOTSOCK:
		case ENOPROTOOPT:
		case EPROTONOSUPPORT:
		case ESOCKTNOSUPPORT:
		case EPFNOSUPPORT:
		case EAFNOSUPPORT:
		case ENAVAIL:return efi_unsupported;
		case ETIME:
		case ETIMEDOUT:return efi_timeout;
		case ENONET:
		case EREMOTE:return efi_no_mapping;
		case ENETDOWN:
		case ENETRESET:
		case EHOSTDOWN:
		case EMEDIUMTYPE:return efi_no_media;
		case ENETUNREACH:
		case EHOSTUNREACH:return efi_no_response;
		case ECANCELED:
		case ECONNABORTED:
		case ECONNRESET:return efi_aborted;
		case ENOTCONN:
		case ESHUTDOWN:return efi_not_started;
		case EUCLEAN:return efi_volume_corrupted;
		case EISNAM:
		case ENOMEDIUM:
		case EKEYEXPIRED:return efi_load_error;
		default:return (efi_status)-1;
	}
}

const char*efi_status_to_string(efi_status st){
	switch(st){
		case efi_success:               return "Success";
		case efi_warn_unknown_glyph:    return "Unknown Glyph";
		case efi_warn_delete_failure:   return "Delete Failure";
		case efi_warn_write_failure:    return "Write Failure";
		case efi_warn_buffer_too_small: return "Buffer Too Small";
		case efi_warn_stale_data:       return "Stale Data";
		case efi_load_error:            return "Load Error";
		case efi_invalid_parameter:     return "Invalid Parameter";
		case efi_unsupported:           return "Unsupported";
		case efi_bad_buffer_size:       return "Bad Buffer Size";
		case efi_buffer_too_small:      return "Buffer Too Small";
		case efi_not_ready:             return "Not Ready";
		case efi_device_error:          return "Device Error";
		case efi_write_protected:       return "Write Protected";
		case efi_out_of_resources:      return "Out of Resources";
		case efi_volume_corrupted:      return "Volume Corrupt";
		case efi_volume_full:           return "Volume Full";
		case efi_no_media:              return "No Media";
		case efi_media_changed:         return "Media changed";
		case efi_not_found:             return "Not Found";
		case efi_access_denied:         return "Access Denied";
		case efi_no_response:           return "No Response";
		case efi_no_mapping:            return "No mapping";
		case efi_timeout:               return "Time out";
		case efi_not_started:           return "Not started";
		case efi_already_started:       return "Already started";
		case efi_aborted:               return "Aborted";
		case efi_icmp_error:            return "ICMP Error";
		case efi_tftp_error:            return "TFTP Error";
		case efi_protocol_error:        return "Protocol Error";
		case efi_incompatible_version:  return "Incompatible Version";
		case efi_security_violation:    return "Security Violation";
		case efi_crc_error:             return "CRC Error";
		case efi_end_of_media:          return "End of Media";
		case efi_end_of_file:           return "End of File";
		case efi_invalid_language:      return "Invalid Language";
		case efi_compromised_data:      return "Compromised Data";
		default:                        return "Unknown";
	}
}

const char*efi_status_to_short_string(efi_status st){
	switch(st){
		case efi_success:               return "success";
		case efi_warn_unknown_glyph:    return "warn-unknown-glyph";
		case efi_warn_delete_failure:   return "warn-delete-failure";
		case efi_warn_write_failure:    return "warn-write-failure";
		case efi_warn_buffer_too_small: return "warn-buffer-too-small";
		case efi_warn_stale_data:       return "warn-stale-data";
		case efi_load_error:            return "load-error";
		case efi_invalid_parameter:     return "invalid-parameter";
		case efi_unsupported:           return "unsupported";
		case efi_bad_buffer_size:       return "bad-buffer-size";
		case efi_buffer_too_small:      return "buffer-too-small";
		case efi_not_ready:             return "not-ready";
		case efi_device_error:          return "device-error";
		case efi_write_protected:       return "write-protected";
		case efi_out_of_resources:      return "out-of-resources";
		case efi_volume_corrupted:      return "volume-corrupted";
		case efi_volume_full:           return "volume-full";
		case efi_no_media:              return "no-media";
		case efi_media_changed:         return "media-changed";
		case efi_not_found:             return "not-found";
		case efi_access_denied:         return "access-denied";
		case efi_no_response:           return "no-response";
		case efi_no_mapping:            return "no-mapping";
		case efi_timeout:               return "timeout";
		case efi_not_started:           return "not-started";
		case efi_already_started:       return "already-started";
		case efi_aborted:               return "aborted";
		case efi_icmp_error:            return "icmp-error";
		case efi_tftp_error:            return "tftp-error";
		case efi_protocol_error:        return "protocol-error";
		case efi_incompatible_version:  return "incompatible-version";
		case efi_security_violation:    return "security-violation";
		case efi_crc_error:             return "crc-error";
		case efi_end_of_media:          return "end-of-media";
		case efi_end_of_file:           return "end-of-file";
		case efi_invalid_language:      return "invalid-language";
		case efi_compromised_data:      return "compromised-data";
		default:                        return "unknown";
	}
}

const char*efi_memory_type_to_string(efi_memory_type type){
	switch(type){
		case efi_reserved_memory_type:  return "Reserved";
		case efi_loader_code:           return "Loader Code";
		case efi_loader_data:           return "Loader Data";
		case efi_bs_code:               return "Boot Services Code";
		case efi_bs_data:               return "Boot Services Data";
		case efi_rs_code:               return "Runtime Services Code";
		case efi_rs_data:               return "Runtime Services Data";
		case efi_conventional_memory:   return "Conventional";
		case efi_unusable_memory:       return "Unusable";
		case efi_acpi_reclaim_memory:   return "ACPI Reclaim";
		case efi_acpi_memory_nvs:       return "ACPI Memory NVS";
		case efi_mmio:                  return "MMIO";
		case efi_mmio_port_space:       return "MMIO Port Space";
		case efi_pal_code:              return "Pal Code";
		case efi_persistent_memory:     return "Persistent";
		case efi_max_memory_type:       return "Max";
		default:                        return "Unknown";
	}
}

const char*efi_guid_to_string(const efi_guid*guid,char*buff,size_t len){
	if(!guid||!buff||len<=0)return NULL;
	memset(buff,0,len);
	snprintf(buff,len-1,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid->data1,guid->data2,guid->data3,
		guid->data4[0],guid->data4[1],guid->data4[2],guid->data4[3],
		guid->data4[4],guid->data4[5],guid->data4[6],guid->data4[7]
	);
	return buff;
}

const char*efi_guid_to_readable_string(const efi_guid*guid,char*buff,size_t len){
	if(!guid||!buff||len<=0)return NULL;
	memset(buff,0,len);
	for(size_t i=0;efi_guid_table[i].guid;i++){
		if(!efi_guid_equals(guid,efi_guid_table[i].guid))continue;
		if(!efi_guid_table[i].name||!efi_guid_table[i].name[0])continue;
		snprintf(buff,len-1,"%s",efi_guid_table[i].name);
		return buff;
	}
	return efi_guid_to_string(guid,buff,len);
}
