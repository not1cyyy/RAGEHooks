#include <pch.h>
#include "../Utilities/Notifier/Notifier.h"
#include "../Utilities/Config/Config.h"
#include "../Utilities/Security/Security.h"
#include "RobloxHooks.h"
#include "Offsets.h"

SafetyHookInline lua_getfield_hook{};
SafetyHookInline lua_tolstring_hook{};
SafetyHookInline lua_index2adr_hook{};

namespace RAGEHooks
{
namespace Lua
{
	int lua_getfieldHook(DWORD L, int idx, const char* k)
	{
		if (RAGESecurity::CheckIfOutsideModule(_ReturnAddress()))
		{
			RAGESecurity::StartDetection();
			return -4;
		}

		if (L == 0 || k == nullptr || idx == 0)
			return -1;

		return lua_getfield_hook.call<int>(L, idx, k);
	}

	char* lua_tolstringHook(DWORD L, int idx, size_t* len)
	{
		if (RAGESecurity::CheckIfOutsideModule(_ReturnAddress()))
			RAGESecurity::StartDetection();

		return lua_tolstring_hook.call<char*>(L, idx, len);
	}

	DWORD index2adrHook(DWORD L, int idx)
	{
		if (RAGESecurity::CheckIfOutsideModule(_ReturnAddress()))
		{
			RAGESecurity::StartDetection();
			return 0;
		}

		return lua_index2adr_hook.call<DWORD>(L, idx);
	}

	void StartLuaHooks()
	{
		lua_getfield_hook = safetyhook::create_inline(Offsets::luaGetField, reinterpret_cast<void*>(lua_getfieldHook));
		lua_tolstring_hook = safetyhook::create_inline(Offsets::luaToLString, reinterpret_cast<void*>(lua_tolstringHook));
		lua_index2adr_hook = safetyhook::create_inline(Offsets::index2Adr, reinterpret_cast<void*>(index2adrHook));
	}
}
}