#include <pch.h>
#include "Winhook.h"
#include "hookdefs.h"

FARPROC myGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
    VMProtectBeginVirtualization("GetProcAddress");
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule +
        ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    DWORD* addressOfFunctions = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfFunctions);
    WORD* addressOfNameOrdinals = (WORD*)((BYTE*)hModule + exportDirectory->AddressOfNameOrdinals);
    DWORD* addressOfNames = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfNames);

    for (DWORD i = 0; i < exportDirectory->NumberOfNames; ++i) {
        if (strcmp(lpProcName, (const char*)hModule + addressOfNames[i]) == 0) {
            return (FARPROC)((BYTE*)hModule + addressOfFunctions[addressOfNameOrdinals[i]]);
        }
    }

    return NULL;
    VMProtectEnd();
}

HMODULE h_Comctl32 = LoadLibraryW(VMProtectDecryptStringW(L"comctl32.dll"));
HMODULE h_Ntdll = GetModuleHandleW(VMProtectDecryptStringW(L"ntdll.dll"));
HMODULE h_Kernel32 = GetModuleHandleW(VMProtectDecryptStringW(L"kernel32.dll"));
HMODULE h_KernelBase = GetModuleHandleA(VMProtectDecryptStringA("KernelBase.dll"));
HMODULE h_User32 = GetModuleHandleW(VMProtectDecryptStringW(L"user32.dll"));

wine_get_versionFunc wine_get_version = reinterpret_cast<wine_get_versionFunc>(myGetProcAddress(h_Ntdll, VMProtectDecryptStringA("wine_get_version")));
//NtWriteVirtualMemoryFunc ntWriteVirtualMemory = reinterpret_cast<NtWriteVirtualMemoryFunc>(myGetProcAddress(hNtdll, VMProtectDecryptStringA("NtWriteVirtualMemory")));
LoadLibraryWFunc loadLibraryW = reinterpret_cast<LoadLibraryWFunc>(myGetProcAddress(h_Kernel32, VMProtectDecryptStringA("LoadLibraryW")));
LoadLibraryAFunc loadLibraryA = reinterpret_cast<LoadLibraryAFunc>(myGetProcAddress(h_Kernel32, VMProtectDecryptStringA("LoadLibraryA")));
CreateNamedPipeAFunc createNamedPipeA = reinterpret_cast<CreateNamedPipeAFunc>(myGetProcAddress(h_KernelBase, VMProtectDecryptStringA("CreateNamedPipeA")));
CreateNamedPipeWFunc createNamedPipeW = reinterpret_cast<CreateNamedPipeWFunc>(myGetProcAddress(h_KernelBase, VMProtectDecryptStringA("CreateNamedPipeW")));
CreateThreadFunc createThread = reinterpret_cast<CreateThreadFunc>(myGetProcAddress(h_KernelBase, VMProtectDecryptStringA("CreateThread")));
AppendMenuAFunc appendMenuA = reinterpret_cast<AppendMenuAFunc>(myGetProcAddress(h_User32, VMProtectDecryptStringA("AppendMenuA")));
FindWindowAFunc findWindowA = reinterpret_cast<FindWindowAFunc>(myGetProcAddress(h_User32, VMProtectDecryptStringA("FindWindowA")));
FindWindowWFunc findWindowW = reinterpret_cast<FindWindowWFunc>(myGetProcAddress(h_User32, VMProtectDecryptStringA("FindWindowW")));
FindWindowExAFunc findWindowExA = reinterpret_cast<FindWindowExAFunc>(myGetProcAddress(h_User32, VMProtectDecryptStringA("FindWindowExA")));
FindWindowExWFunc findWindowExW = reinterpret_cast<FindWindowExWFunc>(myGetProcAddress(h_User32, VMProtectDecryptStringA("FindWindowExW")));
GetModuleHandleWFunc getModuleHandleW = reinterpret_cast<GetModuleHandleWFunc>(myGetProcAddress(h_KernelBase, VMProtectDecryptStringA("GetModuleHandleW")));
AllocConsoleFunc allocConsole = reinterpret_cast<AllocConsoleFunc>(myGetProcAddress(h_KernelBase, VMProtectDecryptStringA("AllocConsole")));

