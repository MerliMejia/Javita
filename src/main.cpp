#include "./engine/core.h"

int main()
{

    // Javita::RendeableObject triangle = Javita::Render::Primitives::createTriangle();
    Javita::RendeableObject quad = Javita::Render::Primitives::createQuad();

    Javita::run();
    return 0;
}