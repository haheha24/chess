#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <iostream>
#include "glad/glad_wgl.h"

HWND* win32_createWindow(PCWSTR lpWindowName,
                         DWORD dwStyle,
                         DWORD dwExStyle = 0,
                         int x = CW_USEDEFAULT,
                         int y = CW_USEDEFAULT,
                         int nWidth = CW_USEDEFAULT,
                         int nHeight = CW_USEDEFAULT,
                         HWND hWndParent = nullptr,
                         HMENU hMenu = nullptr);
bool win32_windowShouldClose();
void win32_closeWindow(HWND* window);
bool win32_makeContextCurrent(HWND* hwnd);
LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool windowShouldClose = false;