#include <pch.h>
#include "../Http/Http.h"
#include "Notifier.h"
#include "../String/Format.h"
#include "../../Hooks/RobloxHooks.h"
#include "../Config/Config.h"
using namespace nlohmann;
using namespace WinToastLib;

WinToast::WinToastError error;
class CustomHandler : public IWinToastHandler {
public:
    void toastActivated() const override {}

    void toastActivated(int actionIndex) const override {}

    void toastDismissed(WinToastDismissalReason state) const
    {
        switch (state) {
        case UserCanceled:
            break;
        case TimedOut:
            break;
        case ApplicationHidden:
            break;
        default:
            break;
        }
    }

    void toastFailed() const override {}
};

void IntializeNotifier()
{
    WinToast::instance()->setAppName(L"RAGEHooks");
    WinToast::instance()->setAppUserModelId(L"RAGEHooks");
    WinToast::instance()->setShortcutPolicy(WinToast::SHORTCUT_POLICY_IGNORE);
    WinToast::instance()->initialize();
}

__forceinline void ShowNotification(const std::wstring& title, const std::wstring& message, const std::wstring& footer)
{
	WinToastTemplate templ = WinToastTemplate(WinToastTemplate::Text02);
	templ.setTextField(title, WinToastTemplate::FirstLine);
	templ.setTextField(message, WinToastTemplate::SecondLine);
    templ.setAttributionText(footer);
    templ.setExpiration(false);
    WinToast::instance()->showToast(templ, new CustomHandler());
}

void OnConnectServerNotify()
{
    CustomHandler* handler = new CustomHandler;
    // hardcoded for now :(
    // Not anymore!
    //std::string serverIp(VMProtectDecryptStringA("45.137.70.23"));
    std::wstring serverCity;
    std::wstring serverCountry;
    std::wstring serverCountryCode;
    //wait till we are connected to the gameserver
    while (!serverInfo.isConnected) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::string serverInfoRsp = GetServerInfo(serverInfo.serverIp);

	if (serverInfoRsp.empty())
    {
		//RAGEDebugger::Log(DebugLog, "Failed to get server info");
		return;
	}
	//RAGEDebugger::Log(DebugLog, "Server Info: %s", serverInfoRsp.c_str());
	json serverInfo = json::parse(serverInfoRsp);
    try
    {
        serverCity = s2w(serverInfo["city"].get<std::string>());
        serverCountry = s2w(serverInfo["country"].get<std::string>());
        serverCountryCode = s2w(serverInfo["countryCode"].get<std::string>());
    }
	catch (const std::exception& e)
	{
		//RAGEDebugger::Log(DebugLog, e.what());
        return;
	}

	if (RAGEConfig::Pekora::FurryMode)
    {
        ShowNotification(
			L"Connyected to the *boops your nose* Pekowa Gamesewvew", 
			L"Gamesewvew location: " + serverCountry + L", " + serverCity + L", " + serverCountryCode,
			L"Y-Y-Youw IP is pwotected"
		);
        return;
	}

    ShowNotification(
        L"Connected to the Pekora Gameserver", 
        L"Gameserver location: " + serverCountry + L", " + serverCity + L", " + serverCountryCode,
        L"Your IP is protected"
    );
}
