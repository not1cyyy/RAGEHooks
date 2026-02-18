#include <pch.h>

extern bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
    for (; *szMask; ++szMask, ++pData, ++bMask)
        if (*szMask == 'x' && *pData != *bMask) return 0;
    return (*szMask) == NULL;
}

extern DWORD SigScan(const char* AoB, const char* Mask) {
    for (DWORD i = (DWORD)GetModuleHandle(0); i <= 0xF000000; ++i) {
        if (Compare((BYTE*)i, (BYTE*)AoB, Mask))
            return i;
    }
    return 0;
}