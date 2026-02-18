#pragma once
#include <pch.h>
#include "../sigscanner.h"

class Offsets
{
public:
	using PrintFunc = void(__cdecl*)(int type, const char* message, ...);
	using RegisterPlayFunc = void(__fastcall*)(DWORD* thisptr, DWORD*, const std::string& key, __int64 userId, __int64 placeId);
	using IsDebuggedFunc = bool(__cdecl*)();
	using LuaGetFieldFunc = int(__cdecl*)(DWORD L, int idx, const char* k);
	using LuaToLStringFunc = char* (__cdecl*)(DWORD L, int idx, size_t* len);
	using Index2AdrFunc = DWORD(__cdecl*)(DWORD L, int idx);
	using VerifySignatureBase64Func = void(__fastcall*)(void* thisPtr, void* unused, std::string message, std::string signatureBase64);
	using ShowErrorWindowFunc = void(__fastcall*)(void* thisPtr, void* unused, const std::string& message, const std::string& errorType, const std::string& errorCategory);
	using SetUiMessageFunc = void(__stdcall*)(int guiMessageType, std::string& message);
	using GetUiMessageFunc = std::string(__cdecl*)();
	using CanUseNotificationServiceFunc = bool(__fastcall*)(void* thisPtr);
	using CollectMd5HashFunc = std::string(__cdecl*)(const std::string& filename);

	static PrintFunc                     print;
	static RegisterPlayFunc              registerPlay;
	static IsDebuggedFunc                isDebugged;
	static LuaGetFieldFunc               luaGetField;
	static LuaToLStringFunc              luaToLString;
	static Index2AdrFunc                 index2Adr;
	static VerifySignatureBase64Func     verifySignatureBase64;
	static ShowErrorWindowFunc           showErrorWindow;
	static SetUiMessageFunc              setUiMessage;
	static GetUiMessageFunc              getUiMessage;
	static CanUseNotificationServiceFunc canUseNotificationService;
	static CollectMd5HashFunc            collectMd5Hash;

	static void* gameConfigurerClassPtr;
};

inline Offsets::PrintFunc inline Offsets::print = reinterpret_cast<PrintFunc>(
	(uintptr_t)SigScan("\x55\x8B\xEC\x6A\xFF\x68\x80\x3D\xF8\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x30\x8D\x45", "xxxxxxxxxxxx????xxxx????xxxxx"));
inline Offsets::RegisterPlayFunc inline Offsets::registerPlay = reinterpret_cast<RegisterPlayFunc>(
	(uintptr_t)SigScan("\x55\x8B\xEC\x6A\xFF\x68\x90\x9E\xFA\x00", "xxxxxxxxxx"));

inline Offsets::IsDebuggedFunc               Offsets::isDebugged = reinterpret_cast<IsDebuggedFunc>(0xBE4B50);
inline Offsets::LuaGetFieldFunc              Offsets::luaGetField = reinterpret_cast<LuaGetFieldFunc>(0x7545A0);
inline Offsets::LuaToLStringFunc             Offsets::luaToLString = reinterpret_cast<LuaToLStringFunc>(0x756940);
inline Offsets::Index2AdrFunc                Offsets::index2Adr = reinterpret_cast<Index2AdrFunc>(0x74C4E0);
inline Offsets::VerifySignatureBase64Func    Offsets::verifySignatureBase64 = reinterpret_cast<VerifySignatureBase64Func>(0xBEFEB0);
inline Offsets::ShowErrorWindowFunc          Offsets::showErrorWindow = reinterpret_cast<ShowErrorWindowFunc>(0xC206A0);
inline Offsets::SetUiMessageFunc             Offsets::setUiMessage = reinterpret_cast<SetUiMessageFunc>(0x4E83F0);
inline Offsets::GetUiMessageFunc             Offsets::getUiMessage = reinterpret_cast<GetUiMessageFunc>(0x5E8E50);
inline Offsets::CanUseNotificationServiceFunc Offsets::canUseNotificationService = reinterpret_cast<CanUseNotificationServiceFunc>(0x69E490);
inline Offsets::CollectMd5HashFunc           Offsets::collectMd5Hash = reinterpret_cast<CollectMd5HashFunc>(0x5CD3D0);

inline void* Offsets::gameConfigurerClassPtr = nullptr;