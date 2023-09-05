#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // 这里将z替换为w，就是让背景的z值在裁剪空间中永远等于1 即z/w = w/w = 1, 
    // 这样在有物体的地方通过深度测试，背景就不在绘制了 
    gl_Position = pos.xyww;
} 