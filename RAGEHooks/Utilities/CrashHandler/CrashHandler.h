#pragma once
class CrashHandler 
{
	public:
		static LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo);
	private:
		static void RobloxCrashMessage();
		static BOOL HideWindowByName(LPCSTR windowName);
		static int ShowCrashDialog(const std::wstring& content, const std::wstring& instruction, const std::wstring& crashDumpLocation);
		static std::wstring GetStackTrace(EXCEPTION_POINTERS* pExceptionInfo);
		static std::wstring CreateMiniDump(EXCEPTION_POINTERS* pExceptionInfo);
};
inline bool didCrash;