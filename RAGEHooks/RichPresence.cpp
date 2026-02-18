#include <pch.h>
#include "RichPresence.h"
#include "Utilities/Http/Http.h"
#include "Hooks/RobloxHooks.h"


bool isFurryfied = false;
std::string pid;
std::string uid;
DiscordRichPresence discordPresence;
const char* discordPresenceId = "1311387299635794011";
int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void UpdateLoadingscreen() {
    try {
        DiscordRichPresence discordPresence = {};
        discordPresence.state = "In loading screen";
        discordPresence.startTimestamp = eptime;
        discordPresence.largeImageKey = "pekora";
        discordPresence.largeImageText = "Pekora";
        discordPresence.button1Label = "Discord";
        discordPresence.button1Url = "https://discord.gg/pekora";
        Discord_UpdatePresence(&discordPresence);
    }
    catch (...) {
    }
}

void UpdateWithPlace() {
    try {
        memset(&discordPresence, 0, sizeof(discordPresence));
        std::string rpcResp = GetRichPresenceInfo(pid, uid, serverInfo.jobId);
		if (rpcResp.empty()) return;
        // TODO: refactor
        nlohmann::json jsonData = nlohmann::json::parse(rpcResp, nullptr, false);
        if (jsonData.is_discarded()) return;

		bool isFurry = jsonData.value("IsFurry", 0);
        int maxPlayers = jsonData.value("MaxPlayers", 0);
        int currentPlayers = jsonData.value("CurrentPlayers", 0);

        std::string partyId = jsonData.value("PartyId", "");
		std::string smallText = jsonData.value("Username", "") + " " + "(" + uid + ")";
        std::string stateText = "Playing " + std::to_string(jsonData.value("Year", 0));
        std::string detailsText = jsonData.value("Name", "");
        //links
        std::string userHeadshot = BaseUrl + "/Thumbs/Avatar-Headshot.ashx?userId=" + uid;

        std::string joinGame = BaseUrl + "/joinserver?jobId=" + serverInfo.jobId + "&placeId=" + pid;
        std::string gameIcon = BaseUrl + "Thumbs/GameIcon.ashx?assetId=" + pid;
        std::string gamePage = BaseUrl + "/games/" + pid + "/---";

        discordPresence.state = stateText.c_str();
        discordPresence.details = detailsText.c_str();
        discordPresence.startTimestamp = eptime;
        discordPresence.largeImageKey = gameIcon.c_str();
        discordPresence.largeImageText = "Pekora";
        discordPresence.smallImageKey = userHeadshot.c_str();
        discordPresence.smallImageText = smallText.c_str();
        discordPresence.button1Label = "Join current server";
        discordPresence.button1Url = joinGame.c_str();
        discordPresence.button2Label = "Open game page";
        discordPresence.button2Url = gamePage.c_str();
        discordPresence.partyId = partyId.c_str();
        discordPresence.partySize = currentPlayers;
        discordPresence.partyMax = maxPlayers;
        Discord_UpdatePresence(&discordPresence);
    }
    catch (...) {
    }
}

void StartRichPresenceForGame(int placeId, int userId) {
    try {
        pid = std::to_string(placeId);
        uid = std::to_string(userId);

        std::thread richPresenceThread([]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(15000));
                UpdateWithPlace();
            }
            });

        richPresenceThread.detach();
    }
    catch (...) {
    }
}

void InitializeRichPresence() {
    try {
        DiscordEventHandlers Handle = {};
        Discord_Initialize(discordPresenceId, &Handle, 1, NULL);
        UpdateLoadingscreen();
    }
    catch (...) {
    }
}
