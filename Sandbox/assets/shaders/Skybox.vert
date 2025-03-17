#version 450 core

layout(location = 0) in vec3 a_SkyboxVertices;

layout(location = 0) out vec3 v_TexCoords;

#include "Transform.glsl"

void main()
{
    v_TexCoords = a_SkyboxVertices;
    vec4 pos = u_ViewProj * vec4(a_SkyboxVertices, 1.0);
    gl_Position = pos.xyww;
}
