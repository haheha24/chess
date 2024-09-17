#include "application.h"
#include "win32_platform.h"

int main() {
    Application app{};
    bool running = app.isRunning();

    Window MainWindow(L"Graphic User Interface");

    MainWindow.win32CreateWindow(800, 800);

    while (running) {
        MainWindow.win32UpdateWindow();
    }

    return 0;
}