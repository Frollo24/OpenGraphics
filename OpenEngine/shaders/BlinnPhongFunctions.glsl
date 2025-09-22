#version 460 core

struct BlinnPhongSurfaceData {
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float glossiness;
};

vec3 AmbientBlinnPhong(BlinnPhongSurfaceData surface) {
    // Ambient component
    return surface.diffuse * 0.05;
}

vec3 LightBlinnPhongWorld(Light light, BlinnPhongSurfaceData surface, vec3 worldViewDirection, vec3 worldNormal) {
    vec3 color = vec3(0.0);

    // Surface parameters
    vec3 Kd = surface.diffuse.rgb;
    vec3 Ks = surface.specular.rgb;
    float glossyExp = 10.0 * surface.glossiness + 1.0;
    float glossyFactor = pow(2.0, glossyExp);

    // Lighting vectors
    vec3 L = normalize(light.direction);
    vec3 V = normalize(worldViewDirection);
    vec3 H = normalize(L + V);
    vec3 N = worldNormal;

    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0);

    // Diffuse component
    vec3 diffuse = Kd;

    // Specular component
    vec3 specular = Ks * pow(NdotH, glossyFactor);

    // Incoming radiance
    vec3 radiance = light.color * light.intensity * light.attenuation * NdotL;

    color = diffuse + specular;
    color *= radiance;
    return color;
}

vec3 DirectionalLightBlinnPhongWorld(DirectionalLight directionalLight, BlinnPhongSurfaceData surface, vec3 worldPos, vec3 worldNormal) {
    vec3 worldViewDirection = normalize(u_CameraPosition - worldPos);

    Light light;
    light.color = directionalLight.color;
    light.intensity = directionalLight.intensity;
    light.direction = normalize(-directionalLight.direction);
    light.attenuation = 1.0;  // Directional light does not attenuate w.r.t. distance

    return LightBlinnPhongWorld(light, surface, worldViewDirection, worldNormal);
}

vec3 PointLightBlinnPhongWorld(PointLight pointLight, BlinnPhongSurfaceData surface, vec3 worldPos, vec3 worldNormal) {
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

    return LightBlinnPhongWorld(light, surface, worldViewDirection, worldNormal);
}

vec3 SpotLightBlinnPhongWorld(SpotLight spotLight, BlinnPhongSurfaceData surface, vec3 worldPos, vec3 worldNormal) {
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

    return LightBlinnPhongWorld(light, surface, worldViewDirection, worldNormal);
}

vec3 EmissiveBlinnPhongWorld(BlinnPhongSurfaceData surface, vec3 worldPos) {
    float dist = distance(u_CameraPosition, worldPos);

    // Emissive component
    float attFactor = 1.5;
    float emmAttenuation = 0.1 + dist * dist;
    vec3 emissive = surface.emissive * (-attFactor * smoothstep(0, 10, emmAttenuation) + 2.0 * attFactor);
    return emissive;
}
