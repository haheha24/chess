#include "application.h"

Application::Application() {}

bool Application::isRunning() {
    return running;
};
void Application::closeWindow() {
    running = false;
}