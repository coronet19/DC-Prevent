#pragma once
#include <string>
#include <windows.h>

void PostLog(const std::wstring& s);
void AddLogToListBox(HWND hList, const wchar_t* text);
