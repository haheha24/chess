#include <iostream>
#include "win32_node.h"

WindowBasePtr win32_nodeCreate(WindowBasePtr head) {
    WindowBasePtr node = WindowBasePtr(new WindowBase{head});
    return node;
}

bool win32_nodeAdd(WindowBasePtr head, WindowBasePtr node) {
    if (head == nullptr || node == nullptr) {
        std::cerr << "ERROR::Neither arguments can be a nullptr\n";
        return false;
    }
    head->next = node;
    return true;
};

bool win32_nodeRemove(WindowBasePtr node) {
    if (node->head == nullptr) {
        std::cerr << "This is the main window (head node). Cannot remove from list: " << node->head << "\n";
        return false;
    }
    node->head->next = nullptr;
    node = nullptr;
    return true;
};