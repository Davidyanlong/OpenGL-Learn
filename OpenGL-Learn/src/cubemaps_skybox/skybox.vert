#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // ���ｫz�滻Ϊw�������ñ�����zֵ�ڲü��ռ�����Զ����1 ��z/w = w/w = 1, 
    // ������������ĵط�ͨ����Ȳ��ԣ������Ͳ��ڻ����� 
    gl_Position = pos.xyww;
} 