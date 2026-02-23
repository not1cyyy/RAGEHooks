#include <pch.h>
#include "Utilities/Security/Memcheck.h"
#include "Utilities/Config/Config.h"
#include "Hooks/RobloxHooks.h"
#include "Utilities/Security/ProcessChecker.h"
#include "RichPresence.h"
#include "hookdefs.h"
#include "Winhook.h"

int countModule = 0;
int furryIds[] = 
{ 
    1, 
    3, 
    16, 
    47,
    68, 
    866 
};

SafetyHookInline g_LoadLibraryA_hook{};
SafetyHookInline g_LoadLibraryW_hook{};
SafetyHookInline g_CreateNamedPipeW_hook{};
SafetyHookInline g_CreateNamedPipeA_hook{};
SafetyHookInline g_AppendMenuA_hook{};
SafetyHookInline g_FindWindowExA_hook{};
SafetyHookInline g_FindWindowExW_hook{};
SafetyHookInline g_GetModuleHandleW_hook{};
SafetyHookInline g_AllocConsole_hook{};

// really fucking hacky way of triggering the diva flag is by hooking findwindowa
HWND WINAPI FindWindowAHook(LPCSTR lpClassName, LPCSTR lpWindowName)
{
    return findWindowA(lpClassName, lpWindowName);
}

void DivaHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)findWindowA, FindWindowAHook);
    DetourTransactionCommit();
}

void TriggerSysStats()
{
    std::thread hookThread(DivaHook);
    hookThread.detach();
}

__forceinline void StartDetection()
{
    while (!serverInfo.isConnected)
    {
        //just wait every 3 secs to prevent exhaustion
        Sleep(3000);
    }   
    TriggerSysStats();
    Sleep(8000);
    TerminateProcess(GetCurrentProcess(), 1);
}
////////////////////////////////////////////////////////////////////////////////////////

//hooks
HMODULE WINAPI LoadLibraryAHook(LPCSTR lpLibFileName)
{
    CHAR fullPathASCII[MAX_PATH] = { 0 };
    WCHAR fullPathW[MAX_PATH] = { 0 };

    DWORD result = SearchPathA(NULL, lpLibFileName, NULL, MAX_PATH, fullPathASCII, NULL);
    if (result == 0) {
        memset(fullPathASCII, 0, MAX_PATH);
        strcpy(fullPathASCII, lpLibFileName);
    }

    MultiByteToWideChar(CP_ACP, 0, fullPathASCII, -1, fullPathW, MAX_PATH);

    std::wstring fullPathStr(fullPathW);

    std::ranges::transform(fullPathStr, fullPathStr.begin(), [](wchar_t c) { return std::tolower(c); });

    bool isVerified = VerifyEmbeddedSignature(fullPathStr.c_str());

    if (!isVerified) 
    {
        if (fullPathStr.contains(VMProtectDecryptStringW(L"system32")) || fullPathStr.contains(VMProtectDecryptStringW(L"syswow64")))
            return g_LoadLibraryA_hook.stdcall<HMODULE>(lpLibFileName);
        return nullptr;
    }

    return g_LoadLibraryA_hook.stdcall<HMODULE>(lpLibFileName);
}

HMODULE WINAPI LoadLibraryWHook(LPCWSTR lpLibFileName)
{
    WCHAR fullPath[MAX_PATH] = { 0 };
    DWORD result = SearchPathW(NULL, lpLibFileName, NULL, MAX_PATH, fullPath, NULL);
    std::wstring fullPathStr(fullPath);
    if (result == 0)
        wcscpy(fullPath, lpLibFileName);
    

    std::ranges::transform(fullPathStr, fullPathStr.begin(), [](wchar_t c) { return std::tolower(c); });

    bool isVerified = VerifyEmbeddedSignature(fullPathStr.c_str());

    if (!isVerified) 
    {
        if (fullPathStr.contains(VMProtectDecryptStringW(L"system32")) || fullPathStr.contains(VMProtectDecryptStringW(L"syswow64"))) 
            return g_LoadLibraryW_hook.stdcall<HMODULE>(lpLibFileName);
        
        return nullptr;
    }
    //AllowedModules.insert({ str.c_str(), countModule });
    //countModule++;
    return g_LoadLibraryW_hook.stdcall<HMODULE>(lpLibFileName);
}

BOOL WINAPI FindWindowHook()
{
    return NULL;
}

BOOL WINAPI GenericHook()
{
	StartDetection();
	return NULL;
}
//we whitelist the discord ipc pipe otherwise it will false flag the user
//discord makes a pioe called discordhook and discord overlay so we will whitelist that
HANDLE WINAPI CreateNamedPipeAHook(LPCSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    std::string s(lpName);
    if (s.contains(VMProtectDecryptStringA("DiscordHook")) || s.contains(VMProtectDecryptStringA("DiscordOverlay")))
    {
        return g_CreateNamedPipeW_hook.stdcall<HANDLE>(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
    }
    StartDetection();
    return NULL;
}

HANDLE WINAPI CreateNamedPipeWHook(LPCWSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    std::wstring ws(lpName);
	if (ws.contains(VMProtectDecryptStringW(L"DiscordHook")) || ws.contains(VMProtectDecryptStringW(L"DiscordOverlay")))
	{
		return g_CreateNamedPipeW_hook.stdcall<HANDLE>(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
	}
    StartDetection();
    return NULL;
}

void doSafetyHook()
{
    VMProtectBegin("Hooks");
    StartRobloxHooks();
    //StartProcessCheckerThread();
    g_CreateNamedPipeW_hook = safetyhook::create_inline(createNamedPipeW, reinterpret_cast<void*>(&CreateNamedPipeWHook));
    g_CreateNamedPipeA_hook = safetyhook::create_inline(createNamedPipeA, reinterpret_cast<void*>(&CreateNamedPipeAHook));
    g_AppendMenuA_hook = safetyhook::create_inline(appendMenuA, reinterpret_cast<void*>(&GenericHook));
    g_FindWindowExA_hook = safetyhook::create_inline(findWindowExA, reinterpret_cast<void*>(&FindWindowHook));
    g_FindWindowExW_hook = safetyhook::create_inline(findWindowExW, reinterpret_cast<void*>(&FindWindowHook));
    g_AllocConsole_hook = safetyhook::create_inline(allocConsole, reinterpret_cast<void*>(&GenericHook));
    g_LoadLibraryA_hook = safetyhook::create_inline(loadLibraryA, reinterpret_cast<void*>(&LoadLibraryAHook));
    g_LoadLibraryW_hook = safetyhook::create_inline(loadLibraryW, reinterpret_cast<void*>(&LoadLibraryWHook));
  
    VMProtectEnd();
}
void StartHooks()
{
    std::thread doHookThread(doSafetyHook);
    doHookThread.detach();
}