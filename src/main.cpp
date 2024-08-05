// ###########################################################################
//                             Platform Globals
// ###########################################################################
static bool running = true;

// ###########################################################################
//                             Platform Functions
// ###########################################################################
bool platform_create_window(int width, int height, char const* title);
void platform_update_window();

// ###########################################################################
//                             WINDOWS Platform
// ###########################################################################
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// ###########################################################################
//                             WINDOWS Globals
// ###########################################################################
static HWND window;
// ###########################################################################
//                            Platform Implementations
// ###########################################################################
LRESULT CALLBACK windows_window_callback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (msg) {
        case WM_CLOSE:
            running = false;
            break;

        default:
            result = DefWindowProcA(window, msg, wParam, lParam);
    }

    return result;
}

bool platform_create_window(int width, int height, char const* title) {
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = title;                  // This is not the TITLE of the window, just a unique ID
    wc.lpfnWndProc = windows_window_callback;  // Callback for input into the window

    if (!RegisterClassA(&wc)) {
        return false;
    }

    // WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
    int dwStyle = WS_OVERLAPPEDWINDOW;

    window = CreateWindowExA(0,
                             title,  // lpszClassName from wc
                             title,  // actual title of the window
                             dwStyle,
                             100,
                             100,
                             width,
                             height,
                             NULL,  // parent
                             NULL,  // menu
                             instance,
                             NULL);  // lpParam

    if (window == NULL) {
        return false;
    }
    ShowWindow(window, SW_SHOW);
    return true;
}

void platform_update_window() {
    MSG msg;
    while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);  // Calls the callback specified when creating the window
    }
};

#endif

int main() {
    platform_create_window(1200, 720, "Chess");

    while (running) {
        // update
        platform_update_window();
    }

    return 0;
}