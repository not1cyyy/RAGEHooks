//typedef NTSTATUS(WINAPI* LdrLoadDllFunc)(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle);
//extern LdrLoadDllFunc ldrLoadDll;z

typedef HRESULT (WINAPI* TaskDialogIndirectFunc)(const TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked);

typedef const char* (*wine_get_versionFunc)();
extern wine_get_versionFunc wine_get_version;

typedef NTSTATUS(NTAPI* NtWriteVirtualMemoryFunc)(HANDLE, PVOID, PVOID, ULONG, PULONG);
extern NtWriteVirtualMemoryFunc ntWriteVirtualMemory;

typedef HMODULE(WINAPI* LoadLibraryWFunc)(LPCWSTR);
extern LoadLibraryWFunc loadLibraryW;

typedef HMODULE(WINAPI* GetModuleHandleWFunc)(LPCWSTR);
extern GetModuleHandleWFunc getModuleHandleW;

typedef HMODULE(WINAPI* LoadLibraryAFunc)(LPCSTR);
extern LoadLibraryAFunc loadLibraryA;

typedef HANDLE(WINAPI* CreateNamedPipeAFunc)(LPCSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES);
extern CreateNamedPipeAFunc createNamedPipeA;

typedef HANDLE(WINAPI* CreateNamedPipeWFunc)(LPCWSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES);
extern CreateNamedPipeWFunc createNamedPipeW;

typedef HANDLE(WINAPI* CreateThreadFunc)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
extern CreateThreadFunc createThread;

typedef BOOL(WINAPI* AppendMenuAFunc)(HMENU, UINT, UINT_PTR, LPCSTR);
extern AppendMenuAFunc appendMenuA;

typedef HWND(WINAPI* FindWindowAFunc)(LPCSTR, LPCSTR);
extern FindWindowAFunc findWindowA;

typedef HWND(WINAPI* FindWindowWFunc)(LPCWSTR, LPCWSTR);
extern FindWindowWFunc findWindowW;

typedef HWND(WINAPI* FindWindowExAFunc)(HWND, HWND, LPCSTR, LPCSTR);
extern FindWindowExAFunc findWindowExA;

typedef HWND(WINAPI* FindWindowExWFunc)(HWND, HWND, LPCWSTR, LPCWSTR);
extern FindWindowExWFunc findWindowExW;

typedef BOOL(WINAPI* AllocConsoleFunc)();
extern AllocConsoleFunc allocConsole;
