#version 460 core
layout(location = 0) in vec3 v_WorldPosition;
layout(location = 1) in vec3 v_WorldNormal;
layout(location = 2) in vec2 v_TexCoord;
layout(location = 3) in mat3 v_TBN;

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_DiffuseTexture;
layout(binding = 1) uniform sampler2D u_SpecularTexture;
layout(binding = 2) uniform sampler2D u_EmissiveTexture;
layout(binding = 3) uniform sampler2D u_NormalTexture;

uniform vec4 u_MainColor;

struct Material {
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 emissiveColor;
};

uniform Material u_Material;

#include "Transform.glsl"
#include "Lighting.glsl"

void main() {
    int textureSz = textureSize(u_NormalTexture, 0).x;
    bool usingNormalmap = textureSz > 1 ? true : false;
    vec3 normalTS = usingNormalmap ? texture(u_NormalTexture, v_TexCoord).xyz : vec3(0.0, 0.0, 1.0);
    vec3 normalWS = GetSurfaceNormalWorldSpace(v_WorldNormal, normalTS, v_TBN, usingNormalmap);

    BlinnPhongSurfaceData blinnPhongData;
    blinnPhongData.diffuse = texture(u_DiffuseTexture, v_TexCoord).rgb * u_Material.diffuseColor.rgb;
    blinnPhongData.specular = texture(u_SpecularTexture, v_TexCoord).rgb * u_Material.specularColor.rgb;
    blinnPhongData.emissive = texture(u_EmissiveTexture, v_TexCoord).rgb * u_Material.emissiveColor.rgb;
    blinnPhongData.glossiness = 1.0;

    DirectionalLight light;
    light.color = vec3(1.0);
    light.intensity = 1.0;
    light.direction = normalize(vec3(-1.0, -1.0, -1.0));

    vec3 finalColor = vec3(0.0);
    finalColor += AmbientBlinnPhong(blinnPhongData);
    finalColor += DirectionalLightBlinnPhongWorld(light, blinnPhongData, v_WorldPosition, normalWS);
    finalColor += EmissiveBlinnPhongWorld(blinnPhongData, v_WorldPosition);

    o_Color = vec4(finalColor, 1.0);
}