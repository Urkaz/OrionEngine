#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_WorldPosition;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;
out vec2 v_TexCoord;

void main()
{
    vec4 worldPosition = u_Transform * vec4(a_Position, 1.0);

    v_WorldPosition = worldPosition.xyz;
    v_Normal        = u_NormalMatrix * a_Normal;
    v_Tangent       = mat3(u_Transform) * a_Tangent;
    v_Bitangent     = mat3(u_Transform) * a_Bitangent;
    v_TexCoord      = a_TexCoord;

    gl_Position = u_ViewProjection * worldPosition;
}

#type fragment
#version 450 core

in vec3 v_WorldPosition;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;
in vec2 v_TexCoord;

uniform sampler2D u_Albedo;    // unit 0, sRGB texture: the GPU returns LINEAR values
uniform sampler2D u_NormalMap; // unit 1, linear data
uniform int u_HasNormalMap;

uniform vec3 u_LightDirection; // direction the light TRAVELS (from the light towards the scene)
uniform vec3 u_LightColor;     // color * intensity, linear
uniform vec3 u_Ambient;        // linear
uniform vec3 u_CameraPosition; // world space
uniform float u_Shininess;
uniform float u_SpecularStrength;

// 0 lit, 1 albedo, 2 diffuse, 3 specular, 4 geometric normal, 5 shading normal (with normal map)
uniform int u_ViewMode;

layout(location = 0) out vec4 o_Color;

// World-space normal used for lighting: the interpolated vertex normal, or the normal map one
vec3 GetShadingNormal()
{
    vec3 N = normalize(v_Normal);
    if (u_HasNormalMap == 0)
        return N;

    vec3 T = normalize(v_Tangent);
    T      = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);
    if (dot(B, v_Bitangent) < 0.0)
        B = -B;

    vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).xyz * 2.0 - 1.0;
    return normalize(mat3(T, B, N) * tangentNormal);
}

void main()
{
    vec4 albedoSample = texture(u_Albedo, v_TexCoord);
    if (albedoSample.a < 0.5)
        discard; // alpha-tested surfaces (foliage, chains, grilles)
    vec3 albedo = albedoSample.rgb;

    vec3 N = GetShadingNormal();

    vec3 L = normalize(-u_LightDirection);
    vec3 V = normalize(u_CameraPosition - v_WorldPosition);
    vec3 H = normalize(L + V);

    float NdotL  = max(dot(N, L), 0.0);
    vec3 diffuse = albedo * u_LightColor * NdotL;

    // No highlight on faces the light does not reach
    float specularTerm = NdotL > 0.0 ? pow(max(dot(N, H), 0.0), u_Shininess) : 0.0;
    vec3 specular      = u_LightColor * u_SpecularStrength * specularTerm;

    vec3 ambient = albedo * u_Ambient;

    // --- view modes (given) ---
    vec3 color;
    if (u_ViewMode == 0)
        color = ambient + diffuse + specular;
    else if (u_ViewMode == 1)
        color = albedo;
    else if (u_ViewMode == 2)
        color = diffuse;
    else if (u_ViewMode == 3)
        color = specular;
    else if (u_ViewMode == 4)
        color = normalize(v_Normal) * 0.5 + 0.5; // already a "color", not light: no gamma
    else
        color = N * 0.5 + 0.5;

    if (u_ViewMode <= 3)
        color = pow(color, vec3(1.0 / 2.2)); // temporary linear -> sRGB

    o_Color = vec4(color, 1.0);
}
