#include <pch.h>

#include "Utilities/Security/Memcheck.h"
#include "Utilities/Http/Http.h"
#include "Utilities/String/Format.h"
#include "Utilities/Config/Config.h"
#include "Utilities/Notifier/Notifier.h"
#include "Hooks/RobloxHooks.h"
#include "hookdefs.h"
#include "D3D11.h"
#include "RichPresence.h"

#include "Winhook.h"

void GetAndSetCookie()
{
    std::string cookie = "";
    int nArgs;
	LPWSTR* szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

    for (int i = 0; i < nArgs; i++)
    {
		// If we found -t in the arguments then we know the next argument is the cookie
		if (wcsstr(szArglist[i], VMProtectDecryptStringW(L"-t")) != NULL)
        {
			// + 1 to skip the -t
            cookie = std::string(w2s(szArglist[i + 1]));
			// If the cookie doesnt start with ey then we know its invalid, because we use JWT tokens
            if (!cookie.starts_with(VMProtectDecryptStringA("ey")))
            {
				// Kill the process because we dont have a valid cookie
				TerminateProcess(GetCurrentProcess(), 0);
            }
            // We have the cookie now lets break out of the loop
            break;
		}
    }
    // Clean up
	LocalFree(szArglist);
	if (cookie.empty())
	{
		//RAGEDebugger::Log(DebugLog, VMProtectDecryptStringA("Failed to get cookie"));
	}
    else
    {
		//RAGEDebugger::Log(DebugLog, VMProtectDecryptStringA("Got cookie: %s"), cookie.c_str());
        SetCookieAndHeaders(cookie);
    }
}

void StartRAGE()
{
    VMProtectBegin("Init rage");
	// uncomment this if you want to see the console
    //RAGEDebugger::InitializeRageDebugger();
	std::thread initNotifierThread(IntializeNotifier);
	initNotifierThread.join();

    //RAGEDebugger::Log(DebugLog, VMProtectDecryptStringA("Not running on wine "));
    if (RAGEConfig::Pekora::FpsUnlocker)
    {
        EnableHook();
    }

    if (RAGEConfig::Pekora::RichPresence)
    {
        InitializeRichPresence();
        //RAGEDebugger::Log(DebugLog, VMProtectDecryptStringA("Initialized Rich Presence"));
    }

    StartHooks();
    GetAndSetCookie();
    VMProtectEnd();
}
