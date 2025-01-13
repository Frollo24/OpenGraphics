#version 460 core
layout(location = 0) in vec3 v_Normal;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

layout(location = 0) out vec4 o_Color;

uniform vec4 u_LightDir;
uniform vec4 u_MainColor;

void main() {
    vec3 normalWS = v_Normal;

    vec3 normalColor = normalize(normalWS) * 0.5 + 0.5;
    normalColor *= max(dot(normalize(u_LightDir.xyz), normalWS), 0.1);

    o_Color = vec4(normalColor, 1.0);
}
