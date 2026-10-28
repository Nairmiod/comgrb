#pragma once
#include "glm/common.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <glm/mat4x4.hpp>
#include <ostream>

class Vertex {
    glm::vec4 m_position;
    glm::vec3 m_worldCoordinates;
    glm::vec3 m_color;
    glm::vec2 m_texCoord;
    glm::vec3 m_normal;

public:
    static Vertex VertexShader(const Vertex& v, const glm::mat4& mvp, const glm::mat4& model);


    Vertex(glm::vec3 position, glm::vec3 color, glm::vec2 texCoord, glm::vec3 normal) : m_position(glm::vec4(
            position, 1.0f)),
        m_worldCoordinates(glm::vec3{0.0f, 0.0f, 0.0f}),
        m_color(color),
        m_texCoord(texCoord),
        m_normal(normal) {
    }

    Vertex(glm::vec4 position, glm::vec3 worldCoordinates, glm::vec3 color, glm::vec2 texCoord,
           glm::vec3 normal) : m_position(position),
                               m_worldCoordinates(worldCoordinates),
                               m_color(color),
                               m_texCoord(texCoord),
                               m_normal(normal) {
    }

    [[nodiscard]] const glm::vec4 &Position() const noexcept {
        return m_position;
    }

    [[nodiscard]] const glm::vec3 &WorldCoordinates() const noexcept {
        return m_worldCoordinates;
    }

    [[nodiscard]] const glm::vec3 &Color() const noexcept {
        return m_color;
    }

    [[nodiscard]] const glm::vec2 &TexCoord() const noexcept {
        return m_texCoord;
    }

    [[nodiscard]] const glm::vec3 &Normal() const noexcept {
        return m_normal;
    }

    friend Vertex operator*(const Vertex &v, const glm::mat4 &m) {
        return {
            m * v.m_position,
            m * glm::vec4(v.m_worldCoordinates, 1.0f),
            v.m_color,
            v.m_texCoord,
            m * glm::vec4(v.m_normal, 0.0f)
        };
    }

    static Vertex Lerp(const Vertex &a, const Vertex &b, const float &t) noexcept {
        return {
            glm::mix(a.m_position, b.m_position, t), glm::mix(a.m_worldCoordinates, b.m_worldCoordinates, t),
            glm::mix(a.m_color, b.m_color, t),
            glm::mix(a.m_texCoord, b.m_texCoord, t), glm::mix(a.m_normal, b.m_normal, t)
        };
    }

    friend Vertex operator*(const Vertex &v, const float x) {
        return {
            v.m_position * x,
            v.m_worldCoordinates * x,
            v.m_color * x,
            v.m_texCoord * x,
            v.m_normal * x,
        };
    }

    friend Vertex operator*(const float x, const Vertex &v) {
        return v * x;
    }

    friend Vertex operator+(const Vertex &a, const Vertex &b) {
        return {
            a.m_position + b.m_position,
            a.m_worldCoordinates + b.m_worldCoordinates,
            a.m_color + b.m_color,
            a.m_texCoord + b.m_texCoord,
            a.m_normal + b.m_normal
        };
    }

    friend Vertex operator-(const Vertex &a, const Vertex &b) {
        return {
            a.m_position - b.m_position,
            a.m_worldCoordinates - b.m_worldCoordinates,
            a.m_color - b.m_color,
            a.m_texCoord - b.m_texCoord,
            a.m_normal - b.m_normal
        };
    }

    friend std::ostream &operator<<(std::ostream &os, const Vertex &v) {
        return os << "(" << v.Position().x << "," << v.Position().y << "," << v.Position().z << "," << v.Position().w <<
               ")" << std::endl;
    }

};
