#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// 场景的正常渲染的颜色framebuffer
uniform sampler2D scene;
// 经过模糊的亮度framebuffer
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

// 将两种贴图进行合成
void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(bloom)
        hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}