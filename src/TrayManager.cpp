#include "TrayManager.h"
#include "Logger.h"
#include "DCPrevent.h"
#include <shellapi.h>

#define IDI_APPLICATION_W 32512  // Standard Windows application icon

void AddTrayIcon(HWND hwnd) {
    NOTIFYICONDATAW nid{};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = TRAY_UID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = LoadIconW(NULL, MAKEINTRESOURCEW(IDI_APPLICATION_W)); // fixed
    wcscpy_s(nid.szTip, sizeof(nid.szTip)/sizeof(WCHAR), L"DC Prevent (in tray)");
    Shell_NotifyIconW(NIM_ADD, &nid);
}

void RemoveTrayIcon(HWND hwnd) {
    NOTIFYICONDATAW nid{};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = TRAY_UID;
    Shell_NotifyIconW(NIM_DELETE, &nid);
}

void ShowMainWindow() {
    ShowWindow(g_hMainWnd, SW_SHOW);
    SetForegroundWindow(g_hMainWnd);
}

void HideToTray() {
    AddTrayIcon(g_hMainWnd);
    ShowWindow(g_hMainWnd, SW_HIDE);
}
