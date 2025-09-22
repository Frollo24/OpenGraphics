#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

layout(location = 0) out vec3 v_WorldPosition;
layout(location = 1) out vec3 v_WorldNormal;
layout(location = 2) out vec2 v_TexCoord;
layout(location = 3) out mat3 v_TBN;

#include "Transform.glsl"

void main() {
    VertexPositionInputs vpi = GetVertexPositionInputs(a_Position);
    v_WorldPosition = vpi.positionWorldSpace;
    gl_Position = vpi.positionClipSpace;

    VertexNormalInputs vni = GetVertexNormalInputs(a_Normal);
    v_WorldNormal = vni.normalWorldSpace;

    vec3 T = vni.tangentWorldSpace;
    vec3 B = vni.bitangentWorldSpace;
    vec3 N = vni.normalWorldSpace;

    v_WorldNormal = N;
    v_TexCoord = a_TexCoord;
    v_TBN = mat3(T, B, N);
}