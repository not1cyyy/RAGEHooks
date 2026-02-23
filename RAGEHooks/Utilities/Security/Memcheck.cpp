#include <pch.h>
#include "Memcheck.h"
#include "Winhook.h"
bool didBackUpByte = false;
//for module listing
std::map<std::string, int> AllowedModules;
int moduleCount = 0;
HMODULE hMods[1024];
HANDLE hProcess = GetCurrentProcess();
DWORD cbNeeded;
HMODULE robloxModule = GetModuleHandle(nullptr);
MODULEINFO robloxModInfo;
void* robloxBase;
SIZE_T robloxSize;
bool isSet = false;

int GetModules()
{
    VMProtectBeginUltra("GetModules");
    //RAGEDebugger::Log(DebugLog, "Module list:");
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            CHAR moduleName[MAX_PATH];
            if (GetModuleFileNameA(hMods[i], moduleName, (sizeof(moduleName) / sizeof(CHAR)))) {
                CHAR* filename = PathFindFileNameA(moduleName);
                //RAGEDebugger::Log(DebugLog, "Module %s (0x%08X)", filename, hMods[i]);
            }
        }
    }
    VMProtectEnd();
    return 0;
}

void GetWhitelistedModules()
{
    //RAGEDebugger::Log(DebugLog, "Whitelisted modules:");
    for (auto it = AllowedModules.cbegin(); it != AllowedModules.cend(); it++) {
        //RAGEDebugger::Log(DebugLog, "%d: %s", it->second, it->first.c_str());
    }
}

bool CheckForHooks(void* function)
{
	BYTE buffer[16];
	ReadProcessMemory(hProcess, function, buffer, sizeof(buffer), nullptr);
    // These are opcodes for assembly
    // JMP = 0xE9 | 0xEB
	if (buffer[0] == 0xE9 || buffer[0] == 0xEB)
    {
		return true;
	}
	return false;
}
