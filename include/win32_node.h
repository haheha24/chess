#pragma once
#include <windows.h>
#include <memory>

// Window mutable state
struct WindowBase {
    WindowBase() {};
    WindowBase(std::shared_ptr<WindowBase> newHead) : head(newHead) {};

    std::shared_ptr<WindowBase> head = nullptr;
    std::shared_ptr<WindowBase> next = nullptr;

    bool shouldClose = false;
    HWND handle = 0;
};
typedef std::shared_ptr<WindowBase> WindowBasePtr;

WindowBasePtr win32_nodeCreate(WindowBasePtr head = nullptr);

bool win32_nodeAdd(WindowBasePtr head, WindowBasePtr node);

bool win32_nodeRemove(WindowBasePtr node, int pos);