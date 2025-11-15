#include "DCPrevent.h"
#include "Logger.h"
#include "HookManager.h"
#include "TrayManager.h"
#include "GUI.h"
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <sstream>


HINSTANCE g_hInst = NULL;
HWND g_hMainWnd = NULL;
HHOOK g_hHook = NULL;
std::mutex g_timeMutex;
std::chrono::steady_clock::time_point g_lastLeftClick  = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point g_lastRightClick = std::chrono::steady_clock::now();
long g_cooldownMs = DEFAULT_COOLDOWN_MS;
std::atomic<int> g_mode{0};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    g_hInst = hInstance;

    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv) {
        if (argc >= 2) {
            long v = _wtol(argv[1]);
            if (v > 0) g_cooldownMs = v;
        }
        LocalFree(argv);
    }

    if (!InitMainWindow(hInstance)) {
        MessageBoxW(NULL, L"Failed to register window class.", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    g_hMainWnd = CreateWindowExW(0, L"DCPreventClass", L"vibecoded asf dc prevent coronet19",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 350, NULL, NULL, hInstance, NULL);

    if (!g_hMainWnd) {
        MessageBoxW(NULL, L"Failed to create main window.", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(g_hMainWnd, SW_SHOW);
    UpdateWindow(g_hMainWnd);

    {
        std::wstringstream ss;
        ss << L"Cooldown set to " << g_cooldownMs << L" ms";
        PostLog(ss.str());
    }

    if (!InstallMouseHook()) {
        MessageBoxW(NULL, L"Failed to install mouse hook. Run as Administrator.", L"Warning", MB_OK | MB_ICONWARNING);
    }

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    RemoveMouseHook();
    return (int)msg.wParam;
}
