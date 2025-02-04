#ifndef __CORE_H__
#define __CORE_H__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "shader.h"

namespace Javita
{
    void run();
    void finish();

    struct Transform
    {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 getModelMatrix()
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, scale);
            return model;
        }
    };

    struct RendeableObject
    {
        std::vector<float> &vertices;
        std::vector<unsigned int> &indices;
        Shader &shader;
        Transform &transform;

        RendeableObject(std::vector<float> &vertices, std::vector<unsigned int> &indices, Shader &shader,
                        Transform &transform) : vertices(vertices), indices(indices), shader(shader), transform(transform) {}
    };

    namespace Render
    {
        Javita::RendeableObject createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices);

        namespace Primitives
        {
            Javita::RendeableObject createTriangle();
            Javita::RendeableObject createQuad();
        } // namespace Primitives

    } // namespace Render

} // namespace Javita

#endif // __CORE_H__