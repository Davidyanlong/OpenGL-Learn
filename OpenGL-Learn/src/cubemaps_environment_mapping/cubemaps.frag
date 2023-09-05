#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{   
    // 渲染点与相机的向量
    vec3 I = normalize(Position - cameraPos);
    // 计算出我们看的像素点的反射向量
    vec3 R = reflect(I, normalize(Normal));
    // 根据反射向量去采集cube贴图上的颜色
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}