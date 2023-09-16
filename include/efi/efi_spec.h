//
// Created by bigfoot on 2023/4/1.
//

#ifndef EFI_WRAPPER_EFI_SPEC_H
#define EFI_WRAPPER_EFI_SPEC_H
#include<string.h>
#include<sys/time.h>
#include"efi_types.h"
#include"efi_status.h"
#include"dxe_services.h"
#include"runtime_services.h"
#include"system_table.h"
#include"boot_services.h"
#define EFI_2_80_REVISION ((2<<16)|(80))
#define EFI_2_70_REVISION ((2<<16)|(70))
#define EFI_2_60_REVISION ((2<<16)|(60))
#define EFI_2_50_REVISION ((2<<16)|(50))
#define EFI_2_40_REVISION ((2<<16)|(40))
#define EFI_2_31_REVISION ((2<<16)|(31))
#define EFI_2_30_REVISION ((2<<16)|(30))
#define EFI_2_20_REVISION ((2<<16)|(20))
#define EFI_2_10_REVISION ((2<<16)|(10))
#define EFI_2_00_REVISION ((2<<16)|(00))
#define EFI_1_10_REVISION ((1<<16)|(10))
#define EFI_1_02_REVISION ((1<<16)|(02))
#define SIZE_1KB   (1024UL)
#define SIZE_2KB   (2    * SIZE_1KB)
#define SIZE_4KB   (4    * SIZE_1KB)
#define SIZE_8KB   (8    * SIZE_1KB)
#define SIZE_16KB  (16   * SIZE_1KB)
#define SIZE_32KB  (32   * SIZE_1KB)
#define SIZE_64KB  (64   * SIZE_1KB)
#define SIZE_128KB (128  * SIZE_1KB)
#define SIZE_256KB (256  * SIZE_1KB)
#define SIZE_512KB (512  * SIZE_1KB)
#define SIZE_1MB   (1024 * SIZE_1KB)
#define SIZE_2MB   (2    * SIZE_1MB)
#define SIZE_4MB   (4    * SIZE_1MB)
#define SIZE_8MB   (8    * SIZE_1MB)
#define SIZE_16MB  (16   * SIZE_1MB)
#define SIZE_32MB  (32   * SIZE_1MB)
#define SIZE_64MB  (64   * SIZE_1MB)
#define SIZE_128MB (128  * SIZE_1MB)
#define SIZE_256MB (256  * SIZE_1MB)
#define SIZE_512MB (512  * SIZE_1MB)
#define SIZE_1GB   (1024 * SIZE_1MB)
#define SIZE_2GB   (2    * SIZE_1GB)
#define SIZE_4GB   (4    * SIZE_1GB)
#define SIZE_8GB   (8    * SIZE_1GB)
#define SIZE_16GB  (16   * SIZE_1GB)
#define SIZE_32GB  (32   * SIZE_1GB)
#define SIZE_64GB  (64   * SIZE_1GB)
#define SIZE_128GB (128  * SIZE_1GB)
#define SIZE_256GB (256  * SIZE_1GB)
#define SIZE_512GB (512  * SIZE_1GB)
#define SIZE_1TB   (1024 * SIZE_1GB)
#define SIZE_2TB   (2    * SIZE_1TB)
#define SIZE_4TB   (4    * SIZE_1TB)
#define SIZE_8TB   (8    * SIZE_1TB)
#define SIZE_16TB  (16   * SIZE_1TB)
#define SIZE_32TB  (32   * SIZE_1TB)
#define SIZE_64TB  (64   * SIZE_1TB)
#define SIZE_128TB (128  * SIZE_1TB)
#define SIZE_256TB (256  * SIZE_1TB)
#define SIZE_512TB (512  * SIZE_1TB)
#define SIZE_1PB   (1024 * SIZE_1TB)
#define EFI_SPECIFICATION_VERSION EFI_2_70_REVISION
static inline bool efi_guid_equals(const efi_guid*guid1,const efi_guid*guid2){
	return guid1&&guid2&&memcmp(guid1,guid2,sizeof(efi_guid))==0;
}
extern bool efi_timestamp_to_time(time_t in,efi_time*out);
extern bool efi_timeval_to_time(struct timeval*in,efi_time*out);
extern bool efi_timespec_to_time(struct timespec*in,efi_time*out);
typedef efiapi efi_status(*efi_image_entry_point)(
	efi_handle image_handle,
	efi_system_table*system_table
);
#endif //EFI_WRAPPER_EFI_SPEC_H
