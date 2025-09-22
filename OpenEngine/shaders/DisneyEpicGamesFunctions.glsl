#version 460 core

#include "PBRFunctions.glsl"

struct DisneyEpicSurfaceData {
    vec4 baseColor;
    float metallic;
    float roughness;
};

vec3 AmbientDisneyEpic(DisneyEpicSurfaceData surface) {
    // Ambient component
    return surface.baseColor.rgb * 0.01;
}

vec3 LightDisneyEpicWorld(Light light, DisneyEpicSurfaceData surface, vec3 worldViewDirection, vec3 worldNormal) {
    vec3 color = vec3(0.0);

    // Surface parameters
    vec3 baseColor = surface.baseColor.rgb;
    float metallic = surface.metallic;
    float perceptualRoughness = max(0.04, surface.roughness); // Preserve specular highlights

    float roughness = perceptualRoughness * perceptualRoughness;
    vec3 F0 = mix(vec3(0.04), baseColor.rgb, metallic);

    // Lighting vectors
    vec3 L = normalize(light.direction);
    vec3 V = normalize(worldViewDirection);
    vec3 H = normalize(L + V);
    vec3 N = worldNormal;

    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    // PBR parameters
    float D = DistributionGGX(NdotH, roughness);
    float G = GeometrySmith(NdotV, NdotL, roughness);
    vec3 F = FresnelSchlick(HdotV, F0);

    // Diffuse component
    vec3 diffuse = (1.0 - metallic) * baseColor * DiffuseLambertian();

    // Specular component
    vec3 specular = D * G * F;
    specular /= max(4.0 * NdotV * NdotL, M_EPSILON); // Prevent division by zero

    // Incoming radiance
    vec3 radiance = light.color * light.intensity * light.attenuation * NdotL;

    color = diffuse + specular;
    color *= radiance;
    return color;
}

vec3 DirectionalLightDisneyEpicWorld(DirectionalLight directionalLight, DisneyEpicSurfaceData surface, vec3 worldPos, vec3 worldNormal) {
    vec3 worldViewDirection = normalize(u_CameraPosition - worldPos);

    Light light;
    light.color = directionalLight.color;
    light.intensity = directionalLight.intensity;
    light.direction = normalize(-directionalLight.direction);
    light.attenuation = 1.0;  // Directional light does not attenuate w.r.t. distance

    return LightDisneyEpicWorld(light, surface, worldViewDirection, worldNormal);
}

vec3 PointLightDisneyEpicWorld(PointLight pointLight, DisneyEpicSurfaceData surface, vec3 worldPos, vec3 worldNormal) {
    vec3 lightDirection = normalize(pointLight.position - worldPos);
    vec3 worldViewDirection = normalize(u_CameraPosition - worldPos);

    float dist = distance(pointLight.position, worldPos);
    float distanceAttenuation = 1.0 / (0.1 + dist * dist);
    distanceAttenuation = mix(distanceAttenuation, 0, clamp(dist / pointLight.radius, 0, 1));

    Light light;
    light.color = pointLight.color;
    light.intensity = pointLight.intensity;
    light.direction = lightDirection;
    light.attenuation = distanceAttenuation;

    return LightDisneyEpicWorld(light, surface, worldViewDirection, worldNormal);
}

vec3 SpotLightBlinnPhongWorld(SpotLight spotLight, DisneyEpicSurfaceData surface, vec3 worldPos, vec3 worldNormal) {
    vec3 lightDirection = normalize(spotLight.position - worldPos);
    vec3 worldViewDirection = normalize(u_CameraPosition - worldPos);

    float dist = distance(spotLight.position, worldPos);
    float distanceAttenuation = 1.0 / (0.1 + dist * dist);
    distanceAttenuation = mix(distanceAttenuation, 0, clamp(dist / spotLight.radius, 0, 1));

    float theta = dot(lightDirection, -spotLight.direction);
    // We invert the substraction because the cosine function (i.e. the dot product) goes
    // from 1 to 0 when increasing the angle from 0 to 90 degrees
    float epsilon = spotLight.innerCutoff - spotLight.outerCutoff;
    float spotIntensityAttenuation = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0, 1.0);

    Light light;
    light.color = spotLight.color;
    light.intensity = spotLight.intensity;
    light.direction = lightDirection;
    light.attenuation = distanceAttenuation * spotIntensityAttenuation;

    return LightDisneyEpicWorld(light, surface, worldViewDirection, worldNormal);
}

vec3 EmissiveDisneyEpicWorld(DisneyEpicSurfaceData surface, vec3 worldPos) {
    // TODO: implement
    return vec3(0.0);
}
