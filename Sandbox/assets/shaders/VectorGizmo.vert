#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec4 v_Color;

uniform mat4 u_ModelViewProj;

void main() {
    gl_Position = u_ModelViewProj * vec4(a_Position, 1.0);
    gl_PointSize = 3.0;

    v_Color = a_Color;
}