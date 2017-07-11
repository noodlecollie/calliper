#version 410 core

in vec2 fPosition;

layout(location = 0) out vec4 color;

void main()
{
    float red = (clamp(fPosition.x, -1.0f, 1.0f) + 1.0f) / 2.0f;
    float green = (clamp(fPosition.y, -1.0f, 1.0f) + 1.0f) / 2.0f;

    color = vec4(red, green, 0, 1);
}
