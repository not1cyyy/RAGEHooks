#pragma once
// MDE - ManualMap Detection Engine 
// by NtKernelMC a.k.a 0pc0d3R
// Date of creation: 07.04.19
// Version: 0.0.1.f BETA
#include <Windows.h>
#pragma comment(lib, "MDE.lib")
namespace MDE
{
	const WORD SCANTIME_DLLHEADERS = 1;
	const WORD SCANTIME_DLLTHREADS = 1;
	const WORD SCANTIME_CRTSTUB = 1;
	const WORD SCANTIME_IAT = 1;
	enum MDE_SCANTYPE
	{
		MMAP_DLL_HEADERS = 0x1,
		MMAP_DLL_THREAD = 0x2,
		MMAP_CRT_STUB = 0x3,
		MMAP_IMPORT_TABLE = 0x4
	};
	typedef struct
	{
		LPVOID base_address;
		MDE_SCANTYPE detectionType;
		DWORD AllocatedProtect;
		PVOID AllocatedBase;
		DWORD AllocatedSize;
	} MMAP_INFO, *PMMAP_INFO;
	typedef void(__cdecl *CallbackMDE)(PMMAP_INFO mmap);
	void __cdecl MemoryScanner(MDE_SCANTYPE scanType, WORD threads, CallbackMDE NotifyCallback);
	bool __cdecl IsScanningStillActive(MDE_SCANTYPE scanType);
}
