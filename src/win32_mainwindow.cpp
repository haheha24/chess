#include "win32_mainwindow.h"

PCWSTR MainWindow::ClassName() const { return L"Sample Window Class"; }

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            BaseWindow::closeWindow();
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            winWidth = LOWORD(lParam);   // New width
            winHeight = HIWORD(lParam);  // New height

            switch (wParam) {
                case SIZE_RESTORED:
                    // Handle normal window resize
                    if (winWidth > 0 && winHeight > 0) {
                        if (openglContextIsCurrent) {
                            // Resize OpenGL viewport or adjust other elements
                            glViewport(0, 0, winWidth, winHeight);
                        }
                    }
                    break;

                case SIZE_MINIMIZED:
                    // Window minimized, possibly pause rendering
                    break;

                case SIZE_MAXIMIZED:
                    // Handle maximized window, adjust the layout as needed
                    if (openglContextIsCurrent) {
                        glViewport(0, 0, winWidth, winHeight);
                    }
                    break;
            }
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
};