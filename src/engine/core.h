#ifndef __CORE_H__
#define __CORE_H__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <deque>

#include "shader.h"

namespace Javita
{
    void run();
    void finish();
    void addOnUpdateCallback(std::function<void(float)>);
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

    enum class RenderMode
    {
        TRIANGLES,
        LINES
    };

    struct Rendeable
    {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Shader shader;
        Javita::Transform transform = Javita::Transform();
        glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.2f);
        RenderMode renderMode = RenderMode::TRIANGLES;

        void init();

        void draw()
        {
            glBindVertexArray(VAO);
            if (renderMode == Javita::RenderMode::TRIANGLES)
            {
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            }
            else
            {
                glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
            }
        }
    };

    namespace Render
    {
        Rendeable *createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices);
        Rendeable *createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec3 color);

        namespace Primitives
        {
            Rendeable *createTriangle();
            Rendeable *createQuad();
            Rendeable *createLine(std::vector<glm::vec3> points);
        } // namespace Primitives

    } // namespace Render

} // namespace Javita

#endif // __CORE_H__