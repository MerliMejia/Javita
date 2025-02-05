#include "../engine/core.h"

namespace PrimitivesExample
{
    int run()
    {
        Javita::Rendeable *triangle = Javita::Render::Primitives::createTriangle();
        Javita::Rendeable *triangle2 = Javita::Render::Primitives::createTriangle();
        Javita::Rendeable *quad = Javita::Render::Primitives::createQuad();
        Javita::Rendeable *quad2 = Javita::Render::Primitives::createQuad();

        triangle->transform.position = glm::vec3(1.0f, 0.0f, 0.0f);
        triangle2->transform.position = glm::vec3(-1.0f, 0.0f, 0.0f);
        quad->transform.position = glm::vec3(0.0f, 1.0f, 0.0f);
        quad2->transform.position = glm::vec3(0.0f, -1.0f, 0.0f);

        std::vector<glm::vec3> points = {triangle->transform.position, triangle2->transform.position,
                                         quad->transform.position, quad2->transform.position};

        Javita::Rendeable *line = Javita::Render::Primitives::createLine(points);

        Javita::Rendeable *cube = Javita::Render::Primitives::createCube();

        cube->transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);

        triangle->color = glm::vec3(1.0f, 0.0f, 0.0f);
        triangle2->color = glm::vec3(0.0f, 1.0f, 0.0f);
        quad->color = glm::vec3(0.0f, 0.0f, 1.0f);
        quad2->color = glm::vec3(1.0f, 1.0f, 0.0f);
        line->color = glm::vec3(1.0f, 1.0f, 1.0f);
        cube->color = glm::vec3(1.0f, 0.0f, 1.0f);

        Javita::addOnUpdateCallback([triangle, triangle2, quad, quad2, cube](float deltaTime)
                                    { triangle->transform.rotation.x += 100.0f * deltaTime;
                                triangle2->transform.rotation.y += 100.0f * deltaTime;
                                quad->transform.rotation.z += 100.0f * deltaTime;
                                quad2->transform.rotation.z += 100.0f * deltaTime;
                                quad2->transform.rotation.y += 100.0f * deltaTime;
                                quad2->transform.rotation.x += 100.0f * deltaTime;
                                cube->transform.rotation.z += 100.0f * deltaTime;
                                cube->transform.rotation.y += 100.0f * deltaTime;
                                cube->transform.rotation.x += 100.0f * deltaTime; });

        Javita::run();
        return 0;
    }
} // namespace PrimitivesExample
