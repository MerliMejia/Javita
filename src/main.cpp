#include "./engine/core.h"

int main()
{

    Javita::RendeableObject triangle = Javita::Render::Primitives::createTriangle();
    Javita::RendeableObject quad = Javita::Render::Primitives::createQuad();

    triangle.transform.position.x = -0.5f;
    quad.transform.position.x = 0.5f;

    Javita::addOnUpdateCallback([&quad, &triangle](float deltaTime)
                                { quad.transform.rotation.y += 100.0f * deltaTime;
                                  triangle.transform.rotation.y += 100.0f * deltaTime; });

    Javita::run();
    return 0;
}