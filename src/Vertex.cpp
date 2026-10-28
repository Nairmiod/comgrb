#include "Vertex.h"



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
