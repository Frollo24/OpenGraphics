#version 460 core
layout(location = 0) in vec3 v_WorldPosition;
layout(location = 1) in vec3 v_WorldNormal;
layout(location = 2) in vec2 v_TexCoord;
layout(location = 3) in mat3 v_TBN;

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_BaseColorTexture;
layout(binding = 1) uniform sampler2D u_MetallicTexture;
layout(binding = 2) uniform sampler2D u_RoughnessTexture;
layout(binding = 3) uniform sampler2D u_NormalTexture;

uniform vec4 u_MainColor;

struct Material {
    vec4 baseColor;
    float metallic;
    float roughness;
};

uniform Material u_Material;

#include "Transform.glsl"
#include "Lighting.glsl"

void main() {
    int textureSz = textureSize(u_NormalTexture, 0).x;
    bool usingNormalmap = textureSz > 1 ? true : false;
    vec3 normalTS = usingNormalmap ? texture(u_NormalTexture, v_TexCoord).xyz : vec3(0.0, 0.0, 1.0);
    vec3 normalWS = GetSurfaceNormalWorldSpace(v_WorldNormal, normalTS, v_TBN, usingNormalmap);

    DisneyEpicSurfaceData disneyEpicData;
    disneyEpicData.baseColor = texture(u_BaseColorTexture, v_TexCoord) * u_Material.baseColor;
    disneyEpicData.metallic = texture(u_MetallicTexture, v_TexCoord).r * u_Material.metallic;
    disneyEpicData.roughness = texture(u_RoughnessTexture, v_TexCoord).r * u_Material.roughness;

    DirectionalLight light;
    light.color = vec3(1.0);
    light.intensity = 2.0;
    light.direction = normalize(vec3(-1.0, -1.0, -1.0));

    vec3 finalColor = vec3(0.0);
    finalColor += AmbientDisneyEpic(disneyEpicData);
    finalColor += DirectionalLightDisneyEpicWorld(light, disneyEpicData, v_WorldPosition, normalWS);
    finalColor += EmissiveDisneyEpicWorld(disneyEpicData, v_WorldPosition);

    // TODO: make the HDR to 0..1 range conversion into separate functions
    // finalColor = finalColor / (finalColor + vec3(1.0));
    o_Color = vec4(finalColor, 1.0);
}
