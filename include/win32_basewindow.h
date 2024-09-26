#pragma once

#ifndef WINAPI

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define WIN_32_EXTRA_LEAN
#define UNICODE
#endif

#include <windows.h>
#include <iostream>
#include "glad/glad_wgl.h"

template <class DERIVED_TYPE>
class BaseWindow {
   public:
    static LRESULT _WindowProc(HWND m_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        DERIVED_TYPE* pThis = nullptr;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<DERIVED_TYPE*>(pCreate->lpCreateParams);
            SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->hwnd = m_hwnd;
        } else {
            pThis = reinterpret_cast<DERIVED_TYPE*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
        }
        if (pThis) {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        } else {
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : hwnd(NULL) {}

    bool createWindow(PCWSTR lpWindowName,
                      DWORD dwStyle,
                      DWORD dwExStyle = 0,
                      int x = CW_USEDEFAULT,
                      int y = CW_USEDEFAULT,
                      int nWidth = CW_USEDEFAULT,
                      int nHeight = CW_USEDEFAULT,
                      HWND hWndParent = nullptr,
                      HMENU hMenu = nullptr) {
        WNDCLASSEX wc = {0};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.lpfnWndProc = _WindowProc;  // Callback for Input into the Window
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(GetModuleHandle(NULL), IDI_APPLICATION);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // This means we decide the look of the cursor(arrow)
        wc.lpszClassName = lpWindowName;           // This is NOT the title, just a unique identifier(ID)

        if (!RegisterClassEx(&wc)) {
            std::cerr << "Window registration failed.\n";
            return false;
        }

        // Window config
        RECT windowRect = {};
        SetRect(&windowRect, 1, 1, 32, 32);
        AdjustWindowRect(&windowRect, dwStyle, 0);

        hwnd = CreateWindowEx(
            0,
            lpWindowName,  // This references lpszClassName from wc
            lpWindowName,  // This is the actual Title
            dwStyle,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            hWndParent,             // parent
            hMenu,                  // menu
            GetModuleHandle(NULL),  // handle instance
            this);                  // additional application data

        if (hwnd == NULL) {
            std::cerr << "Failed to create m_hwnd. ERROR: " << GetLastError() << "\n";
            return false;
        }

        // Show the main window
        ShowWindow(hwnd, SW_SHOW);

        return true;
    };

    HWND handle() const { return hwnd; }
    void updateWindow() {
        MSG msg;
        BOOL ret;
        while ((ret = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) != 0) {
            if (ret == -1) {
                std::cout << "There was an error in the msg loop: " << GetLastError() << "\n";
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);  // Calls the callback specified when creating the window
            }
        }
    }
    void closeWindow() {
        shouldClose = true;
    }
    bool windowShouldClose() {
        return shouldClose;
    }
    bool makeContextCurrent() {
        // Device Context
        HDC hdc = GetDC(hwnd);

        // Create create, choose and set the pixel format
        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
            std::cout << "Failed to Set pixel format\n";
            return false;
        }

        // Temp context
        HGLRC tempContext = wglCreateContext(hdc);
        wglMakeCurrent(hdc, tempContext);
        if (!wglMakeCurrent(hdc, tempContext)) {
            std::cout << "Failed to make WGL Temporary Context current\n";
            return false;
        }

        // Load WGL extensions
        if (!gladLoadWGL(hdc)) {
            std::cout << "Failed to load WGL extensions\n";
            return false;
        }

        // Set OpenGL versions
        const int attribList[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            3,
            WGL_CONTEXT_MINOR_VERSION_ARB,
            3,
            WGL_CONTEXT_FLAGS_ARB,
            0,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0,
        };

        // Final opengl context
        HGLRC openglContext = wglCreateContextAttribsARB(hdc, 0, attribList);

        // Release the device context
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);

        if (!wglMakeCurrent(hdc, openglContext)) {
            std::cout << "Failed to make OpenGl Context current\n";
            return false;
        }

        // Load glad and opengl functions
        if (!gladLoadGL()) {
            std::cout << "Failed to load Glad OpenGL\n";
            return false;
        }

        return true;
    }

   protected:
    virtual PCWSTR ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    HWND hwnd = 0;
    bool shouldClose = false;
};
