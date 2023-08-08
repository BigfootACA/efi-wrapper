#include"pe.h"

const char*pe_flag_to_string(pe_flag flag,const char*def){
	switch(flag){
		case FLAG_RELOCS_STRIPPED:return "Relocations Stripped";
		case FLAG_EXECUTABLE_IMAGE:return "Executable Image";
		case FLAG_LINE_NUMS_STRIPPED:return "Line Numbers Stripped";
		case FLAG_LOCAL_SYMS_STRIPPED:return "LOCAL Symbols Stripped";
		case FLAG_AGGRESIVE_WS_TRIM:return "Aggressively trim working set";
		case FLAG_LARGE_ADDRESS_AWARE:return "Large Address Aware";
		case FLAG_BYTES_REVERSED_LO:return "Reversed Low";
		case FLAG_32BIT_MACHINE:return "32-Bit Machine";
		case FLAG_DEBUG_STRIPPED:return "Debug Stripped";
		case FLAG_REMOVABLE_RUN_FROM_SWAP:return "Removable Run From Swap";
		case FLAG_NET_RUN_FROM_SWAP:return "Network Run From Swap";
		case FLAG_SYSTEM:return "System";
		case FLAG_DLL:return "DLL";
		case FLAG_UP_SYSTEM_ONLY:return "Uniprocessor System Only";
		case FLAG_BYTES_REVERSED_HI:return "Reversed High";
		default:return def;
	}
}

const char*pe_machine_type_to_string(pe_machine_type machine,const char*def){
	switch(machine){
		case MACHINE_I386:return "I386";
		case MACHINE_R3000:return "R3000";
		case MACHINE_R4000:return "R4000";
		case MACHINE_R10000:return "R10000";
		case MACHINE_WCEMIPSV2:return "WCEMIPSV2";
		case MACHINE_ALPHA:return "ALPHA";
		case MACHINE_SH3:return "SH3";
		case MACHINE_SH3DSP:return "SH3DSP";
		case MACHINE_SH3E:return "SH3E";
		case MACHINE_SH4:return "SH4";
		case MACHINE_SH5:return "SH5";
		case MACHINE_ARM:return "ARM";
		case MACHINE_ARMV7:return "ARMV7";
		case MACHINE_ARM64:return "ARM64";
		case MACHINE_THUMB:return "THUMB";
		case MACHINE_AM33:return "AM33";
		case MACHINE_POWERPC:return "POWERPC";
		case MACHINE_POWERPCFP:return "POWERPCFP";
		case MACHINE_IA64:return "IA64";
		case MACHINE_MIPS16:return "MIPS16";
		case MACHINE_ALPHA64:return "ALPHA64";
		case MACHINE_MIPSFPU:return "MIPSFPU";
		case MACHINE_MIPSFPU16:return "MIPSFPU16";
		case MACHINE_TRICORE:return "TRICORE";
		case MACHINE_CEF:return "CEF";
		case MACHINE_EBC:return "EBC";
		case MACHINE_AMD64:return "AMD64";
		case MACHINE_M32R:return "M32R";
		case MACHINE_CEE:return "CEE";
		default:return def;
	}
}

const char*pe_subsystem_to_string(pe_subsystem subsystem,const char*def){
	switch(subsystem){
		case SUBSYSTEM_NATIVE:return "Native";
		case SUBSYSTEM_WINDOWS_GUI:return "Windows GUI";
		case SUBSYSTEM_WINDOWS_CUI:return "Windows CUI";
		case SUBSYSTEM_OS2_CUI:return "OS2 CUI";
		case SUBSYSTEM_POSIX_CUI:return "Posix CUI";
		case SUBSYSTEM_NATIVE_WINDOWS:return "Native Windows";
		case SUBSYSTEM_WINDOWS_CE_GUI:return "Windows CE GUI";
		case SUBSYSTEM_EFI_APPLICATION:return "EFI Application";
		case SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:return "EFI Boot Service Driver";
		case SUBSYSTEM_EFI_RUNTIME_DRIVER:return "EFI Runtime Driver";
		case SUBSYSTEM_EFI_ROM:return "EFI Rom";
		case SUBSYSTEM_XBOX:return "X-Box";
		case SUBSYSTEM_WINDOWS_BOOT_APPLICATION:return "Windows Boot Application";
		default:return def;
	}
}

const char*pe_section_flag_to_string(pe_section_flag subsystem,const char*def){
	switch(subsystem){
		case SCN_TYPE_NO_PAD:return "Type No Pad";
		case SCN_CNT_CODE:return "Contains Code";
		case SCN_CNT_INITIALIZED_DATA:return "Contains Initialized Data";
		case SCN_CNT_UNINITIALIZED_DATA:return "Contains Uninitialized Data";
		case SCN_LNK_OTHER:return "Linker Other";
		case SCN_LNK_INFO:return "Linker Info";
		case SCN_LNK_REMOVE:return "Linker Remove";
		case SCN_LNK_COMDAT:return "Linker COMDAT";
		case SCN_NO_DEFER_SPEC_EXC:return "No Defer Special Exception";
		case SCN_GPREL:return "Global Pointer Reference";
		case SCN_MEM_PURGEABLE:return "Memory Purgeable";
		case SCN_MEM_LOCKED:return "Memory Locked";
		case SCN_MEM_PRELOAD:return "Memory PRELOAD";
		case SCN_ALIGN_1BYTES:return "Align 1-Bytes";
		case SCN_ALIGN_2BYTES:return "Align 2-Bytes";
		case SCN_ALIGN_4BYTES:return "Align 4-Bytes";
		case SCN_ALIGN_8BYTES:return "Align 8-Bytes";
		case SCN_ALIGN_16BYTES:return "Align 16-Bytes";
		case SCN_ALIGN_32BYTES:return "Align 32-Bytes";
		case SCN_ALIGN_64BYTES:return "Align 64-Bytes";
		case SCN_ALIGN_128BYTES:return "Align 128-Bytes";
		case SCN_ALIGN_256BYTES:return "Align 256-Bytes";
		case SCN_ALIGN_512BYTES:return "Align 512-Bytes";
		case SCN_ALIGN_1024BYTES:return "Align 1024-Bytes";
		case SCN_ALIGN_2048BYTES:return "Align 2048-Bytes";
		case SCN_ALIGN_4096BYTES:return "Align 4096-Bytes";
		case SCN_ALIGN_8192BYTES:return "Align 8192-Bytes";
		case SCN_ALIGN_MASK:return "Align Mask";
		case SCN_LNK_NRELOC_OVFL:return "Linker Number Of Relocations Overflow";
		case SCN_MEM_DISCARDABLE:return "Memory Discardable";
		case SCN_MEM_NOT_CACHED:return "Memory Not Cached";
		case SCN_MEM_NOT_PAGED:return "Memory Not Paged";
		case SCN_MEM_SHARED:return "Memory Shared";
		case SCN_MEM_EXECUTE:return "Memory Execute";
		case SCN_MEM_READ:return "Memory Read";
		case SCN_MEM_WRITE:return "Memory Write";
		default:return def;
	}
}
