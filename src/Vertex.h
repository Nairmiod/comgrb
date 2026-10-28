/*
* Copyright (c) 2013 - 2025 Simon Felix
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *  Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *  Neither the name of FHNW nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Translated to CPP by Fabio Kaufmann
 */
#pragma once
#include "glm/common.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include <ostream>

class Vertex {
    glm::vec4 m_position;
    glm::vec3 m_worldCoordinates;
    glm::vec3 m_color;
    glm::vec2 m_texCoord;
    glm::vec3 m_normal;

public:
    Vertex VertexShader(Vertex v, const glm::mat4x4& rotation);


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

Vertex VertexShader(Vertex v,const glm::mat4x4& rotation);
