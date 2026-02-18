#include <pch.h>
#include "Config.h"
#include "../../procinfo.h"
#include "../Utilities/Notifier/TaskDialog.h"
HANDLE configSetupEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
int SetupPekoraConfig()
{
	TASKDIALOGCONFIG config = { 0 };
	int stage1Button = 0;
	int stage2Button = 0;
	config.cbSize = sizeof(config);
	config.hwndParent = nullptr;
	config.hInstance = GetModuleHandle(nullptr);
	config.dwFlags = TDF_USE_COMMAND_LINKS;
	config.pszWindowTitle = L"Pekora First time setup";
	config.pszMainIcon = TD_INFORMATION_ICON;
	config.pszMainInstruction = L"Enable Rich Presence?";
	config.pszContent = L"Turning this on will let your friends see that you're playing Pekora on Discord";

	TASKDIALOG_BUTTON buttons[] =
	{
		{ 101, L"Yes" },
		{ 102, L"No" },
	};

	config.pButtons = buttons;
	config.cButtons = ARRAYSIZE(buttons);
	HRESULT stage1 = TaskDialogShow(&config, &stage1Button, NULL, NULL);
	if (SUCCEEDED(stage1))
	{
		switch (stage1Button)
		{
		case 101:
			RAGEConfig::Pekora::RichPresence = true;
			break;
		case 102:
			RAGEConfig::Pekora::RichPresence = false;
			break;
		}
	}
	else
	{
		RAGEConfig::Pekora::FpsUnlocker = false;
		RAGEConfig::Pekora::RichPresence = true;
		return 0;
	}

	config.pszMainInstruction = L"Enable FPS Unlocker";
	config.pszContent = L"Turning this on might break some games!";
	HRESULT stage2 = TaskDialogShow(&config, &stage2Button, NULL, NULL);
	if (SUCCEEDED(stage2))
	{
		switch (stage2Button)
		{
		case 101:
			RAGEConfig::Pekora::FpsUnlocker = true;
			break;
		case 102:
			RAGEConfig::Pekora::FpsUnlocker = false;
			break;
		}
	}
	SetEvent(configSetupEvent);
	return 0;
}

int LoadConfig()
{
	wchar_t currentFilePath[MAX_PATH];
	LPWSTR path = currentFilePath;

	GetModuleFileName(NULL, currentFilePath, MAX_PATH);
	//need to do this when launching via launcher otherwise file not found
	std::wstring filePath = currentFilePath;
	filePath = filePath.substr(0, filePath.find_last_of(L'\\')) + L"\\config.ini";
	ini::IniFile configFile;
	std::string narrowFilePath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(filePath);

	if (!DoesFileExist(narrowFilePath))
	{
		SetupPekoraConfig();
		configFile["Settings"]["FPSUnlocked"] = RAGEConfig::Pekora::FpsUnlocker;
		configFile["Settings"]["RichPresence"] = RAGEConfig::Pekora::RichPresence;
		configFile["Settings"]["FurryMode"] = RAGEConfig::Pekora::FurryMode;
		configFile.save(narrowFilePath);
	}

	configFile.load(narrowFilePath);
	RAGEConfig::Pekora::FpsUnlocker = configFile["Settings"]["FPSUnlocked"].as<bool>();
	RAGEConfig::Pekora::RichPresence = configFile["Settings"]["RichPresence"].as<bool>();
	RAGEConfig::Pekora::FurryMode = configFile["Settings"]["FurryMode"].as<bool>();
	return 0;
}