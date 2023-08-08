#ifndef EFI_WRAPPER_EFI_STRING_H
#define EFI_WRAPPER_EFI_STRING_H
#include"efi_spec.h"
extern int efi_status_to_errno(efi_status st);
extern efi_status efi_errno_to_status(int err);
extern const char*efi_status_to_string(efi_status st);
extern const char*efi_status_to_short_string(efi_status st);
extern const char*efi_memory_type_to_string(efi_memory_type type);
extern const char*efi_guid_to_string(const efi_guid*guid,char*buff,size_t len);
extern const char*efi_guid_to_readable_string(const efi_guid*guid,char*buff,size_t len);
extern ssize_t ustrcmp(const char16*str1,const char16*str2);
extern ssize_t ustrncmp(const char16*str1,const char16*str2,size_t len);
extern ssize_t ustrcasecmp(const char16*str1,const char16*str2);
extern ssize_t ustrncasecmp(const char16*str1,const char16*str2,size_t len);
extern char16*ustrstr(const char16*str,const char16*key);
extern char16*ustrcasestr(const char16*str,const char16*key);
extern char16*ustrchrnul(const char16*str,char16 c);
extern char16*ustrncat(char16*restrict dest,const char16*restrict src,size_t n);
extern size_t ustrlcpy(char16*dest,const char16*src,size_t n);
extern size_t ustrlcat(char16*dest,const char16*src,size_t n);
extern size_t ustr2xstrn(char*dest,size_t dsize,const char16*src,size_t ssize);
extern size_t ustr2astrn(char*dest,size_t dsize,const char16*src,size_t ssize);
extern size_t astr2ustrn(char16*dest,size_t dsize,const char*src,size_t ssize);
extern const char*ustr2astrnf(const char16*src,size_t ssize);
extern const char*ustr2xstrnf(const char16*src,size_t ssize);
extern const char16*astr2ustrnf(const char*src,size_t ssize);
extern char*ustr2astrna(const char16*src,size_t ssize);
extern char*ustr2xstrna(const char16*src,size_t ssize);
extern char16*astr2ustrna(const char*src,size_t ssize);

header_func size_t ustrlen(const char16*str){
	size_t len;
	if(!str)return 0;
	for(len=0;*str;str++,len++);
	return len;
}

header_func size_t ustrnlen(const char16*str,size_t n){
	size_t len;
	if(!str)return 0;
	for(len=0;*str&&len<n;str++,len++);
	return len;
}

header_func char16*ustrchr(const char16*str,char16 c){
	char16*r=ustrchrnul(str,c);
	return *(uchar16*)r==(uchar16)c?r:NULL;
}

header_func char16*ustrcpy(char16*restrict dest,const char16*restrict src){
	for(;(*dest=*src);src++,dest++);
	return dest;
}

header_func char16*ustrncpy(char16*restrict dest,const char16*restrict src,size_t n){
	for(;n>0&&(*dest=*src);n--,src++,dest++);
	return dest;
}

header_func size_t ustrsize(const char16*str){return str?((ustrlen(str)+1)*sizeof(char16)):0;}
header_func char16*ustrcat(char16*restrict dest,const char16*restrict src){ustrcpy(dest+ustrlen(dest),src);return dest;}
header_func const char*ustr2astrf(const char16*src){return src?ustr2astrnf(src,ustrsize(src)):NULL;}
header_func const char*ustr2xstrf(const char16*src){return src?ustr2xstrnf(src,ustrsize(src)):NULL;}
header_func const char16*astr2ustrf(const char*src){return src?astr2ustrnf(src,strlen(src)+1):NULL;}
header_func char*ustr2astra(const char16*src){return src?ustr2astrna(src,ustrsize(src)):NULL;}
header_func char*ustr2xstra(const char16*src){return src?ustr2xstrna(src,ustrsize(src)):NULL;}
header_func char16*astr2ustra(const char*src){return src?astr2ustrna(src,strlen(src)+1):NULL;}
header_func size_t ustr2xstr(char*dest,size_t dsize,const char16*src){return src?ustr2xstrn(dest,dsize,src,ustrsize(src)):0;}
header_func size_t ustr2astr(char*dest,size_t dsize,const char16*src){return src?ustr2astrn(dest,dsize,src,ustrsize(src)):0;}
header_func size_t astr2ustr(char16*dest,size_t dsize,const char*src){return src?astr2ustrn(dest,dsize,src,strlen(src)+1):0;}
header_func bool uisupper(char16 c){return (char16)c>=(char16)L'A'&&(char16)c<=(char16)L'Z';}
header_func bool uislower(char16 c){return (char16)c>=(char16)L'a'&&(char16)c<=(char16)L'z';}
header_func bool uisspace(char16 c){return (char16)c==(char16)L' '||(char16)c-(char16)L'\t'<5;}
header_func bool uisalpha(char16 c){return ((char16)c|32)-(char16)L'a'<26;}
header_func bool uisdigit(char16 c){return (char16)c-(char16)L'0'<10;}
header_func bool uisxdigit(char16 c){return uisdigit(c)||((char16)c|32)-L'a'<6;}
header_func bool uisalnum(char16 c){return uisalpha(c)||uisdigit(c);}
header_func bool uiscntrl(char16 c){return (char16)c<0x20||c==0x7f;}
header_func bool uisgraph(char16 c){return (char16)c-0x21<0x5e;}
header_func bool uispunct(char16 c){return uisgraph(c)&&!uisalnum(c);}
header_func bool uisprint(char16 c){return (char16)c-0x20<0x5f;}
header_func char16 utoupper(char16 c){return uislower(c)?(char16)(c-(char16)(L'a'-L'A')):c;}
header_func char16 utolower(char16 c){return uisupper(c)?(char16)(c+(char16)(L'a'-L'A')):c;}

#endif //EFI_WRAPPER_EFI_STRING_H
