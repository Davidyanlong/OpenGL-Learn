#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    // NDC ¿Õ¼äµÄ×ø±ê [-1, 1]
    gl_Position = vec4(aPos, 1.0);
}