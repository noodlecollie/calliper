#version 410 core
in vec3 position;
in vec2 uv;
out vec2 UV;
void main()
{
    gl_Position = vec4(position, 1);
    UV = uv;
}
