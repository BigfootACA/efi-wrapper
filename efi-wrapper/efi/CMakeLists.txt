add_library(
	efi_wrapper_efi
	STATIC
	context.c
	service.c
	handles.c
)
add_subdirectory(data)
add_subdirectory(tables)
add_subdirectory(protocols)
target_link_libraries(
	efi_wrapper
	efi_wrapper_efi_data
	efi_wrapper_efi_tables
	efi_wrapper_efi_protocols
)
