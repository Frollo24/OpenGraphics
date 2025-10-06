#version 460 core

struct DirectionalLight {
    vec3 color;
    float intensity;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    float intensity;
    vec3 position;
    float radius;
};

struct SpotLight {
    vec3 color;
    float intensity;
    vec3 position;
    float radius;
    vec3 direction;
    float _futureUse;
    float innerCutoff;
    float outerCutoff;
};

struct Light {
    vec3 color;
    float intensity;
    vec3 direction;
    float attenuation;
};

uniform vec3 u_CameraPosition;  // TODO: think for a better place for this

layout (std140, binding = 0) uniform Lights {
    uniform DirectionalLight u_DirectionalLight;
    uniform PointLight u_PointLight;
};

#include "BlinnPhongFunctions.glsl"
#include "DisneyEpicGamesFunctions.glsl"
