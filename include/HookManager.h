#pragma once
#include <windows.h>

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
bool InstallMouseHook();
void RemoveMouseHook();
