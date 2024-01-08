# efi-wrapper

## Whats this?

A UEFI implemented running under OS (Linux), implemented some UEFI interfaces

NOTICE: the project is in the early experimental stage, and some functions may have many bugs and may not work as expected

**This project is not a sandbox, efi program code will be executed directly!**

## How does it work?

As we all know, UEFI does not have system calls.
Applications call UEFI services through System Table struct of arguments.
As long as we implement System Table and then emulate some privileged instructions, we can run the UEFI program.

## UEFI Shell

If you want to run UEFI Shell, you need HII related DXE (from EDK2):

- HiiDatabase.efi
- EnglishDxe.efi

## Implemented UEFI protocols

- gEfiDevicePathUtilitiesProtocolGuid
- gEfiComponentNameProtocolGuid
- gEfiComponentName2ProtocolGuid
- gEfiGraphicsOutputProtocolGuid (SDL2 / VNC / MemoryOnly)
- gEfiAbsolutePointerProtocolGuid (SDL2 / VNC)
- gEfiSimplePointerProtocolGuid (SDL2 / VNC)
- gEfiSimpleTextInProtocolGuid (SDL2 / VNC / STDIO)
- gEfiSimpleTextInputExProtocolGuid (partial)
- gEfiSimpleFileSystemProtocolGuid (write untested)
- gEfiSimpleTextOutProtocolGuid
- gEfiUnicodeCollationProtocolGuid
- gEfiUnicodeCollation2ProtocolGuid

## UEFI services (Boot Service / Runtime Service)

### Unimplemented services

- gRT->SetVirtualAddressMap (no possible)
- gRT->ConvertPointer (no possible)
- gRT->UpdateCapsule
- gRT->QueryCapsuleCapabilities
- gRT->SetTime (return EFI_ACCESS_DENIED)
- gRT->GetWakeupTime
- gRT->SetWakeupTime
- gRT->GetNextHighMonotonicCount
- gBS->GetNextMonotonicCount
- gBS->UnloadImage
- gBS->SetWatchdogTimer

### Partial implemented services

- gRT->ResetSystem (force exit application)
- gRT->SetVariable (no persistent storage)
- gRT->GetVariable (no persistent storage)
- gRT->GetNextVariableName (no persistent storage)
- gBS->ExitBootServices (fake exit)

## Architecture:

- x86_64 / amd64
- aarch64 / arm-v8a
- x86 / i386 (untested)
- arm32 / arm-v7 (untested)

## TODO

- Fix STDIO Console
- Implement SimpleTextInputEx
- Implement HII Protocols
- Emulate more privileged instructions

## Bugs

- All code are unstable
- Memory management (pool allocate) are very bogous
- Memory protection does not seem to be working properly, you may need add `-p` to arguments (enable memory permissive)
- PE32 relocations broken
- No persistent variable storage

## Code used: 

- edk2
- U-Boot
