#pragma once

class Application {
   public:
    Application();
    bool createWindow();
    void udpateWindow();
    void closeWindow();
    bool isRunning();

   protected:
    bool running = true;
};
