#version 460 core

uniform mat4 u_Model;
uniform mat4 u_ModelView;
uniform mat4 u_ModelViewProj;
uniform mat4 u_View;
uniform mat4 u_ViewProj;
uniform mat4 u_Normal;
uniform mat4 u_NormalView;
uniform mat4 u_NormalViewProj;

uniform mat4 u_InverseModel;
uniform mat4 u_InverseModelView;
uniform mat4 u_InverseModelViewProj;
uniform mat4 u_InverseView;
uniform mat4 u_InverseViewProj;

struct VertexPositionInputs {
    vec3 positionWorldSpace;
    vec3 positionViewSpace;
    vec4 positionClipSpace;
    vec4 positionNDC;
};

VertexPositionInputs GetVertexPositionInputs(vec3 positionObjectSpace) {
    VertexPositionInputs posInputs;
    posInputs.positionWorldSpace = (u_Model * vec4(positionObjectSpace, 1.0)).xyz;
    posInputs.positionViewSpace = (u_ModelView * vec4(positionObjectSpace, 1.0)).xyz;
    posInputs.positionClipSpace = u_ModelViewProj * vec4(positionObjectSpace, 1.0);

    vec4 ndc = posInputs.positionClipSpace * 0.5;
    // x = 1 or -1 (-1 if projection is flipped)
    // y = near plane
    // z = far plane
    // w = 1/far plane
    // float4 _ProjectionParams;
    float projectionParam = 1.0;  // TODO: get projection parameters
    posInputs.positionNDC.xy = vec2(ndc.x, ndc.y * projectionParam) + ndc.w;
    posInputs.positionNDC.zw = posInputs.positionClipSpace.zw;

    return posInputs;
}

struct VertexNormalInputs {
    vec3 normalWorldSpace;
    vec3 tangentWorldSpace;
    vec3 bitangentWorldSpace;
};

VertexNormalInputs GetVertexNormalInputs(vec3 normalObjectSpace) {
    VertexNormalInputs tbn;
    tbn.tangentWorldSpace = vec3(1.0, 0.0, 0.0);
    tbn.bitangentWorldSpace = vec3(0.0, 1.0, 0.0);
    tbn.normalWorldSpace = (u_Normal * vec4(normalObjectSpace, 1.0)).xyz;
    return tbn;
}

VertexNormalInputs GetVertexNormalInputs(vec3 normalObjectSpace, vec4 tangentObjectSpace) {
    VertexNormalInputs tbn;
    float sign = 1.0;  // TODO: calculate sign based on tangent handedness
    tbn.normalWorldSpace = (u_Normal * vec4(normalObjectSpace, 0.0)).xyz;
    tbn.tangentWorldSpace = (u_Model * vec4(tangentObjectSpace.xyz, 0.0)).xyz;
    tbn.bitangentWorldSpace = cross(tbn.normalWorldSpace, tbn.tangentWorldSpace) * sign;
    return tbn;
}

vec3 GetSurfaceNormalWorldSpace(vec3 normalWorldSpace, vec3 normalTangentSpace, mat3 tbnMatrix, bool usingNormalmap) {
    vec3 surfNormal = normalize(normalWorldSpace);
    if (usingNormalmap) {
        normalTangentSpace = normalTangentSpace * 2.0 - 1.0;
        surfNormal = normalize(tbnMatrix * normalTangentSpace);
    }
    return surfNormal;
}
