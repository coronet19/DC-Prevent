#pragma once
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool InitMainWindow(HINSTANCE hInstance);
