#include "win32_basewindow.h"

bool win32_closeWindow(bool close) {
    return close = true;
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            std::cout << "WM_CLOSE: " << msg << "\n";
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            std::cout << "WM_DESTROY: " << msg << "\n";
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
            std::cout << "End Paint: " << msg << "\n";
            return 0;
        }
        case WM_CREATE:
            std::cout << "WM_CREATE: " << msg << "\n";
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

HWND* win32_createWindow(PCWSTR lpWindowName,
                         DWORD dwStyle,
                         DWORD dwExStyle,
                         int x,
                         int y,
                         int nWidth,
                         int nHeight,
                         HWND hWndParent,
                         HMENU hMenu) {
    static HWND hwnd;
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WindowProc;  // Callback for Input into the Window
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // This means we decide the look of the cursor(arrow)
    wc.lpszClassName = lpWindowName;           // This is NOT the title, just a unique identifier(ID)

    if (!RegisterClassEx(&wc)) {
        std::cerr << "Window registration failed.\n";
        return 0;
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
        return 0;
    }

    if (!win32_makeContextCurrent(&hwnd)) {
        std::cout << "Failed to load OpenGL Context" << "\n";
        return 0;
    }

    // Show the main window
    ShowWindow(hwnd, SW_SHOW);

    return &hwnd;
}

bool win32_windowShouldClose() {
    MSG msg;
    BOOL ret;
    while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if (ret == -1) {
            std::cout << "There was an error in the msg when updating the msg loop: " << "\n";
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);  // Calls the callback specified when creating the window
        }
    }
}

void win32_closeWindow(HWND* hwnd) {
    delete hwnd;
}

bool win32_makeContextCurrent(HWND* hwnd) {
    // Device Context
    HDC hdc = GetDC(*hwnd);

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

    return true;
}
