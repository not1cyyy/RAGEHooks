#include <pch.h>
#include "Offsets.h"

namespace RAGESecurity
{
	std::atomic<bool> g_isSet = false;
	HMODULE g_robloxModule = GetModuleHandle(nullptr);
	MODULEINFO g_robloxModInfo;
	void* g_robloxBase;
	SIZE_T g_robloxSize;

	void CorruptMemory()
	{
		int mem = 0x000a1000 + (std::rand() % (0x100b2000 - 0x000a1000 + 1));
		int count = 0x00abcdef + (std::rand() % (0xfafdfa0f - 0x00abcdef + 1));
		int addition = 0x000000ff + (std::rand() % (0xff000000 - 0x000000ff + 1));

		for (int i = 0x00000000; i < count; i += addition)
		{
			unsigned char byte = std::rand() % 255;
			*(reinterpret_cast<char*>(mem + i)) = byte;
		}

		_asm
		{
			__emit 0xF3;
			__emit 0x64;
			__emit 0xCC;
		}
	}

	bool CheckIfOutsideModule(void* returnAddress)
	{
		VMProtectBegin("RETCheck");

		if (!g_isSet)
		{
			GetModuleInformation(GetCurrentProcess(), g_robloxModule, &g_robloxModInfo, sizeof(g_robloxModInfo));
			g_robloxBase = g_robloxModInfo.lpBaseOfDll;
			g_robloxSize = g_robloxModInfo.SizeOfImage;
			g_isSet = true;
		}

		bool outside = (returnAddress < g_robloxBase ||
			returnAddress >= static_cast<char*>(g_robloxBase) + g_robloxSize);

		VMProtectEnd();
		return outside;
	}

	bool isDebuggedHook()
	{
		return true;
	}

	void StartDetection()
	{
		SafetyHookInline isdebugged_hook = safetyhook::create_inline(
			Offsets::isDebugged,
			reinterpret_cast<void*>(isDebuggedHook)
		);

		std::this_thread::sleep_for(std::chrono::seconds(2));
		CorruptMemory();
	}
}