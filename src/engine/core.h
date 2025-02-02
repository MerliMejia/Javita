#ifndef __CORE_H__
#define __CORE_H__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "shader.h"

namespace Javita
{
    void run();
    void finish();

    struct RendeableObject
    {
        std::vector<float> &vertices;
        std::vector<unsigned int> &indices;
        Shader &shader;

        RendeableObject(std::vector<float> &vertices, std::vector<unsigned int> &indices, Shader &shader) : vertices(vertices), indices(indices), shader(shader) {}
    };

    namespace Render
    {
        Javita::RendeableObject createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices);
    } // namespace Render

} // namespace Javita

#endif // __CORE_H__