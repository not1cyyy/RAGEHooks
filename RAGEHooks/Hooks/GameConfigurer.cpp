#include <pch.h>
#include "../Utilities/Config/Config.h"
#include "Offsets.h"
#include "../RichPresence.h"

namespace RAGEHooks
{
	namespace GameConfigurer
	{
		SafetyHookInline g_registerPlay_hook{};

		void __stdcall SetUiMessageHook(int guiMessageType, std::string& message)
		{
			if (RAGEConfig::Pekora::FurryMode)
			{
				if (message.contains("Requesting Server"))
					message = "Wequesting sewvew... :3";
				else if (message.contains("Server found"))
					message = "Sewvew found, woading... -w-";
				else if (message.contains("Requested game is full"))
					message = "Wequested game is fuww, wetwying... >_<";
			}
			else
			{
				if (message.contains("Requesting Server"))
					message = "Requesting Pekora server";
			}

			Offsets::setUiMessage(guiMessageType, message);
		}

		void __fastcall registerPlayHook(void* thisptr, void* unused, const std::string& key, int userId, int placeId, __int64 placeId2)
		{
			if (placeId == 0 && placeId2 != NULL)
				placeId = static_cast<int>(placeId2);

			Offsets::gameConfigurerClassPtr = thisptr;

			if (RAGEConfig::Pekora::RichPresence)
				StartRichPresenceForGame(placeId, userId);

			g_registerPlay_hook = {};
		}

		void StartGameConfigurerHooks()
		{
			g_registerPlay_hook = safetyhook::create_inline(
				Offsets::registerPlay,
				reinterpret_cast<void*>(registerPlayHook)
			);

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)Offsets::setUiMessage, SetUiMessageHook);
			DetourTransactionCommit();
		}
	}
}