#include <pch.h>
#include "ProcessChecker.h"
#include <TlHelp32.h>
#include <algorithm>

const char* blacklistedProcessNames[] = 
{
	VMProtectDecryptStringA("ollydbg"), 
	VMProtectDecryptStringA("x32dbg"),
	VMProtectDecryptStringA("x64dbg"), 
	VMProtectDecryptStringA("windbg"), 
	VMProtectDecryptStringA("scylla"), 
	VMProtectDecryptStringA("cheatengine"),
};

void CheckForBlacklistedProcess()
{
	// needs to run always
	while (true)
	{
		if (IsDebuggerPresent())
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}

		BOOL isDebuggerPresent = FALSE;
		CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent);
		if (isDebuggerPresent)
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32W pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32W);

			if (Process32FirstW(hSnapshot, &pe32))
			{
				do
				{
					std::wstring processNameW = pe32.szExeFile;
					std::string processName(processNameW.begin(), processNameW.end());
					std::transform(processName.begin(), processName.end(), processName.begin(), ::tolower);

					for (const char* blacklistedName : blacklistedProcessNames)
					{
						if (processName.find(blacklistedName) != std::string::npos)
						{
							TerminateProcess(GetCurrentProcess(), 0);
						}
					}
				} while (Process32NextW(hSnapshot, &pe32));
			}
			CloseHandle(hSnapshot);
		}

		// sleep for 1 second
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void StartProcessCheckerThread()
{
	std::thread processCheckerThread(CheckForBlacklistedProcess);
	processCheckerThread.detach();
}