#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "application.h"
#include "win32_mainwindow.h"
#include "shader.h"
#include "StbImageLoader.h"

int main() {
    MainWindow mainWindow;
    int screenWidth = 800, screenHeight = 800;

    if (!mainWindow.createWindow(L"My Gui", WS_OVERLAPPEDWINDOW, 0, 100, 100, screenWidth, screenHeight)) {
        std::cout << "Failed to create the window: " << GetLastError() << "\n";
        return 0;
    }
    if (!mainWindow.makeContextCurrent()) {
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
    // clang-format on

    unsigned int VBO, VAO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);              // bind vertex array first
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // then bind buffers and set buffer data
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
    /* auto start = std::chrono::high_resolution_clock::now(); */

    POINT mousePtr;

    while (!mainWindow.windowShouldClose()) {
        mainWindow.setWindowSize(screenWidth, screenHeight);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GetCursorPos(&mousePtr);

        /* auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = current - start;
        float time = duration.count();
        trans = glm::rotate(trans, time, glm::vec3(0.5f, 0.f, 0.5f)); */

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(myshader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(myshader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(myshader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        mainWindow.updateWindow();
        SwapBuffers(GetDC(mainWindow.handle()));
    }

    return 0;
}