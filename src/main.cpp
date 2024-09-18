#include "application.h"
#include "win32_platform.h"
#include "glad/glad_wgl.h"
#include "shader.h"

int main() {
    Window MainWindow(L"Graphic User Interface");
    MainWindow.win32CreateWindow(800, 800);
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

    while (app.isRunning()) {
        MainWindow.win32UpdateWindow();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    return 0;
}