#include "Vertex.h"

#include <glm/mat4x4.hpp>


Vertex VertexShader(Vertex v, const glm::mat4& rotation)
{
    auto p = rotation * v.Position();

    glm::vec4 newPosition(
        p.x,
        p.y,
        0.0f,
        p.z + 4.0f
    );
    return Vertex(
        newPosition, // MVP * Position
        v.WorldCoordinates(), // M * Position
        v.Color(),
        v.TexCoord(),
        v.Normal() // *M normal * Normal
    );
}
