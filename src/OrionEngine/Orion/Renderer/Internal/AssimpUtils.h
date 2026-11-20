#pragma once

#include <assimp/matrix4x4.h>
#include <assimp/matrix4x4.inl>
#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <span>

namespace Orion::Internal
{
    inline glm::vec3 ToVec3(const aiVector3D& v)
    {
        return {v.x, v.y, v.z};
    }

    // Wraps an Assimp raw array + count. A missing attribute (nullptr) gives an empty span
    template <typename T>
    std::span<T> MakeSpan(T* data, unsigned int count)
    {
        return data ? std::span<T>(data, count) : std::span<T>();
    }

    // aiMatrix4x4 is row-major (a1..a4 is the first ROW; translation is a4, b4, c4).
    // glm::mat4 is column-major (m[column][row]; translation is m[3]).
    inline glm::mat4 Mat4FromAssimp(const aiMatrix4x4& m)
    {
        // clang-format off
        return glm::mat4(m.a1, m.b1, m.c1, m.d1,
                         m.a2, m.b2, m.c2, m.d2,
                         m.a3, m.b3, m.c3, m.d3,
                         m.a4, m.b4, m.c4, m.d4);
        // clang-format on
    }
} // namespace Orion::Internal
