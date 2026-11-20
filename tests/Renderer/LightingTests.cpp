#include "Orion/Renderer/LightingMath.h"
#include "Orion/Renderer/MeshSource.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Catch::Approx;

TEST_CASE("Renderer/Lighting/NormalMatrix: a normal stays perpendicular to the surface under non-uniform scale",
          "[Renderer][Lighting]")
{
    const glm::vec3 tangent{1.0f, -1.0f, 0.0f};
    const glm::vec3 normal{1.0f, 1.0f, 0.0f};
    REQUIRE(glm::dot(normal, tangent) == Approx(0.0f));

    const glm::mat4 model = glm::scale(glm::mat4(1.0f), {2.0f, 1.0f, 1.0f});

    const glm::vec3 worldTangent = glm::mat3(model) * tangent;
    const glm::vec3 worldNormal  = Orion::NormalMatrix(model) * normal;
    // Transformed like the tangent, with the model matrix
    const glm::vec3 incorrectNormal = glm::mat3(model) * normal;

    // Tangent and normal are still perpendicular
    REQUIRE(glm::dot(worldNormal, worldTangent) == Approx(0.0f).margin(1e-5f));
    // The incorrect normal is no longer perpendicular to the tangent
    REQUIRE(std::abs(glm::dot(incorrectNormal, worldTangent)) > 1.0f);
}

TEST_CASE("Renderer/Lighting/NormalMatrix: for rotation and uniform scale it matches the model matrix direction",
          "[Renderer][Lighting]")
{
    const glm::mat4 model =
        glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f))) *
        glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
    const glm::vec3 normal = glm::normalize(glm::vec3(0.3f, 1.0f, -0.4f));

    const glm::vec3 viaNormalMatrix = glm::normalize(Orion::NormalMatrix(model) * normal);
    // Valid here: NormalMatrix is R/3 and the model matrix is 3R, same direction once normalized
    const glm::vec3 viaModel = glm::normalize(glm::mat3(model) * normal);

    REQUIRE(viaNormalMatrix.x == Approx(viaModel.x).margin(1e-5f));
    REQUIRE(viaNormalMatrix.y == Approx(viaModel.y).margin(1e-5f));
    REQUIRE(viaNormalMatrix.z == Approx(viaModel.z).margin(1e-5f));
}

TEST_CASE("Renderer/Lighting/NormalMatrix: ignores the translation", "[Renderer][Lighting]")
{
    const glm::mat4 scaled     = glm::scale(glm::mat4(1.0f), {2.0f, 3.0f, 4.0f});
    const glm::mat4 translated = glm::translate(glm::mat4(1.0f), {10.0f, -5.0f, 7.0f}) * scaled;

    const glm::mat3 a = Orion::NormalMatrix(scaled);
    const glm::mat3 b = Orion::NormalMatrix(translated);
    // Both matrices should be equal
    for (int column = 0; column < 3; ++column)
        for (int row = 0; row < 3; ++row)
            REQUIRE(a[column][row] == Approx(b[column][row]));
}
