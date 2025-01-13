#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

layout(location = 0) out vec3 v_Normal;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out mat3 v_TBN;

uniform mat4 u_ModelViewProj;
uniform mat4 u_Model;
uniform mat4 u_Normal;

void main() {
    gl_Position = u_ModelViewProj * vec4(a_Position, 1.0);
    vec3 T = normalize(vec3(u_Normal * vec4(a_Tangent, 0.0)));
    vec3 B = normalize(vec3(u_Normal * vec4(a_Bitangent, 0.0)));
    vec3 N = normalize(vec3(u_Normal * vec4(a_Normal, 0.0)));

    v_Normal = N;
    v_TexCoord = a_TexCoord;
    v_TBN = mat3(T, B, N);
}
