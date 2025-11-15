#pragma once
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <shellapi.h>
#include <chrono>
#include <string>
#include <mutex>
#include <sstream>
#include <atomic>

// Constants
constexpr long DEFAULT_COOLDOWN_MS = 50;

// Controls IDs
enum {
    IDC_COMBO_MODE = 1001,
    IDC_BTN_TRAY   = 1002,
    IDC_LOG_LIST   = 1003
};

// WM_APP messages
#define WM_APP_ADD_LOG (WM_APP + 1)
constexpr UINT TRAY_UID = 0xDEADBEEF;
constexpr UINT WM_TRAY = WM_APP + 2;

// Globals
extern HINSTANCE g_hInst;
extern HWND g_hMainWnd;
extern HHOOK g_hHook;
extern std::mutex g_timeMutex;
extern std::chrono::steady_clock::time_point g_lastLeftClick;
extern std::chrono::steady_clock::time_point g_lastRightClick;
extern long g_cooldownMs;
extern std::atomic<int> g_mode; // 0 = both, 1 = left, 2 = right
