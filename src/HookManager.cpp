#include "DCPrevent.h"
#include "Logger.h"
#include "HookManager.h"
#include <chrono>
#include <sstream>
#include <mutex>

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN) {
            bool isLeft = (wParam == WM_LBUTTONDOWN);
            int mode = g_mode.load();
            bool relevant = (mode == 0) || (mode == 1 && isLeft) || (mode == 2 && !isLeft);

            auto now = std::chrono::steady_clock::now();
            bool shouldBlock = false;

            if (relevant) {
                std::lock_guard<std::mutex> lk(g_timeMutex);
                if (isLeft) {
                    long ms = (long)std::chrono::duration_cast<std::chrono::milliseconds>(now - g_lastLeftClick).count();
                    g_lastLeftClick = now;
                    shouldBlock = (ms < g_cooldownMs);
                } else {
                    long ms = (long)std::chrono::duration_cast<std::chrono::milliseconds>(now - g_lastRightClick).count();
                    g_lastRightClick = now;
                    shouldBlock = (ms < g_cooldownMs);
                }
            }

            std::wstringstream ss;
            ss << (shouldBlock ? L"Suppressed a DC\t" : L"MouseDown\t") 
               << (isLeft ? L"Left" : L"Right");
            PostLog(ss.str());

            if (shouldBlock) return 1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool InstallMouseHook() {
    g_hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    if (!g_hHook) return false;
    PostLog(L"Mouse hook installed.");
    return true;
}

void RemoveMouseHook() {
    if (g_hHook) {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
}
