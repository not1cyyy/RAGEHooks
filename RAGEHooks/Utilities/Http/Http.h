#pragma once
// base url obv
const std::string BaseUrl(VMProtectDecryptStringA("https://www.pekora.zip"));

// headers for the http requests
void SetCookieAndHeaders(std::string cookie);
std::string GetRichPresenceInfo(std::string placeId, std::string userId, std::string jobId);
std::string GetServerInfo(std::string ip);