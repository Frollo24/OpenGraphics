#version 460 core

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 o_Color;
layout(depth_less) out float gl_FragDepth;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);

    if (length(coord) > 0.5)
        discard;

    float sqrLen = length(coord) * length(coord);
    gl_FragDepth = gl_FragCoord.z - 0.02 * sqrt(0.25 - sqrLen);

    o_Color = v_Color;
}