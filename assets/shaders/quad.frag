#version 330 core
out vec4 FragTex;

in vec2 texCoord;

uniform sampler2D cheeky;

void main()
{
    FragTex = texture(cheeky, texCoord);
}