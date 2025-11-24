#version 460 core

#ifndef _PBR_FUNCTIONS_
#define _PBR_FUNCTIONS_

#include "Defines.glsl"


// ============================================================
// NORMAL DISTRIBUTION FUNCTIONS
// ============================================================

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

// Ashikhmin Velvet Normal Distribution
float DistributionAshikhmin(float NdotH, float roughness) {
    // Ashikhmin 2007, "Distribution-based BRDFs"
    float a2 = roughness * roughness;
    float cos2h = NdotH * NdotH;
    float sin2h = max(1.0 - cos2h, M_EPSILON); // Prevent division by zero
    float sin4h = sin2h * sin2h;
    float cot2h = cos2h / sin2h;

    float expTerm = exp(-cot2h / a2);
    float denom = M_PI * (4.0 * a2 + 1.0) * sin4h;
    return (sin4h + 4.0 * expTerm) / max(denom, M_EPSILON); // Prevent division by zero
}

// Beckmann Normal Distribution
float DistributionBeckmann(float NdotH, float roughness) {
    float NdotH2 = NdotH * NdotH;
    float NdotH4 = NdotH2 * NdotH2;
    float a2 = roughness * roughness;

    float tanTheta2 = (1.0 - NdotH2) / max(NdotH2, M_EPSILON); // Prevent division by zero
    float expTerm = exp(-tanTheta2 / a2);
    float denom = M_PI * a2 * NdotH4;
    return expTerm / max(denom, M_EPSILON); // Prevent division by zero
}


// ============================================================
// GEOMETRY ATTENUATION FUNCTIONS
// ============================================================

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

// Smith's Method for Image-Based Lighting
float GeometrySmithIBL(float NdotV, float NdotL, float roughness) {
    float r = roughness;
    float k = (r * r) / 2.0;
    float ggx1 = SchlickGGX(NdotV, k);
    float ggx2 = SchlickGGX(NdotL, k);
    return ggx1 * ggx2;
}

// Geometry function for use with Velvet
float GeometryNeubelt(float NdotV, float NdotL) {
    // Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
    float denom = 4.0 * (NdotL + NdotV - NdotL * NdotV);
    return 1.0 / max(denom, M_EPSILON);
}

float BeckmannG1(float NdotX, float roughness) {
    float NdotX2 = NdotX * NdotX;
    float tanTheta = max(sqrt((1 - NdotX2) / NdotX2), M_EPSILON); // Prevent division by zero
    float a = 1.0 / (roughness * tanTheta);
    float a2 = a * a;

    float g = 1.0;
    if (a < 1.6)
        g = (3.535 * a + 2.181 * a2) / (1.0 + 2.276 * a + 2.577 * a2);

    return g;
}

float GeometryBeckmann(float NdotV, float NdotL, float roughness) {
    float g1i = BeckmannG1(NdotL, roughness);
    float g1o = BeckmannG1(NdotV, roughness);
    return g1i * g1o;
}


// ============================================================
// FRESNEL APPROXIMATION FUNCTIONS
// ============================================================

// Fresnel Schlick where f90 = 1
vec3 FresnelSchlick(float u, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - u, 5.0);
}

// Fresnel Schlick where f90 = param
vec3 FresnelSchlick(float u, vec3 F0, vec3 F90) {
    return F0 + (F90 - F0) * pow(1.0 - u, 5.0);
}

// Float Fresnel Schlick where f90 = 1
float FresnelSchlickFloat(float u, float F0) {
    return F0 + (1.0 - F0) * pow(1.0 - u, 5.0);
}

// Float Fresnel Schlick where f90 = param
float FresnelSchlickFloat90(float u, float F0, float F90) {
    return F0 + (F90 - F0) * pow(1.0 - u, 5.0);
}


// ============================================================
// DIFFUSE COMPONENT FUNCTIONS
// ============================================================

// Uniform Response Diffuse Lambertian
float DiffuseLambertian() {
    return M_1_OVER_PI;
}

// Disney Burley Diffuse
float DiffuseDisneyBurley(float NdotV, float NdotL, float HdotL, float roughness) {
    float f90 = 0.5 + 2.0 * roughness * HdotL * HdotL;
    float viewScatter = FresnelSchlickFloat90(NdotV, 1, f90);
    float lightScatter = FresnelSchlickFloat90(NdotL, 1, f90);
    return lightScatter * viewScatter * M_1_OVER_PI;
}

// Oren Nayar Diffuse Generalization
float DiffuseOrenNayar(float NdotV, float NdotL, float phiDiff, float roughness) {
    // Michael Oren and Shree K. Nayar. 1994. Generalization of lambert's reflectance model.

    // Oren-Nayar constants
    const float sigma = roughness;
    const float sigma2 = sigma * sigma;

    const float A = 1.0 - 0.5 * sigma2 / (sigma2 + 0.33);
    const float B = 0.45 * sigma2 / (sigma2 + 0.09);

    const float alpha = max(acos(NdotV), acos(NdotL));
    const float sinAlpha = sin(alpha);

    const float beta = min(acos(NdotV), acos(NdotL));
    const float tanBeta = tan(beta);

    return (A + B * clamp(phiDiff, 0.0, 1.0) * sinAlpha * tanBeta) * M_1_OVER_PI;
}

#endif
