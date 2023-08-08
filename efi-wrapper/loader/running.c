#include<unistd.h>
#include<string.h>
#include<threads.h>
#include"efi/efi_file.h"
#include"efi/device_path.h"

static inline bool check_file_stack(efi_file*file){
	return file&&file->image&&file->image_size>0&&file->entry&&file->context;
}

list**efi_running_stack(){
	static thread_local list*efi_run_stack=NULL;
	return &efi_run_stack;
}

efi_file*efi_running_get(){
	list*l=list_last(*efi_running_stack());
	return l?LIST_DATA(l,efi_file*):NULL;
}

bool efi_is_running(efi_file*file){
	if(!check_file_stack(file))return false;
	return list_search_one(
		*efi_running_stack(),
		list_pointer_comparator,
		file
	)!=NULL;
}

bool efi_running_push(efi_file*file){
	if(efi_is_running(file))return false;
	list_obj_add_new(NULL,efi_running_stack(),file);
	return true;
}

void efi_running_pop(){
	list*l=list_last(*efi_running_stack());
	if(l)list_obj_del(efi_running_stack(),l,NULL);
}

void efi_running_remove(efi_file*file){
	list_obj_del_data(efi_running_stack(),file,NULL);
}

efi_file*efi_running_lookup_ptr(size_t ptr){
	list*l;
	if((l=list_first(*efi_running_stack())))do{
		LIST_DATA_DECLARE(file,l,efi_file*);
		if(!check_file_stack(file))continue;
		if(IS_BETWEEN_MEM(
			ptr,(size_t)file->image,
			file->image_size
		))return file;
	}while((l=l->next));
	return NULL;
}
