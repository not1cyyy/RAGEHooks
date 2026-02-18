#pragma once
struct ServerInfo
{
	bool isConnected;
	std::string jobId;
	std::string serverIp;
};

extern ServerInfo serverInfo;

void StartRobloxHooks();