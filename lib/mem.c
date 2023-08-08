#include<stdlib.h>
#include<string.h>
#include"defines.h"
#include"logger.h"
#include"mem.h"

#define MAGIC "XMEM"
#define MAGIC_DESC "DESC"
#define MAGIC_POOL "POOL"

static void*std_allocate(mem_pool*pool,size_t len){
	if(!pool||len<=0)return NULL;
	return malloc(len);
}

static void*std_reallocate(mem_pool*pool,void*ptr,size_t len){
	if(!pool||len<=0)return NULL;
	if(!ptr)return std_allocate(pool,len);
	return realloc(ptr,len);
}

static void std_free(mem_pool*pool,void*ptr){
	if(!pool||!ptr)return;
	free(ptr);
}

static mem_pool global={
	.head={.magic0=MAGIC,.magic1=MAGIC_POOL},
	.used_size=0,.alloc_size=0,.count=0,.desc=NULL,
	.allocate=std_allocate,
	.reallocate=std_reallocate,
	.free=std_free,
};

static inline bool check_magic(mem_head*h,const char*magic1){
	if(!h)return false;
	if(memcmp(h->magic0,MAGIC,sizeof(h->magic0))!=0)return false;
	if(memcmp(h->magic1,magic1,sizeof(h->magic1))!=0)return false;
	return true;
}

static inline void set_magic(mem_head*h,const char*magic1){
	if(!h)return;
	memcpy(h->magic0,MAGIC,sizeof(h->magic0));
	memcpy(h->magic1,magic1,sizeof(h->magic1));
}

static inline bool check_buffer(mem_desc*desc,void*buff){
	uint32_t sum1,sum2;
	if(!desc||!buff)return false;
	if(!check_magic(&desc->head,MAGIC_DESC))
		EDONE(xerror("invalid memory block magic"));
	sum1=desc->crc,desc->crc=0;
	sum2=s_crc32(desc,sizeof(mem_desc)),desc->crc=sum2;
	if(sum2!=sum1)EDONE(xerror("head checksum failed"));
	if(desc->list.data!=desc)EDONE(xerror("invalid list link"));
	if(desc->buff!=buff)EDONE(xerror("invalid buffer"));
	if(!desc->pool||!check_magic(&desc->pool->head,MAGIC_POOL))
		EDONE(xerror("invalid memory pool"));
	if(desc->real_size>desc->pool->used_size)
		EDONE(xerror("bogus pool size"));
	if(desc->size>desc->pool->alloc_size)
		EDONE(xerror("bogus pool size"));
	if(buff-desc->offset!=desc->data)
		EDONE(xerror("invalid block offset"));
	return true;
	done:
	return false;
}

mem_pool*mem_get_global_pool(){
	return &global;
}

mem_pool*mem_new_pool(){
	mem_pool*pool=malloc(sizeof(mem_pool));
	if(!pool)return NULL;
	memset(pool,0,sizeof(mem_pool));
	set_magic(&pool->head,MAGIC_POOL);
	return pool;
}

char*mem_strdup(mem_pool*pool,const char*buff){
	char*out;
	size_t len;
	if(!buff)return NULL;
	len=strlen(buff);
	if(!(out=mem_allocate(pool,len+1)))return NULL;
	memset(out,0,len+1);
	memcpy(out,buff,len);
	return out;
}

char*mem_strndup(mem_pool*pool,const char*buff,size_t l){
	char*out;
	size_t len;
	if(!buff)return NULL;
	len=strnlen(buff,l);
	if(!(out=mem_allocate(pool,len+1)))return NULL;
	memset(out,0,len+1);
	memcpy(out,buff,len);
	return out;
}

void*mem_memdup(mem_pool*pool,const void*buff,size_t len){
	void*out;
	if(!buff)return NULL;
	if(!(out=mem_allocate(pool,len)))return NULL;
	memcpy(out,buff,len);
	return out;
}

static void update_sum(mem_desc*desc){
	if(!desc)return;
	desc->crc=0,desc->crc=s_crc32(desc,sizeof(mem_desc));
}

void*mem_allocate(mem_pool*pool,size_t size){
	void*data,*buff;
	mem_head*foot=NULL;
	mem_desc*desc=NULL;
	if(!pool)pool=&global;
	if(size<=0||!check_magic(&pool->head,MAGIC_POOL))return NULL;
	size_t ms=sizeof(mem_desc)+size+sizeof(mem_head);
	if(!(data=pool->allocate(pool,ms)))return NULL;
	desc=data,buff=data+sizeof(mem_desc),foot=buff+size;
	memset(desc,0,sizeof(mem_desc));
	set_magic(&desc->head,MAGIC_DESC);
	set_magic(foot,MAGIC_DESC);
	desc->offset=buff-data;
	desc->align=0,desc->size=size;
	desc->data=data,desc->buff=buff;
	desc->real_size=ms,desc->pool=pool;
	desc->list.data=desc;
	pool->used_size+=ms;
	pool->alloc_size+=size;
	pool->count++;
	list_obj_add(&pool->desc,&desc->list);
	if(desc->list.prev)update_sum(desc->list.prev->data);
	if(desc->list.next)update_sum(desc->list.next->data);
	update_sum(desc);
	return buff;
}

void*mem_reallocate(void*ptr,size_t size){
	void*data,*buff;
	mem_head*foot;
	mem_desc*desc,tmp;
	mem_desc*old=ptr-sizeof(mem_desc);
	if(!ptr||size<=0)return NULL;
	if(!check_buffer(old,ptr))goto done;
	if(!check_magic(ptr+old->size,MAGIC_DESC))
		EDONE(xerror("MEMORY ERROR: detect invalid write on foot"));
	if(old->align!=0){
		xerror("cannot realloc an aligned pool");
		return NULL;
	}
	if(old->size==size)return ptr;
	if(!list_search_one(
		old->pool->desc,list_pointer_comparator,old
	))EDONE(xerror("invalid pool list"));
	size_t ms=sizeof(mem_desc)+size+sizeof(mem_head);
	memcpy(&tmp,old,sizeof(mem_desc));
	if(!(data=old->pool->reallocate(old->pool,old->data,ms)))goto done;
	desc=data,buff=data+sizeof(mem_desc),foot=buff+size;
	set_magic(foot,MAGIC_DESC);
	memcpy(desc,&tmp,sizeof(mem_desc));
	desc->pool->used_size-=desc->real_size;
	desc->pool->used_size+=ms;
	desc->pool->alloc_size-=desc->size;
	desc->pool->alloc_size+=size;
	desc->data=data,desc->buff=buff;
	desc->align=0,desc->size=size;
	desc->real_size=ms;
	desc->list.data=desc;
	if(desc->pool->desc==&desc->list)desc->pool->desc=&desc->list;
	if(desc->list.prev){
		desc->list.prev->next=&desc->list;
		update_sum(desc->list.prev->data);
	}
	if(desc->list.next){
		desc->list.next->prev=&desc->list;
		update_sum(desc->list.next->data);
	}
	update_sum(desc);
	return buff;
	done:
	abort();
}

void*mem_aligned_allocate(mem_pool*pool,size_t align,size_t size){
	mem_desc*desc;
	mem_head*foot;
	void*data,*buff;
	if(!pool)pool=&global;
	if(align<=0)return mem_allocate(pool,size);
	if(size<=0||(align%sizeof(void*))!=0)return NULL;
	if(!check_magic(&pool->head,MAGIC_POOL))return NULL;
	size_t ext=(align*2)+sizeof(mem_desc)+sizeof(mem_head);
	size_t rs=round_up(size,align);
	size_t ms=round_up(rs+ext,align);
	if(!(data=pool->allocate(pool,ms)))return NULL;
	buff=(void*)round_up((size_t)data,align);
	while((size_t)(buff-data)<sizeof(mem_desc))buff+=align;
	desc=buff-sizeof(mem_desc),foot=buff+rs;
	memset(desc,0,sizeof(mem_desc));
	set_magic(&desc->head,MAGIC_DESC);
	set_magic(foot,MAGIC_DESC);
	desc->align=align,desc->size=rs;
	desc->data=data,desc->buff=buff;
	desc->real_size=ms,desc->pool=pool;
	desc->offset=buff-data;
	desc->list.data=desc;
	pool->used_size+=ms;
	pool->alloc_size+=rs;
	pool->count++;
	list_obj_add(&pool->desc,&desc->list);
	if(desc->list.prev)update_sum(desc->list.prev->data);
	if(desc->list.next)update_sum(desc->list.next->data);
	update_sum(desc);
	return buff;
}

void mem_free(void*buff){
	mem_pool*pool;
	mem_desc*desc=buff-sizeof(mem_desc);
	if(!check_buffer(desc,buff))abort();
	if(!check_magic(buff+desc->size,MAGIC_DESC)){
		xerror("MEMORY ERROR: detect invalid write on foot");
		abort();
	}
	pool=desc->pool;
	if(!list_search_one(pool->desc,list_pointer_comparator,desc)){
		xerror("invalid pool list");
		abort();
	}
	list*p=desc->list.prev,*n=desc->list.next;
	list_obj_strip(&pool->desc,&desc->list);
	if(p)update_sum(p->data);
	if(n)update_sum(n->data);
	pool->used_size-=desc->real_size;
	pool->alloc_size-=desc->size;
	pool->count--;
	void*data=desc->data;
	memset(desc,0,sizeof(mem_desc));
	pool->free(pool,data);
}

bool mem_check_pool(void*buff,mem_pool*pool){
	mem_desc*desc=buff-sizeof(mem_desc);
	if(!check_buffer(desc,buff))return false;
	return desc->pool==pool;
}

mem_desc*mem_get_desc(void*buff){
	mem_desc*desc=buff-sizeof(mem_desc);
	return check_buffer(desc,buff)?desc:NULL;
}
