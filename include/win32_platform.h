#include <windows.h>

class Window {
   public:
    Window(wchar_t* title);
    static LRESULT CALLBACK windowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

    bool win32CreateWindow(int width, int height);

    void win32UpdateWindow();

   private:
    LRESULT handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    const wchar_t* className;
    HWND window;
    HINSTANCE instance = GetModuleHandle(0);
    WNDCLASS wc = {};
    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
    RECT windowRect;
};
