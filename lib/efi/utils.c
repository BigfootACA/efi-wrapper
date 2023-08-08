#include<time.h>
#include<unistd.h>
#include"mem.h"
#include"defines.h"
#include"efi/efi_hii.h"
#include"efi/efi_context.h"

bool efi_timestamp_to_time(time_t in,efi_time*out){
	struct tm*tm=localtime(&in);
	if(!tm||!out)return false;
	memset(out,0,sizeof(efi_time));
	out->year=1900+tm->tm_year;
	out->month=tm->tm_mon+1;
	out->day=tm->tm_mday;
	out->hour=tm->tm_hour;
	out->minute=tm->tm_min;
	out->second=tm->tm_sec;
	out->timezone=(int16_t)(tm->tm_gmtoff/60);
	out->daylight=(daylight?1:0)|(tm->tm_isdst>0?2:0);
	return true;
}

bool efi_timeval_to_time(struct timeval*in,efi_time*out){
	bool r;
	if(!in||!out)return false;
	r=efi_timestamp_to_time(in->tv_sec,out);
	if(r)out->nanosecond=in->tv_usec*1000;
	return r;
}

bool efi_timespec_to_time(struct timespec*in,efi_time*out){
	bool r;
	if(!in||!out)return false;
	r=efi_timestamp_to_time(in->tv_sec,out);
	if(r)out->nanosecond=in->tv_nsec;
	return r;
}

void efi_table_calc_sum(void*table,size_t len){
	uint32_t sum;
	efi_table_header*hdr=table;
	if(!table||len<sizeof(efi_table_header))return;
	hdr->crc32=0;
	sum=s_crc32(hdr,len);
	hdr->crc32=sum;
}
