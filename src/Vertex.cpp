#include "Vertex.h"
#include <algorithm>
#include <cmath>

Vertex Vertex::VertexShader(const Vertex& v, const glm::mat4& mvp, const glm::mat4& model)
{
    glm::vec4 newPosition = mvp * v.Position();
    glm::vec3 newWorldCoordinates = glm::vec3(model * v.Position());

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
    glm::vec3 newNormal = glm::normalize(normalMatrix * v.Normal());

    return Vertex(
        newPosition,
        newWorldCoordinates,
        v.Color(),
        v.TexCoord(),
        newNormal
    );
}

glm::vec3 Vertex::FragmentShader(const Vertex& v, const glm::vec3& cameraPos, const glm::vec3& lightPos, const glm::vec3& lightColor)
{
    // Ambient component
    constexpr float ambientStrength = 0.15f;
    glm::vec3 ambient = ambientStrength * v.Color();

    // Normal and direction vectors in World Space
    glm::vec3 N = glm::normalize(v.Normal());
    glm::vec3 L = glm::normalize(lightPos - v.WorldCoordinates());
    glm::vec3 V = glm::normalize(cameraPos - v.WorldCoordinates());

    // Diffuse component (Lambertian)
    float diff = std::max(glm::dot(N, L), 0.0f);
    glm::vec3 diffuse = diff * v.Color() * lightColor;

    // Specular component (Blinn-Phong)
    glm::vec3 H = glm::normalize(L + V);
    float spec = 0.0f;
    if (diff > 0.0f) {
        constexpr float shininess = 32.0f;
        spec = std::pow(std::max(glm::dot(N, H), 0.0f), shininess);
    }
    constexpr float specularStrength = 0.7f;
    glm::vec3 specular = specularStrength * spec * lightColor;

    // Combined shaded color clamped to [0, 1]
    return glm::clamp(ambient + diffuse + specular, 0.0f, 1.0f);
}
