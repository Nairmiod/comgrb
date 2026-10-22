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
#include <memory>
#include <SDL3/SDL_log.h>

#include "consts.h"
#include "Tri.h"
#include "Vertex.h"

class Mesh {
    std::vector<std::shared_ptr<Vertex> > m_vertices;
    std::vector<std::shared_ptr<Tri> > m_tris;
    unsigned int m_id;

public:
   explicit Mesh(const unsigned int id = 0): m_id(id) {

    };

    ~Mesh() {
        SDL_Log("Mesh destroyed %i",m_id);
    }

    [[nodiscard]] std::vector<std::shared_ptr<Vertex> > &Vertices() noexcept {
        return m_vertices;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Tri> > &Tris() noexcept {
        return m_tris;
    }

    static void AddCube(std::vector<std::shared_ptr<Vertex> > &vertices, std::vector<std::shared_ptr<Tri> > &tris,
                        const glm::vec3 &colorFront, const glm::vec3 &colorBack,
                        const glm::vec3 &colorTop, const glm::vec3 &colorBottom,
                        const glm::vec3 &colorRight, const glm::vec3 &colorLeft) {
        //front
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, -1, -1), colorFront, glm::vec2(0, 0), -vec3::UnitZ));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, -1, -1), colorFront, glm::vec2(1, 0), -vec3::UnitZ));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, 1, -1), colorFront, glm::vec2(1, 1), -vec3::UnitZ));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, 1, -1), colorFront, glm::vec2(0, 1), -vec3::UnitZ));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));

        //back
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, -1, 1), colorBack, glm::vec2(0, 0), vec3::UnitZ));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, -1, 1), colorBack, glm::vec2(1, 0), vec3::UnitZ));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, 1, 1), colorBack, glm::vec2(1, 1), vec3::UnitZ));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, 1, 1), colorBack, glm::vec2(0, 1), vec3::UnitZ));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));

        //top
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, 1, -1), colorTop, glm::vec2(0, 1), vec3::UnitY));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, 1, -1), colorTop, glm::vec2(1, 1), vec3::UnitY));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, 1, 1), colorTop, glm::vec2(1, 0), vec3::UnitY));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, 1, 1), colorTop, glm::vec2(0, 0), vec3::UnitY));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));

        //bottom
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, -1, -1), colorBottom, glm::vec2(0, 1), -vec3::UnitY));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, -1, -1), colorBottom, glm::vec2(1, 1), -vec3::UnitY));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, -1, 1), colorBottom, glm::vec2(1, 0), -vec3::UnitY));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, -1, 1), colorBottom, glm::vec2(0, 0), -vec3::UnitY));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));

        //right
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, -1, -1), colorRight, glm::vec2(0, 1), vec3::UnitX));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, -1, 1), colorRight, glm::vec2(1, 1), vec3::UnitX));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, 1, 1), colorRight, glm::vec2(1, 0), vec3::UnitX));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(1, 1, -1), colorRight, glm::vec2(0, 0), vec3::UnitX));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));

        //left
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, -1, 1), colorLeft, glm::vec2(0, 1), -vec3::UnitX));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, -1, -1), colorLeft, glm::vec2(1, 1), -vec3::UnitX));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, 1, -1), colorLeft, glm::vec2(1, 0), -vec3::UnitX));
        vertices.push_back(std::make_shared<Vertex>(glm::vec3(-1, 1, 1), colorLeft, glm::vec2(0, 0), -vec3::UnitX));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
        tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));
    }

    static std::shared_ptr<Mesh> CreateCube(const glm::vec3 &colorFront, const glm::vec3 &colorBack,
                                            const glm::vec3 &colorTop, const glm::vec3 &colorBottom,
                                            const glm::vec3 &colorRight, const glm::vec3 &colorLeft) {
        auto m = std::make_shared<Mesh>();
        AddCube(m->m_vertices, m->m_tris, colorFront, colorBack, colorTop, colorBottom, colorRight, colorLeft);
        return m;
    }


    static glm::vec2 ToTex(glm::vec3 p) {
        return glm::vec2(static_cast<float>(std::atan2(p.y, p.x) / std::numbers::pi / 2) + 0.5f,
                         static_cast<float>(std::acos(p.z) / std::numbers::pi));
    }

    static void AddPatch(std::vector<std::shared_ptr<Vertex> > &vertices, std::vector<std::shared_ptr<Tri> > &tris,
                         glm::vec3 color, glm::vec3 p1,
                         glm::vec3 p2, glm::vec3 p3,
                         glm::vec3 p4,
                         bool flipNormal) {
        auto n1 = glm::normalize(p1);
        auto n2 = glm::normalize(p2);
        auto n3 = glm::normalize(p3);
        auto n4 = glm::normalize(p4);

        auto t1 = ToTex(n1);
        auto t2 = ToTex(n2);
        auto t3 = ToTex(n3);
        auto t4 = ToTex(n4);
        if (t1.x > 0.75f || t2.x > 0.75f || t3.x > 0.75f || t4.x > 0.75f) {
            if (t1.x < 0.25f)
                t1.x++;
            if (t2.x < 0.25f)
                t2.x++;
            if (t3.x < 0.25f)
                t3.x++;
            if (t4.x < 0.25f)
                t4.x++;
        }
        vertices.push_back(std::make_shared<Vertex>(n1, color, t1, n1));
        vertices.push_back(std::make_shared<Vertex>(n2, color, t2, n2));
        vertices.push_back(std::make_shared<Vertex>(n3, color, t3, n3));
        vertices.push_back(std::make_shared<Vertex>(n4, color, t4, n4));
        if (flipNormal) {
            tris.push_back(std::make_shared<Tri>(vertices.size() - 3, vertices.size() - 4, vertices.size() - 2));
            tris.push_back(std::make_shared<Tri>(vertices.size() - 2, vertices.size() - 4, vertices.size() - 1));
        } else {
            tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 3, vertices.size() - 2));
            tris.push_back(std::make_shared<Tri>(vertices.size() - 4, vertices.size() - 2, vertices.size() - 1));
        }
    }


    static void AddSphere(std::vector<std::shared_ptr<Vertex> > &vertices,
                          std::vector<std::shared_ptr<Tri> > &tris, const int patches, const glm::vec3 color) {
        for (auto x = 0; x < patches; x++)
            for (auto y = 0; y < patches; y++) {
                auto x1 = (x * 2.f / patches) - 1.f;
                auto x2 = ((x + 1) * 2.f / patches) - 1.f;
                auto y1 = (y * 2.f / patches) - 1.f;
                auto y2 = ((y + 1) * 2.f / patches) - 1.f;

                AddPatch(vertices, tris, color, glm::vec3(x1, y1, -1),
                         glm::vec3(x2, y1, -1),
                         glm::vec3(x2, y2, -1),
                         glm::vec3(x1, y2, -1), false);
                AddPatch(vertices, tris, color, glm::vec3(x1, y1, 1),
                         glm::vec3(x2, y1, 1),
                         glm::vec3(x2, y2, 1),
                         glm::vec3(x1, y2, 1), true);
                AddPatch(vertices, tris, color, glm::vec3(x1, -1, y1),
                         glm::vec3(x2, -1, y1),
                         glm::vec3(x2, -1, y2),
                         glm::vec3(x1, -1, y2), true);
                AddPatch(vertices, tris, color, glm::vec3(x1, 1, y1),
                         glm::vec3(x2, 1, y1),
                         glm::vec3(x2, 1, y2),
                         glm::vec3(x1, 1, y2), false);
                AddPatch(vertices, tris, color, glm::vec3(-1, x1, y1),
                         glm::vec3(-1, x2, y1),
                         glm::vec3(-1, x2, y2),
                         glm::vec3(-1, x1, y2), false);
                AddPatch(vertices, tris, color, glm::vec3(1, x1, y1),
                         glm::vec3(1, x2, y1),
                         glm::vec3(1, x2, y2),
                         glm::vec3(1, x1, y2), true);
            }
    }

    static std::shared_ptr<Mesh> CreateSphere(const int patches, const glm::vec3 color) {
        auto m = std::make_shared<Mesh>();
        AddSphere(m->m_vertices, m->m_tris, patches, color);
        return m;
    }
};
