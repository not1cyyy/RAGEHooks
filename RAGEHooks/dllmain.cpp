#include <pch.h>
#include "Utilities/Config/Config.h"
#include "Utilities/CrashHandler/CrashHandler.h"
#include "Utilities/Security/Security.h"
#include "Utilities/Security/Memcheck.h"
#include "Winhook.h"
#include "Init.h"

/*
    Hello to the person digging through the source code, the code is a mess it is from 2024 when I just started with C++ 
    Please spare me be warned though the code is horrible.
    Some of the filters are also fucked so that's why ur unable to see any files
    List of things missing:
    RAGEDebugger.cpp/RAGEDebugger.h (consist of only a log function
    RAGELoader (lost, never uploaded to github)
*/

std::thread configSetupThread(LoadConfig);
__forceinline void VerifyIntegrity()
{
    if (!VMProtectIsValidImageCRC())
    {
        TerminateProcess(GetCurrentProcess(), 1);
    }
}

void StartRage()
{
    std::thread startRageThread(StartRAGE);
    startRageThread.detach();
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        VerifyIntegrity();
        AddVectoredExceptionHandler(0, CrashHandler::ExceptionFilter);
        configSetupThread.join();
        StartRage();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
