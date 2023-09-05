#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{   
    // ��Ⱦ�������������
    vec3 I = normalize(Position - cameraPos);
    // ��������ǿ������ص�ķ�������
    vec3 R = reflect(I, normalize(Normal));
    // ���ݷ�������ȥ�ɼ�cube��ͼ�ϵ���ɫ
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}