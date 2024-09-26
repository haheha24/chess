#pragma once

#include "win32_basewindow.h"

class MainWindow : public BaseWindow<MainWindow> {
   public:
    MainWindow() {}
    PCWSTR ClassName() const;
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};