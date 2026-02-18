#include <pch.h>
#include "../Utilities/Config/Config.h"
#include "../Utilities/Security/Security.h"
#include "../Utilities/Security/Memcheck.h"
#include "../Utilities/Notifier/Notifier.h"
#include "../RichPresence.h"
#include "RobloxHooks.h"
#include "Offsets.h"
#include "Lua.h"
#include "GameConfigurer.h"

bool g_printDone = false;
std::string g_joinMessage;

extern ServerInfo serverInfo = { false, "", "" };

SafetyHookInline collectMd5Hash_hook;
SafetyHookInline print_hook{};
SafetyHookInline verifySignatureBase64_hook{};

bool __cdecl IsDebuggedHook()
{
	return true;
}

void __cdecl PrintHook(int code, const char* format, ...)
{
	if (!g_printDone)
	{
		std::string msg(format);
		va_list args;
		va_start(args, format);

		if (msg.contains(VMProtectDecryptStringA("! Joining game")))
		{
			serverInfo.jobId = va_arg(args, const char*);
			va_arg(args, int);
			serverInfo.serverIp = va_arg(args, const char*);

			g_joinMessage = RAGEConfig::Pekora::FurryMode
				? "Joining sewvew OwO (" + serverInfo.jobId + ")"
				: "Attempting to join the Pekora server (" + serverInfo.jobId + ")";

			Offsets::setUiMessage(1, g_joinMessage);
			std::thread(OnConnectServerNotify).detach();
		}
		else if (msg.contains(VMProtectDecryptStringA("Connection accepted from")))
		{
			g_joinMessage = "Connected to the Pekora server";
			Offsets::setUiMessage(1, g_joinMessage);
			serverInfo.isConnected = true;
			g_printDone = true;
		}

		va_end(args);
		return;
	}

	if (RAGESecurity::CheckIfOutsideModule(_ReturnAddress()))
		RAGESecurity::StartDetection();
}

inline int Base64DecodeGetRequiredLength(_In_ int nSrcLen) throw()
{
	return nSrcLen;
}

void __fastcall verifySignatureBase64Hook(void* thisPtr, void* unused, std::string message, std::string signatureBase64)
{
	if (Base64DecodeGetRequiredLength(static_cast<int>(signatureBase64.length())) > 10240)
		throw std::runtime_error("[RAGE] Signature is too long");

	Offsets::verifySignatureBase64(thisPtr, unused, message, signatureBase64);
}

std::string __cdecl CollectMd5HashHook(const std::string& filename)
{
	collectMd5Hash_hook = {};
	return "0a5d9189b9f7a764ccf8b5655f442971";
}

void StartRobloxHooks()
{
	RAGEHooks::GameConfigurer::StartGameConfigurerHooks();
	RAGEHooks::Lua::StartLuaHooks();

	print_hook = safetyhook::create_inline(Offsets::print, reinterpret_cast<void*>(PrintHook));
	collectMd5Hash_hook = safetyhook::create_inline(Offsets::collectMd5Hash, reinterpret_cast<void*>(CollectMd5HashHook));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Offsets::verifySignatureBase64, verifySignatureBase64Hook);
	DetourAttach(&(PVOID&)Offsets::collectMd5Hash, CollectMd5HashHook);
	DetourTransactionCommit();
}