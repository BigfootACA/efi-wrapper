//
// Created by bigfoot on 2023/5/7.
//

#include<iconv.h>
#include<threads.h>
#include"efi/efi_string.h"

ssize_t ustrcmp(const char16*str1,const char16*str2){
	if(ustrsize(str1)==0||ustrsize(str2)==0)return -1;
	while(*str1&&*str2&&*str1==*str2)str1++,str2++;
	return *str1-*str2;
}

ssize_t ustrncmp(const char16*str1,const char16*str2,size_t len){
	if(len==0)return 0;
	if(ustrsize(str1)==0||ustrsize(str2)==0)return -1;
	while(*str1&&*str2&&*str1==*str2&&len>1)str1++,str2++,len--;
	return *str1-*str2;
}

ssize_t ustrcasecmp(const char16*str1,const char16*str2){
	if(ustrsize(str1)==0||ustrsize(str2)==0)return -1;
	while(*str1&&*str2&&utoupper(*str1)==utoupper(*str2))str1++,str2++;
	return utoupper(*str1)-utoupper(*str2);
}

ssize_t ustrncasecmp(const char16*str1,const char16*str2,size_t len){
	if(len==0)return 0;
	if(ustrsize(str1)==0||ustrsize(str2)==0)return -1;
	while(*str1&&*str2&&utoupper(*str1)==utoupper(*str2)&&len>1)str1++,str2++,len--;
	return utoupper(*str1)-utoupper(*str2);
}

char16*ustrstr(const char16*str,const char16*key){
	const char16*match;
	const char16*tmp;
	if(ustrsize(str)==0||ustrsize(key)==0)return NULL;
	if(!*key||!*str)return (char16*)str;
	while(*str){
		tmp=key,match=str;
		while(*str==*tmp&&*str)str++,tmp++;
		if(!*tmp)return(char16*)match;
		if(!*str)return NULL;
		str=match+1;
	}
	return NULL;
}

char16*ustrcasestr(const char16*str,const char16*key){
	size_t len=ustrlen(key);
	for(;*str;str++)if(!ustrncasecmp(str,key,len))return(char16*)str;
	return NULL;
}

char16*ustrchrnul(const char16*str,char16 c){
	if(c==0)return(char16*)str+ustrlen(str);
	for(;*str&&*(uchar16*)str!=c;str++);
	return (char16*)str;
}

char16*ustrncat(char16*restrict dest,const char16*restrict src,size_t n){
	char16*a=dest;
	dest+=ustrlen(dest);
	while(n&&*src)n--,*dest++=*src++;
	*dest++=0;
	return a;
}

size_t ustrlcpy(char16*dest,const char16*src,size_t n){
	char16*d0=dest;
	if(!n--)goto finish;
	for(;n&&(*dest=*src);n--,src++,dest++);
	*dest=0;
	finish:
	return dest-d0+ustrlen(src);
}

size_t ustrlcat(char16*dest,const char16*src,size_t n){
	size_t l=ustrnlen(dest,n);
	if(l==n)return l+ustrlen(src);
	return l+ustrlcpy(dest+l,src,n-l);
}

size_t ustr2astrn(char*dest,size_t dsize,const char16*src,size_t ssize){
	size_t sin,sout,out;
	char*pin,*pout;
	static iconv_t ic=NULL;
	if(!dest||dsize<=0||!src||ssize<=0)return 0;
	if(!ic&&!(ic=iconv_open("UTF-8//TRANSLIT","UCS-2")))return 0;
	memset(dest,0,dsize);
	sin=ssize,pin=(char*)src,pout=dest,sout=dsize;
	out=iconv(ic,&pin,&sin,&pout,&sout);
	if(out==(size_t)-1)return 0;
	return dsize-sout-1;
}

size_t astr2ustrn(char16*dest,size_t dsize,const char*src,size_t ssize){
	size_t sin,sout,out;
	char*pin,*pout;
	static iconv_t ic=NULL;
	if(!dest||dsize<=0||!src||ssize<=0)return 0;
	if(!ic&&!(ic=iconv_open("UCS-2//TRANSLIT","UTF-8")))return 0;
	memset(dest,0,dsize);
	sin=ssize,pin=(char*)src,pout=(char*)dest,sout=dsize;
	out=iconv(ic,&pin,&sin,&pout,&sout);
	if(out==(size_t)-1)return 0;
	return dsize-sout-1;
}

size_t ustr2xstrn(char*dest,size_t dsize,const char16*src,size_t ssize){
	size_t len=0;
	static const char hex_table[]="0123456789ABCDEF";
	if(!dest||dsize<=0||!src||ssize<=0)return 0;
	memset(dest,0,dsize);
	for(;ssize>0&&dsize>0&&*src;ssize-=sizeof(char16),src++){
		switch(*src){
			case '\r':case '\n':case '\t':
				if(dsize<2)continue;
				*dest++='\\';
				switch(*src){
					case '\r':*dest++='r';break;
					case '\n':*dest++='n';break;
					case '\t':*dest++='t';break;
				}
				dsize-=sizeof(char)*2,len+=2;
			continue;
		}
		if(!uisprint(*src)){
			if(dsize<6)continue;
			*dest++='\\',*dest++='u';
			*dest++=hex_table[((*src)>>12)&0xF];
			*dest++=hex_table[((*src)>>8)&0xF];
			*dest++=hex_table[((*src)>>4)&0xF];
			*dest++=hex_table[((*src)>>0)&0xF];
			dsize-=sizeof(char)*6,len+=6;
			continue;
		}
		dsize-=sizeof(char);
		*dest++=*src,len++;
	}
	return len;
}

const char*ustr2astrnf(const char16*src,size_t ssize){
	static thread_local char buff[8192];
	size_t r=ustr2astrn(buff,sizeof(buff),src,ssize);
	return r>0?buff:NULL;
}

const char*ustr2xstrnf(const char16*src,size_t ssize){
	static thread_local char buff[8192];
	size_t r=ustr2xstrn(buff,sizeof(buff),src,ssize);
	return r>0?buff:NULL;
}

const char16*astr2ustrnf(const char*src,size_t ssize){
	static thread_local char16 buff[4096];
	size_t r=astr2ustrn(buff,sizeof(buff),src,ssize);
	return r>0?buff:NULL;
}

char*ustr2astrna(const char16*src,size_t ssize){
	size_t len=(ssize+1)*sizeof(char);
	char*buff=malloc(len);
	if(!buff)return NULL;
	memset(buff,0,len);
	if(ustr2astrn(buff,len,src,ssize)<=0){
		free(buff);
		buff=NULL;
	}
	return buff;
}

char*ustr2xstrna(const char16*src,size_t ssize){
	size_t len=(ssize+1)*sizeof(char);
	char*buff=malloc(len);
	if(!buff)return NULL;
	memset(buff,0,len);
	if(ustr2xstrn(buff,len,src,ssize)<=0){
		free(buff);
		buff=NULL;
	}
	return buff;
}

char16*astr2ustrna(const char*src,size_t ssize){
	size_t len=(ssize+1)*sizeof(char16);
	char16*buff=malloc(len);
	if(!buff)return NULL;
	memset(buff,0,len);
	if(astr2ustrn(buff,len,src,ssize)<=0){
		free(buff);
		buff=NULL;
	}
	return buff;
}
