#include <pch.h>
#include "ProcessChecker.h"

const char* blacklistedWindowNames[] = 
{
	VMProtectDecryptStringA("ollydbg"), 
	VMProtectDecryptStringA("x32dbg"),
	VMProtectDecryptStringA("x64dbg"), 
	VMProtectDecryptStringA("windbg"), 
	VMProtectDecryptStringA("scylla"), 
};

void CheckForBlacklistedProcess()
{
	// needs to run always
	while (true)
	{
		// go through list of blacklisted window names
		for (const char* windowName : blacklistedWindowNames)
		{
			// use findwindowa to check if the window is open
			if (FindWindowA(NULL, windowName))
			{
				// if the window is open, terminate the process
				TerminateProcess(GetCurrentProcess(), 0);
			}
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