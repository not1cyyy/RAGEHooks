#include <pch.h>
#include "CrashHandler.h"
#include "../Utilities/Notifier/TaskDialog.h"

const std::vector<DWORD> criticalExceptions =
{
	EXCEPTION_ACCESS_VIOLATION,
	EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
	EXCEPTION_BREAKPOINT,
	EXCEPTION_DATATYPE_MISALIGNMENT,
	EXCEPTION_FLT_DENORMAL_OPERAND,
	EXCEPTION_FLT_DIVIDE_BY_ZERO,
	EXCEPTION_FLT_INEXACT_RESULT,
	EXCEPTION_FLT_INVALID_OPERATION,
	EXCEPTION_FLT_OVERFLOW,
	EXCEPTION_FLT_STACK_CHECK,
	EXCEPTION_FLT_UNDERFLOW,
	EXCEPTION_ILLEGAL_INSTRUCTION,
	EXCEPTION_IN_PAGE_ERROR,
	EXCEPTION_INT_DIVIDE_BY_ZERO,
	EXCEPTION_INT_OVERFLOW,
	EXCEPTION_INVALID_DISPOSITION,
	EXCEPTION_NONCONTINUABLE_EXCEPTION,
	EXCEPTION_PRIV_INSTRUCTION,
	EXCEPTION_STACK_OVERFLOW,
	EXCEPTION_GUARD_PAGE,
	EXCEPTION_INVALID_HANDLE,
};

BOOL CrashHandler::HideWindowByName(LPCSTR windowName)
{
	HWND hwnd = FindWindowA(NULL, windowName);
	if (hwnd == NULL)
		return FALSE;

	return PostMessageA(hwnd, WM_DESTROY, 0, 0);
}

void CrashHandler::RobloxCrashMessage()
{
	MessageBoxW(
		NULL,
		VMProtectDecryptStringW(L"An unexpected error occurred and Pekora needs to quit.  We're sorry!"),
		VMProtectDecryptStringW(L"Pekora Crash"),
		MB_OK
	);
}

int CrashHandler::ShowCrashDialog(const std::wstring& content, const std::wstring& instruction, const std::wstring& crashDumpLocation)
{
	if (didCrash)
		return 0;

	didCrash = true;

	TASKDIALOG_BUTTON buttons[] =
	{
		{ 101, L"Open discord server\nOpens the discord server in your default browser" },
		{ 102, L"Open crash dump location\nOpens explorer and highlights the crashdump" },
		{ 103, L"Continue" }
	};

	TASKDIALOGCONFIG config = { 0 };
	config.cbSize = sizeof(config);
	config.hwndParent = nullptr;
	config.hInstance = GetModuleHandle(nullptr);
	config.dwFlags = TDF_USE_COMMAND_LINKS;
	config.pszWindowTitle = L"RAGE";
	config.pszMainIcon = MAKEINTRESOURCEW(-7);
	config.pszMainInstruction = instruction.c_str();
	config.pszContent = content.c_str();
	config.pButtons = buttons;
	config.cButtons = ARRAYSIZE(buttons);

	int buttonPressed = 0;
	HRESULT hr = TaskDialogShow(&config, &buttonPressed, nullptr, nullptr);

	if (!SUCCEEDED(hr))
	{
		MessageBoxW(NULL, content.c_str(), L"RAGEHooks Crash", NULL);
		return 0;
	}

	switch (buttonPressed)
	{
	case 101:
		ShellExecuteW(0, 0, L"https://discord.gg/oldroblox", 0, 0, SW_SHOW);
		return 101;

	case 102:
	{
		std::wstringstream command;
		command << L"/c explorer.exe /select," << crashDumpLocation;
		ShellExecuteW(NULL, L"open", L"cmd.exe", command.str().c_str(), NULL, SW_HIDE);
		return 102;
	}

	case 103:
		return 103;

	default:
		return -1;
	}
}

std::wstring CrashHandler::GetStackTrace(EXCEPTION_POINTERS* pExceptionInfo)
{
	std::wstringstream stackTraceStream;
	CONTEXT* context = pExceptionInfo->ContextRecord;

	DWORD* stackPtr = (DWORD*)context->Esp;
	DWORD instructionPtr = context->Eip;

	stackTraceStream << L"Pointer: 0x" << std::hex << std::uppercase << instructionPtr << L"\n";

	for (int i = 0; i < 10; ++i)
		stackTraceStream << L"Frame" << i << L": 0x" << std::hex << std::uppercase << stackPtr[i] << L"\n";

	return stackTraceStream.str();
}

std::wstring CrashHandler::CreateMiniDump(EXCEPTION_POINTERS* pExceptionInfo)
{
	UUID uuid;
	UuidCreate(&uuid);

	RPC_WSTR uuidStr;
	UuidToStringW(&uuid, &uuidStr);

	std::wstring uuidString(reinterpret_cast<wchar_t*>(uuidStr));
	RpcStringFreeW(&uuidStr);

	std::wstringstream dumpFileName;
	dumpFileName << L"..\\..\\..\\logs\\" << uuidString << L".dmp";

	HANDLE dump = CreateFileW(
		dumpFileName.str().c_str(),
		GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr
	);

	if (dump == INVALID_HANDLE_VALUE)
		return L"";

	MINIDUMP_EXCEPTION_INFORMATION mei;
	mei.ThreadId = GetCurrentThreadId();
	mei.ExceptionPointers = pExceptionInfo;
	mei.ClientPointers = FALSE;

	bool writeSuccess = MiniDumpWriteDump(
		GetCurrentProcess(), GetCurrentProcessId(),
		dump, MiniDumpNormal, &mei, nullptr, nullptr
	);

	CloseHandle(dump);

	if (writeSuccess)
		return dumpFileName.str();

	return L"";
}

LONG WINAPI CrashHandler::ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
{
	DWORD exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;

	if (exceptionCode == EXCEPTION_BREAKPOINT)
	{
		TerminateProcess(GetCurrentProcess(), 1);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	if (std::find(criticalExceptions.begin(), criticalExceptions.end(), exceptionCode) == criticalExceptions.end())
		return EXCEPTION_CONTINUE_SEARCH;

	std::wstringstream instruction;
	instruction << L"RAGEHooks has unfortunately crashed (0x" << std::hex << exceptionCode << L")";

	std::wstring stacktrace = GetStackTrace(pExceptionInfo);

	std::wstringstream debugInfo;
	debugInfo << L"Address: 0x" << pExceptionInfo->ExceptionRecord->ExceptionAddress << L"\n";
	debugInfo << L"Stacktrace:\n" << stacktrace << L"\n";
	debugInfo << L"Please create a ticket in the Pekora discord server";

	std::wstring dumpPath = CreateMiniDump(pExceptionInfo);

	std::thread msgBoxThread(ShowCrashDialog, debugInfo.str(), instruction.str(), dumpPath);
	msgBoxThread.detach();

	HideWindowByName("Roblox");

	return EXCEPTION_EXECUTE_HANDLER;
}