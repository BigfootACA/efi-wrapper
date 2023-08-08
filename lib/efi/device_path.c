//
// Created by bigfoot on 2023/5/10.
//
#include"path.h"
#include"efi/device_path.h"
#include"efi/efi_context.h"
#include"efi/efi_wrapper.h"

const efi_device_path efi_device_path_end={
	END_DEVICE_PATH_TYPE,
	END_ENTIRE_DEVICE_PATH_SUBTYPE,
	sizeof(efi_device_path),
};

uint8_t efi_device_path_get_type(const void*node){
	return node?((efi_device_path*)(node))->type:0;
}

uint8_t efi_device_path_get_sub_type(const void*node){
	return node?((efi_device_path*)(node))->sub_type:0;
}

uint16_t efi_device_path_get_node_length(const void*node){
	return node?((efi_device_path*)(node))->length:0;
}

uint16_t efi_device_path_set_node_length(void*node,uint16_t length){
	if(!node||length<sizeof(efi_device_path))return 0;
	((efi_device_path*)(node))->length=length;
	return length;
}

efi_device_path*efi_device_path_node_next(const void*node){
	if(!node||efi_device_path_is_end(node))return NULL;
	node+=efi_device_path_get_node_length(node);
	return (efi_device_path*)node;
}

bool efi_device_path_is_end_type(const void*node){
	return node&&efi_device_path_get_type(node)==END_DEVICE_PATH_TYPE;
}

bool efi_device_path_is_end_sub_type(const void*node){
	return node&&efi_device_path_get_sub_type(node)==END_ENTIRE_DEVICE_PATH_SUBTYPE;
}

bool efi_device_path_is_end_instance_sub_type(const void*node){
	return node&&efi_device_path_get_sub_type(node)==END_INSTANCE_DEVICE_PATH_SUBTYPE;
}

bool efi_device_path_is_end(const void*node){
	return node&&efi_device_path_is_end_type(node)&&efi_device_path_is_end_sub_type(node);
}

bool efi_device_path_is_end_instance(const void*node){
	return node&&efi_device_path_is_end_type(node)&&efi_device_path_is_end_instance_sub_type(node);
}

bool efi_device_path_valid(const efi_device_path*dp,size_t max){
	size_t size,node;
	if(!dp||(max>0&&max<sizeof(efi_device_path)))return false;
	if(max==0)max=UINTN_MAX;
	for(size=0;!efi_device_path_is_end(dp);dp=efi_device_path_node_next(dp)){
		node=efi_device_path_get_node_length(dp);
		if(node<sizeof(efi_device_path))return false;
		if(node>UINTN_MAX-size)return false;
		size+=node;
		if(size>max-sizeof(efi_device_path))return false;
		if(
			efi_device_path_get_type(dp)==MEDIA_DEVICE_PATH&&
			efi_device_path_get_sub_type(dp)==MEDIA_FILEPATH_DP&&
			*(char16*)((uint8_t*)dp+node-2)!=0
		)return false;
	}
	return efi_device_path_get_node_length(dp)==sizeof(efi_device_path);
}

void efi_device_path_add_node_end(void*node){
	if(!node)return;
	memcpy(node,&efi_device_path_end,sizeof(efi_device_path_end));
}

size_t efi_device_path_get_size(const efi_device_path*dp){
	const efi_device_path*start;
	if(!dp||!efi_device_path_valid(dp,0))return 0;
	start=dp;
	while(!efi_device_path_is_end(dp))dp=efi_device_path_node_next(dp);
	return ((uintn_t)dp-(uintn_t)start)+efi_device_path_get_node_length(dp);
}

size_t efi_device_path_get_instance_size(const efi_device_path*dp){
        size_t size=0;
        if(!dp||efi_device_path_is_end(dp))return 0;
        do{size+=efi_device_path_get_node_length(dp);}
	while((dp=efi_device_path_node_next(dp))&&!efi_device_path_is_end(dp));
        return size;
}

efi_device_path*efi_device_path_dup(mem_pool*pool,const efi_device_path*dp){
	size_t size=efi_device_path_get_size(dp);
	return size<=0?NULL:mem_memdup(pool,dp,size);
}

efi_device_path*efi_device_path_append(
	mem_pool*pool,
	const efi_device_path*dp1,
	const efi_device_path*dp2
){
	uintn_t size,size1,size2;
	efi_device_path*ret,*next;
	if(!dp1)return efi_device_path_dup(pool,dp2?dp2:&efi_device_path_end);
	if(!dp2)return efi_device_path_dup(pool,dp1);
	if(!efi_device_path_valid(dp1,0)||!efi_device_path_valid(dp2,0))return NULL;
	size1=efi_device_path_get_size(dp1);
	size2=efi_device_path_get_size(dp2);
	size=size1+size2-sizeof(efi_device_path);
	if(!(ret=mem_allocate(pool,size)))return NULL;
	ret=memcpy(ret,dp1,size1);
	next=(efi_device_path*)((char8*)ret+(size1-sizeof(efi_device_path)));
	memcpy(next,dp2,size2);
	return ret;
}

efi_device_path*efi_device_path_node_append(
	mem_pool*pool,
	const efi_device_path*dp,
	const efi_device_path*node
){
	uintn_t len;
	efi_device_path*buf,*next,*ret;
	if(!node)return efi_device_path_dup(pool,dp?dp:&efi_device_path_end);
	len=efi_device_path_get_node_length(node);
	if(!(buf=mem_allocate(pool,len+sizeof(efi_device_path))))return NULL;
	buf=memcpy(buf,node,len);
	next=efi_device_path_node_next(buf);
	efi_device_path_add_node_end(next);
	ret=efi_device_path_append(pool,dp,buf);
	mem_free(buf);
	return ret;
}

efi_device_path*efi_device_path_instance_append(
	mem_pool*pool,
	const efi_device_path*dp,
	const efi_device_path*instance
){
	uintn_t src,ins;
	efi_device_path*ret,*buf;
	if(!dp)return efi_device_path_dup(pool,instance);
	if(!efi_device_path_valid(dp,0)||!efi_device_path_valid(instance,0))return NULL;
	src=efi_device_path_get_size(dp);
	ins=efi_device_path_get_size(instance);
	if(!(ret=mem_allocate(pool,src+ins)))return NULL;
	buf=memcpy(ret,dp,src);
	while(!efi_device_path_is_end(buf))
		buf=efi_device_path_node_next(buf);
	buf->sub_type=END_INSTANCE_DEVICE_PATH_SUBTYPE;
	buf=efi_device_path_node_next(buf);
	memcpy(buf,instance,ins);
	return ret;
}

efi_device_path*efi_device_path_instance_next(
	mem_pool*pool,
	efi_device_path**dp,
	uintn_t*size
){
	uint8_t sub_type;
	efi_device_path*p,*ret;
	if(!dp||!*dp||!size||!efi_device_path_valid(*dp,0)){
		if(size)*size=0;
		return NULL;
	}
	for(p=*dp;!efi_device_path_is_end_type(p);p=efi_device_path_node_next(p));
	*size=((uintn_t)p-(uintn_t)(*dp))+sizeof(efi_device_path);
	sub_type=p->sub_type;
	p->sub_type=END_ENTIRE_DEVICE_PATH_SUBTYPE;
	ret=efi_device_path_dup(pool,*dp);
	p->sub_type=sub_type;
	*dp=efi_device_path_node_next(p);
	return ret;
}

bool efi_device_path_is_multi_instance(const efi_device_path*dp){
	const efi_device_path*node;
	if(!efi_device_path_valid(dp,0))return false;
	node=dp;
	while(!efi_device_path_is_end(node)){
		if(efi_device_path_is_end_instance(node))return true;
		node=efi_device_path_node_next(node);
	}
	return false;
}

efi_device_path*efi_device_path_create_node(
	mem_pool*pool,
	uint8_t type,
	uint8_t sub_type,
	uint16_t length
){
	efi_device_path*dp;
	if(length<sizeof(efi_device_path))return NULL;
	if(!(dp=mem_allocate(pool,length)))return NULL;
        memset(dp,0,length);
	dp->type=type,dp->sub_type=sub_type;
	efi_device_path_set_node_length(dp,length);
	return dp;
}

efi_device_path*efi_device_path_from_file_str16(
	mem_pool*pool,
	efi_handle handle,
	const char16*file
){
	size_t size;
	struct{
		filepath_device_path dp;
		char16 path[4092];
		efi_device_path end;
	}cdecl_attr_packed dp;
	efi_device_path*parent=NULL;
	if(!file||!*file||(size=ustrsize(file))>sizeof(dp.path))return NULL;
	memset(&dp,0,sizeof(dp));
	memcpy(dp.path,file,size);
	dp.dp.header.type=MEDIA_DEVICE_PATH;
	dp.dp.header.sub_type=MEDIA_FILEPATH_DP;
	efi_device_path_set_node_length(&dp.dp,sizeof(efi_device_path)+size);
	efi_device_path_add_node_end(efi_device_path_node_next(&dp.dp));
	if(handle&&!(parent=efi_device_path_from_handle(handle)))return NULL;
	return efi_device_path_append(pool,parent,&dp.dp.header);
}

efi_device_path*efi_device_path_from_file_str8(
	mem_pool*pool,
	efi_handle handle,
	const char*file
){
	char16 str[4096];
	if(!file||!*file)return NULL;
	memset(str,0,sizeof(str));
	astr2ustr(str,sizeof(str),file);
	return *str?efi_device_path_from_file_str16(pool,handle,str):NULL;
}

efi_device_path*efi_device_path_from_handle(
	efi_handle handle
){
	efi_status st;
	efi_interface_entry*ent;
	if(!efi_current_ctx||!efi_current_ctx->bs)return NULL;
	st=efi_get_protocol(handle,&gEfiDevicePathProtocolGuid,&ent);
	if(efi_error(st)||!ent)return NULL;
	return ent->interface;
}

ssize_t efi_device_path_get_path(const efi_device_path*dp,char sep,char*buff,size_t len){
	char path[4096],*str;
	filepath_device_path*fp;
	if(!dp||!buff||len<=0)ERET(EINVAL);
	memset(buff,0,len);
	memset(path,0,sizeof(path));
        do{
		if(
			efi_device_path_get_type(dp)!=MEDIA_DEVICE_PATH||
			efi_device_path_get_type(dp)!=MEDIA_FILEPATH_DP
		)continue;
	        fp=(filepath_device_path*)dp;
		if(path[0])strlcat(path,"/",sizeof(path)-1);
		if((str=ustr2astra(fp->path_name))){
			strlcat(path,str,sizeof(path)-1);
			free(str);
		}
	}while(
		(dp=efi_device_path_node_next(dp))&&
		!efi_device_path_is_end(dp)
	);
	if(!path[0])return 0;
	return (ssize_t)path_resolve(buff,len,sep,path);
}
