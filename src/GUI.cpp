#include "GUI.h"
#include "DCPrevent.h"
#include "TrayManager.h"
#include "HookManager.h"
#include "Logger.h"
#include <windows.h>

bool InitMainWindow(HINSTANCE hInstance) {
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DCPreventClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    return RegisterClassW(&wc) != 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_CREATE: {
        // ----- Create Monospace Font -----
        HFONT hMonoFont = CreateFontW(
            18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN,
            L"Consolas"
        );

        // ----- Combo Box -----
        HWND hCombo = CreateWindowW(L"COMBOBOX", NULL,
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
            10, 10, 200, 200,
            hwnd, (HMENU)IDC_COMBO_MODE, g_hInst, NULL);

        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Both");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Left only");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Right only");
        SendMessageW(hCombo, CB_SETCURSEL, 0, 0);

        // ----- Button -----
        CreateWindowW(L"BUTTON", L"Hide to Tray",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 8, 100, 24,
            hwnd, (HMENU)IDC_BTN_TRAY, g_hInst, NULL);

        // ----- Log ListBox -----
        HWND hList = CreateWindowW(
            L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_HASSTRINGS,
            10, 40, 460, 260,
            hwnd, (HMENU)IDC_LOG_LIST, g_hInst, NULL
        );

        // APPLY MONOSPACE FONT TO LISTBOX
        SendMessageW(hList, WM_SETFONT, (WPARAM)hMonoFont, TRUE);

    } return 0;

    case WM_COMMAND: {
        WORD id = LOWORD(wParam);
        WORD notify = HIWORD(wParam);

        if (id == IDC_COMBO_MODE && notify == CBN_SELCHANGE) {
            HWND hCombo = GetDlgItem(hwnd, IDC_COMBO_MODE);
            int sel = (int)SendMessageW(hCombo, CB_GETCURSEL, 0, 0);
            if (sel >= 0) g_mode.store(sel);
        }
        else if (id == IDC_BTN_TRAY) {
            HideToTray();
        }
    } return 0;

    case WM_APP_ADD_LOG: {
        wchar_t* buf = (wchar_t*)lParam;
        if (buf) {
            HWND hList = GetDlgItem(hwnd, IDC_LOG_LIST);
            AddLogToListBox(hList, buf);
            ::CoTaskMemFree(buf);
        }
    } return 0;

    case WM_TRAY:
        if (LOWORD(lParam) == WM_LBUTTONUP || LOWORD(lParam) == WM_RBUTTONUP) {
            RemoveTrayIcon(hwnd);
            ShowMainWindow();
        }
        return 0;

    case WM_CLOSE:
        RemoveTrayIcon(hwnd);
        if (g_hHook) RemoveMouseHook();
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
