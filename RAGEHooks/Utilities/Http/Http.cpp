#include <pch.h>
#include <cpr/cpr.h>
#include "Http.h"

cpr::Header headers = { 
	{ "User-Agent", "Roblox/WinInet" },
};
cpr::Cookies cookies = {};

void SetCookieAndHeaders(std::string cookie)
{
	cookies = {
		{ ".ROBLOSECURITY", cookie },
	};
}

cpr::Response Post(std::string uri, cpr::Body body)
{
	//RAGEDebugger::Log(DebugLog, uri.c_str());
	try
	{
		auto res = cpr::Post(cpr::Url{ BaseUrl + uri }, body, headers, cookies);
		return res;
	}
	catch (const std::exception& e)
	{
		//RAGEDebugger::Log(DebugLog, e.what());
	}
}

cpr::Response Get(std::string uri)
{
	//RAGEDebugger::Log(DebugLog, uri.c_str());
	try
	{
		auto res = cpr::Get(cpr::Url{ BaseUrl + uri }, headers, cookies);
		return res;
	}
	catch (const std::exception& e)
	{
		//RAGEDebugger::Log(DebugLog, e.what());
	}
}

// why use strings for place id and user id? because its easier to work with in this context, because its easier to concentrate strings
std::string GetRichPresenceInfo(std::string placeId, std::string userId, std::string jobId)
{
	try
	{
		cpr::Response res = Get("/getrichpresence?placeId=" + placeId + "&userId=" + userId + "&jobId=" + jobId);
		//RAGEDebugger::Log(DebugLog, VMProtectDecryptStringA(res.text.c_str()));
		return res.text;
	}
	catch (const std::exception& e)
	{
		//RAGEDebugger::Log(DebugLog, e.what());
	}
}

// get server info from ip
std::string GetServerInfo(std::string ip)
{
	try
	{
		cpr::Response res = Get("/getserverinfo?ip=" + ip);
		return res.text;
	}
	catch (const std::exception& e)
	{
		//RAGEDebugger::Log(DebugLog, e.what());
	}
}