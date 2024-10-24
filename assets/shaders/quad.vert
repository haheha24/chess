#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}