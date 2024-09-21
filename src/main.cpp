#include "application.h"
#include "win32_basewindow.h"
#include "glad/glad_wgl.h"
#include "shader.h"

int main() {
    HWND* window = win32_createWindow(L"My Gui", WS_OVERLAPPEDWINDOW, 0, 100, 100, 800, 800);
    if (!window) {
        std::cout << "Failed to create the window: " << GetLastError() << "\n";
        return 0;
    }
    Application app{};

    const char* vertShader = "assets/shaders/quad.vert";
    const char* fragShader = "assets/shaders/quad.frag";

    Shader myshader = Shader(vertShader, fragShader);

    float vertices[] = {
        // x,y,z                colors
        -0.5f, -0.5f, 0.f,
        0.f, 0.5f, 0.f,
        0.5f, -0.5f, 0.f};

    unsigned int VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    myshader.use();

    while (win32_windowShouldClose()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    win32_closeWindow(window);

    return 0;
}