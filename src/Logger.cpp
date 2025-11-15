#include "DCPrevent.h"
#include "Logger.h"

void PostLog(const std::wstring& s) {
    if (!g_hMainWnd) return;
    size_t cb = (s.size() + 1) * sizeof(wchar_t);
    wchar_t* buf = (wchar_t*)::CoTaskMemAlloc(cb);
    if (!buf) return;
    wcscpy_s(buf, s.size() + 1, s.c_str());
    PostMessageW(g_hMainWnd, WM_APP_ADD_LOG, 0, (LPARAM)buf);
}

void AddLogToListBox(HWND hList, const wchar_t* text) {
    if (!hList || !text) return;
    (void)SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)text);
    int count = (int)SendMessageW(hList, LB_GETCOUNT, 0, 0);
    if (count > 0) SendMessageW(hList, LB_SETCURSEL, count - 1, 0);
}
