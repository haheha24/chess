#include <iostream>
#include "application.h"
#include "win32_mainwindow.h"
#include "glad/glad_wgl.h"
#include "shader.h"

int main() {
    WindowBasePtr window = win32_createWindow(L"My Gui", WS_OVERLAPPEDWINDOW, 0, 100, 100, 800, 800);
    if (!window) {
        std::cout << "Failed to create the window: " << GetLastError() << "\n";
        return 0;
    }
    if (!win32_makeContextCurrent(window->handle)) {
        std::cout << "Failed to make the context current";
        return 0;
    }

    const char* vertShader = "assets/shaders/quad.vert";
    const char* fragShader = "assets/shaders/quad.frag";

    Shader myshader = Shader(vertShader, fragShader);
    // clang-format off
    float vertices[] = {
		// positions          // colors          
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   // top left 
	};
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    //clang-format on

    unsigned int VBO, VAO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO); // bind vertex array first
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // then bind buffers and set buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    myshader.use();

    while (!win32_windowShouldClose(window->shouldClose)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        win32_updateWindow(window);
        SwapBuffers(GetDC(window->handle));
    }

    return 0;
}