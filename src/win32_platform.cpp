#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include "win32_platform.h"
#include "glad/glad_wgl.h"

Window::Window(const wchar_t* title) : className(title) {
}

/**
 * wc.lpfnWndProc type is not the same as a class Window::windowProc because of the 'this' variable inherent in class objects.
 * Storing the Window Object inside GWLP_USERDATA inside WindowLongPtr preserves the Window instance, but also removes 'this'
 * Once pThis has been assigned, it will handle messages as normal
 */
LRESULT CALLBACK Window::windowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* pThis = nullptr;

    // When receiving WM_NCCREATE, store the window instance (this) in GWLP_USERDATA
    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        // Retrieve the window instance from GWLP_USERDATA for all other messages
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
    }

    // Make sure pThis is valid before calling any instance method
    if (pThis) {
        // Call the non-static member function to handle the message
        return pThis->handleWindowProc(window, msg, wParam, lParam);
    }

    // Fallback to default if pThis is null
    return DefWindowProc(window, msg, wParam, lParam);
}

// The 'real' windowProc
LRESULT Window::handleWindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(window, msg, wParam, lParam);
    }
    return 0;
}

bool Window::win32CreateWindow(int width, int height) {
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // This means we decide the look of the cursor(arrow)
    wc.lpszClassName = className;              // This is NOT the title, just a unique identifier(ID)
    wc.lpfnWndProc = Window::windowProc;       // Callback for Input into the Window

    RegisterClass(&wc);

    // Window config
    SetRect(&windowRect, 1, 1, 32, 32);
    AdjustWindowRect(&windowRect, style, 0);

    window = CreateWindowEx(0, className,  // This references lpszClassName from wc
                            className,     // This is the actual Title
                            style,
                            100,
                            100,
                            width,
                            height,
                            NULL,  // parent
                            NULL,  // menu
                            instance,
                            NULL);  // lpParam

    // Device Context
    HDC hdc = GetDC(window);

    // Create create, choose and set the pixel format
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    // Temp context
    HGLRC tempContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempContext);

    // Load WGL extensions
    gladLoadWGL(hdc);

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
    wglMakeCurrent(hdc, openglContext);

    // Load glad and opengl functions
    gladLoadGL();

    // Show the main window
    ShowWindow(window, SW_SHOW);

    return true;
}

void Window::win32UpdateWindow() {
    MSG msg;
    while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);  // Calls the callback specified when creating the window
    }
};