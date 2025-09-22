#version 460 core

#include "Defines.glsl"

// Uniform Response Diffuse Lambertian
float DiffuseLambertian() {
    return M_1_OVER_PI;
}

// Trowbridge-Reitz GGX Normal Distribution
float DistributionGGX(float NdotH, float roughness) {
#define USE_EFFICIENT_GGX_IMPL 1
#if !USE_EFFICIENT_GGX_IMPL
    float a2 = roughness * roughness;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    denom = M_PI * denom * denom;
    return a2 / max(denom, M_EPSILON); // Prevent division by zero
#else
    // A more efficient version
    float a = NdotH * roughness;
    float denom = 1.0 - NdotH * NdotH + a * a;
    float k = roughness / max(denom, M_EPSILON); // Prevent division by zero
    return k * k * M_1_OVER_PI;
#endif
}

// Schlick Geometry Function
float SchlickGGX(float NdotV, float k) {
    return NdotV / (NdotV * (1.0 - k) + k);
}

// Smith's Method
float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float ggx1 = SchlickGGX(NdotV, k);
    float ggx2 = SchlickGGX(NdotL, k);
    return ggx1 * ggx2;
}

// Fresnel Schlick
vec3 FresnelSchlick(float HdotV, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}
