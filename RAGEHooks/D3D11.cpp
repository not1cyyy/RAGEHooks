#include <pch.h>
#include "sigscanner.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_stdlib.h"
#pragma comment(lib, "d3d11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

typedef HRESULT(WINAPI* IDXGISwapChainPresent)(IDXGISwapChain*, UINT, UINT);

typedef HRESULT(WINAPI* IDXGISwapChainResizeBuffers)(
    IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

bool IsInitialized = false;

WNDPROC OriginalWindowProcedure = NULL;
HWND    OutputWindow = NULL;

ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* pRenderTargetView = NULL;
IDXGISwapChain* pSwapChain = NULL;

DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};

IDXGISwapChainPresent       PresentFunc;
IDXGISwapChainResizeBuffers ResizeBuffersFunc;

LRESULT WINAPI NewWindowProcedure(const HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam) {

    if (ImGui_ImplWin32_WndProcHandler(_hWnd, _uMsg, _wParam, _lParam))
        return true;

    return CallWindowProc(OriginalWindowProcedure, _hWnd, _uMsg, _wParam, _lParam);
}

__forceinline uintptr_t FindDebugGraphicsVsync()
{
    uintptr_t flag;

    try
    {
        flag = (uintptr_t)SigScan("\x80\x3D\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x74\x00\xC7\x45", "xx?????xx?????x?xx");
    }
    catch (...)
    {
        flag = 0;
    }


    return *(uint32_t*)(flag + 2);
}

HRESULT WINAPI NewPresent(IDXGISwapChain* _pSwapChain, UINT _SyncInterval, UINT _Flags) 
{
    return PresentFunc(_pSwapChain, 0, _Flags);
}

HRESULT WINAPI NewResizeBuffers(IDXGISwapChain* _pSwapChain,
    UINT            _BufferCount,
    UINT            _Width,
    UINT            _Height,
    DXGI_FORMAT     _NewFormat,
    UINT            _SwapChainFlags) {

    if (pRenderTargetView) {
        pContext->OMSetRenderTargets(0, 0, 0);
        pRenderTargetView->Release();
    }

    HRESULT _Result =
        ResizeBuffersFunc(_pSwapChain, _BufferCount, _Width, _Height, _NewFormat, _SwapChainFlags);

    ID3D11Texture2D* _pBuffer;

    _pSwapChain->GetBuffer(0, IID_PPV_ARGS(&_pBuffer));

    if (pDevice->CreateRenderTargetView(_pBuffer, nullptr, &pRenderTargetView))
        return 0;

    _pBuffer->Release();

    pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

    D3D11_VIEWPORT _ViewPort;
    ZeroMemory(&_ViewPort, sizeof(_ViewPort));
    _ViewPort.Width = (float)_Width;
    _ViewPort.Height = (float)_Height;
    _ViewPort.MinDepth = 0.0f;
    _ViewPort.MaxDepth = 1.0f;
    _ViewPort.TopLeftX = 0;
    _ViewPort.TopLeftY = 0;

    pContext->RSSetViewports(1, &_ViewPort);

    return _Result;
}

void EnableHook() {
    if (GetModuleHandleA("dxgi.dll"))
    {
        uint32_t Flag = FindDebugGraphicsVsync();
        ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

        SwapChainDesc.BufferDesc.Width = NULL;
        SwapChainDesc.BufferDesc.Height = NULL;
        SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        SwapChainDesc.SampleDesc.Count = 1;
        SwapChainDesc.SampleDesc.Quality = 0;
        SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.BufferCount = 2;
        SwapChainDesc.OutputWindow = GetForegroundWindow();
        SwapChainDesc.Windowed = true;
        SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        SwapChainDesc.Flags = NULL;

        if (D3D11CreateDeviceAndSwapChain(NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            NULL,
            NULL,
            NULL,
            D3D11_SDK_VERSION,
            &SwapChainDesc,
            &pSwapChain,
            &pDevice,
            NULL,
            NULL))
            return;
        void** pp_SwapChainVTable = *reinterpret_cast<void***>(pSwapChain);

        PresentFunc = (IDXGISwapChainPresent)pp_SwapChainVTable[8];
        
        ResizeBuffersFunc = (IDXGISwapChainResizeBuffers)pp_SwapChainVTable[13];
		//wait till roblox is open
        while (!FindWindowA(nullptr, VMProtectDecryptStringA("Roblox"))) 
        {
			Sleep(100);
        }
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        //DetourAttach(&(PVOID&)ResizeBuffersFunc, (void*)NewResizeBuffers);
        DetourAttach(&(PVOID&)PresentFunc, (void*)NewPresent);
        DetourTransactionCommit();

        *(unsigned char*)Flag = 1;
    }
    else
    {
        //RAGEDebugger::Log(DebugLog, VMProtectDecryptStringA("Module not found"));
    }
}

void DisableHook() 
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)PresentFunc, (void*)NewPresent);
    DetourDetach(&(PVOID&)ResizeBuffersFunc, (void*)NewResizeBuffers);
    DetourTransactionCommit();

    pRenderTargetView->Release();
    pSwapChain->Release();
    pContext->Release();
    pDevice->Release();
}
