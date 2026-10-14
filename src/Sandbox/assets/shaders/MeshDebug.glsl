#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;
out vec2 v_TexCoord;

void main()
{
    // Rotating the directions with the model matrix is only correct while the scale is uniform.
    v_Normal    = mat3(u_Transform) * a_Normal;
    v_Tangent   = mat3(u_Transform) * a_Tangent;
    v_Bitangent = mat3(u_Transform) * a_Bitangent;
    v_TexCoord  = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;
out vec2 v_TexCoord;

void main()
{
    // Rotating the directions with the model matrix is only correct while the scale is uniform.
    // Lesson 3 explains the normal matrix that fixes it.
    v_Normal    = mat3(u_Transform) * a_Normal;
    v_Tangent   = mat3(u_Transform) * a_Tangent;
    v_Bitangent = mat3(u_Transform) * a_Bitangent;
    v_TexCoord  = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;
in vec2 v_TexCoord;

uniform int u_DebugMode; // 0 normal, 1 tangent, 2 bitangent, 3 UV, 4 submesh index
uniform int u_SubmeshIndex;

layout(location = 0) out vec4 o_Color;

// A distinct, stable colour per index (golden-ratio steps around a cosine palette)
vec3 IndexToColor(int index)
{
    return 0.5 + 0.5 * cos(6.28318 * (float(index) * 0.61803398875 + vec3(0.0, 0.33, 0.67)));
}

// Direction -> color. A missing attribute (zero vector) shows as flat grey instead of NaN
vec3 DirectionToColor(vec3 v)
{
    return length(v) > 0.0001 ? normalize(v) * 0.5 + 0.5 : vec3(0.5);
}

void main()
{
    vec3 color;
    if (u_DebugMode == 0)
        color = DirectionToColor(v_Normal);
    else if (u_DebugMode == 1)
        color = DirectionToColor(v_Tangent);
    else if (u_DebugMode == 2)
        color = DirectionToColor(v_Bitangent);
    else if (u_DebugMode == 3)
        color = vec3(fract(v_TexCoord), 0.0);
    else
        color = IndexToColor(u_SubmeshIndex);

    o_Color = vec4(color, 1.0);
}
