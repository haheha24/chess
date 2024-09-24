#include <iostream>
#include "win32_mainwindow.h"

LRESULT _WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowBase* windowPtr = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (msg) {
        case WM_CLOSE:
            if (windowPtr) {
                win32_closeWindow(*windowPtr);
            }
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            EndPaint(hwnd, &ps);
            return 0;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

WindowBasePtr win32_createWindow(PCWSTR lpWindowName,
                                 DWORD dwStyle,
                                 DWORD dwExStyle,
                                 int x,
                                 int y,
                                 int nWidth,
                                 int nHeight,
                                 HWND hWndParent,
                                 HMENU hMenu) {
    HWND hwnd;
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
        return NULL;
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
        NULL);                  // additional application data

    if (hwnd == NULL) {
        std::cerr << "Failed to create m_hwnd. ERROR: " << GetLastError() << "\n";
        return NULL;
    }

    WindowBasePtr windowNode = win32_nodeCreate();
    windowNode->handle = hwnd;
    // Store the window pointer in the user data of the HWND so that it can be accessed by the _WindowProc
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowNode.get()));

    if (!win32_makeContextCurrent(hwnd)) {
        std::cout << "Failed to load OpenGL Context" << "\n";
        return NULL;
    }

    // Show the main window
    ShowWindow(hwnd, SW_SHOW);

    return windowNode;
}

void win32_updateWindow(WindowBasePtr& window) {
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

void win32_closeWindow(WindowBase& window) {
    window.shouldClose = true;
}

bool win32_windowShouldClose(bool shouldClose) {
    return shouldClose;
}

bool win32_makeContextCurrent(HWND& hwnd) {
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
