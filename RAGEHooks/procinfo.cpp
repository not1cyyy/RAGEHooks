#include <pch.h>
std::wstring GetProcessFilePath(DWORD processID)
{
    TCHAR szFilePath[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL)
    {
        if (GetModuleFileNameEx(hProcess, NULL, szFilePath, sizeof(szFilePath) / sizeof(TCHAR)) > 0)
        {
            CloseHandle(hProcess);
            return std::wstring(szFilePath);
        }
        CloseHandle(hProcess);
    }
    return std::wstring();
}

bool DoesFileExist(const std::string& path)
{
    DWORD fileAttribs = GetFileAttributesA(path.c_str());
    if (fileAttribs == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    return true;
}

