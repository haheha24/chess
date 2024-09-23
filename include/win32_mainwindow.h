#pragma once

#ifndef WINAPI

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define WIN_32_EXTRA_LEAN
#define UNICODE
#endif

#include <windows.h>
#include <memory>
#include "glad/glad_wgl.h"
#include "win32_node.h"

std::shared_ptr<WindowBase> win32_createWindow(PCWSTR lpWindowName,
                                               DWORD dwStyle,
                                               DWORD dwExStyle = 0,
                                               int x = CW_USEDEFAULT,
                                               int y = CW_USEDEFAULT,
                                               int nWidth = CW_USEDEFAULT,
                                               int nHeight = CW_USEDEFAULT,
                                               HWND hWndParent = nullptr,
                                               HMENU hMenu = nullptr);
void win32_updateWindow(WindowBasePtr& window);
void win32_closeWindow(WindowBase& window);
bool win32_windowShouldClose(bool shouldClose);
bool win32_makeContextCurrent(HWND& hwnd);
LRESULT _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);