#include<stdio.h>
#include<string.h>
#include"defines.h"
static const char*size_units[]={"B","KiB","MiB","GiB","TiB","PiB","EiB","ZiB","YiB",NULL};

const char*make_readable_str_buf(
	char*buf,size_t len,
	unsigned long long val,
	unsigned long block_size,
	unsigned long display
){
	int unit=0;
	memset(buf,0,len);
	if(val==0)return strncpy(buf,"0",len-1);
	if(block_size>1)val*=block_size;
	if(display)val+=display/2,val/=display;
	else while((val>=1024))val/=1024,unit++;
	snprintf(buf,len-1,"%llu %s",val,size_units[unit]);
	return buf;
}
