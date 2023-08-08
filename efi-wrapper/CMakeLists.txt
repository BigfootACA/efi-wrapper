add_subdirectory(efi)
add_subdirectory(loader)
target_link_libraries(
	efi_wrapper
	efi_wrapper_efi
	efi_wrapper_loader
)
