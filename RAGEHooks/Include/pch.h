// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <httplib.h>
#include <windows.h> 
#include <regex>
#include <winternl.h>       
#include <wintoastlib.h>
#include <detours.h>   
#include <thread>           
#include <tchar.h>    
#include <vector>
#include <Commctrl.h>
#include <shlwapi.h>
#include <sstream>
#include <codecvt>
#include <dbghelp.h>
#include <errhandlingapi.h>
#include <inicpp.h>
#include <iostream>
#include <d2d1.h>
#include <dxgi.h>
#include <dwrite.h>
#include <string>          
#include "../Utilities/WinTrust/verify.h"     
#include <safetyhook.hpp>
#include <json.hpp>
#include <VMProtectSDK.h>
#include <Psapi.h>
#include <wininet.h>
#include <discord_rpc.h>
#include <discord_register.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <d3d11.h>
#endif //PCH_H
