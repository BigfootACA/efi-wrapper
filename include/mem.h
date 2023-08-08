//
// Created by bigfoot on 2023/4/6.
//

#ifndef EFI_WRAPPER_MEM_H
#define EFI_WRAPPER_MEM_H
#include<stdbool.h>
#include<sys/types.h>

typedef struct mem_head mem_head;
typedef struct mem_pool mem_pool;
typedef struct mem_desc mem_desc;

extern mem_pool*mem_get_global_pool();
extern mem_pool*mem_new_pool();
extern char*mem_strdup(mem_pool*pool,const char*buff);
extern char*mem_strndup(mem_pool*pool,const char*buff,size_t l);
extern void*mem_memdup(mem_pool*pool,const void*buff,size_t len);
extern void*mem_allocate(mem_pool*pool,size_t size);
extern void*mem_reallocate(void*ptr,size_t size);
extern void*mem_aligned_allocate(mem_pool*pool,size_t align,size_t size);
extern bool mem_check_pool(void*buff,mem_pool*pool);
extern mem_desc*mem_get_desc(void*buff);
extern void mem_free(void*buff);

#include"list.h"

struct mem_head{
	char magic0[4];
	char magic1[4];
};

struct mem_pool{
	mem_head head;
	size_t count;
	size_t used_size;
	size_t alloc_size;
	list*desc;
	void*(*allocate)(mem_pool*pool,size_t len);
	void*(*reallocate)(mem_pool*pool,void*ptr,size_t len);
	void(*free)(mem_pool*pool,void*ptr);
	void*userdata[4];
};

struct mem_desc{
	mem_head head;
	list list;
	mem_pool*pool;
	void*data;
	void*buff;
	uint32_t crc;
	uint16_t offset;
	uint16_t align;
	size_t size;
	size_t real_size;
};

#endif //EFI_WRAPPER_MEM_H
