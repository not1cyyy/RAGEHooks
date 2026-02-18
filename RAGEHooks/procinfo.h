#include <string>
#include "windows.h"
#pragma once
extern std::wstring GetProcessFilePath(DWORD processID);
extern bool ValidateDigitalSignature(const std::wstring& filePath);
extern void FillExceptionPointersWithRandomData(_EXCEPTION_POINTERS* exceptionPointers);
bool DoesFileExist(const std::string& path);