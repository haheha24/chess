#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "application.h"
#include "win32_mainwindow.h"
#include "shader.h"
#include "ImageLoader.h"

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
        // top
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   // top right
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   // top left
        // front
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    // top right
		 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    // bottom right
		-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    // bottom left
		-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,    // top left
        // bottom
		 0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   // top right
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   // bottom left
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   // top left
        // back
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   // top right
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   // bottom left
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   // top left
        // left
		-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,    // top right
		-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    // bottom right
		 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    // bottom left
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    // top left
        // right
	    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   // top right
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   // bottom right
		 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    // bottom left
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    // top left
	};
    unsigned int indices[] = {
        0, 1, 3,    // top first triangle
        1, 2, 3,    // top second triangle
        4, 5, 7,    // front first
        5, 6, 7,    // front second
        8, 9, 11,   // bottom first
        9, 10, 11,  // bottom second
        12, 13, 15, // back first
        13, 14, 15, // back second
        16, 17, 19, // left first
        17, 18, 19, // left second
        20, 21, 23, // right first
        21, 22, 23  // right second
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

    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

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
        model = glm::rotate(model, glm::radians(55.f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screenWidth / screenHeight), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(myshader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(myshader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(myshader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        mainWindow.updateWindow();
        SwapBuffers(GetDC(mainWindow.handle()));
    }

    return 0;
}