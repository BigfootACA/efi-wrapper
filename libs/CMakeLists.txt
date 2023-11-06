add_subdirectory(tlsf)
add_subdirectory(udisasm)
target_link_libraries(
	efi_wrapper
	efi_wrapper_libs_tlsf
	efi_wrapper_libs_udisasm
)
