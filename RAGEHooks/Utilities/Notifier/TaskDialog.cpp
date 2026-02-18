#include <pch.h>
#include "../../hookdefs.h"
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HRESULT TaskDialogShow(const TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked)
{
    HMODULE hComCtl = LoadLibrary(L"Comctl32.dll");
    if (!hComCtl) {
        return -1;
    }

    TaskDialogIndirectFunc taskDialogIndirect = reinterpret_cast<TaskDialogIndirectFunc>(GetProcAddress(hComCtl, VMProtectDecryptStringA("TaskDialogIndirect")));
    if (!taskDialogIndirect) {
        FreeLibrary(hComCtl);
        return -1;
    }

    HRESULT result = taskDialogIndirect(pTaskConfig, pnButton, nullptr, nullptr);
    FreeLibrary(hComCtl);
    return 0;
}