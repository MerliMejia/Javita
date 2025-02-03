#include "./engine/core.h"

int main()
{
    Javita::RendeableObject rendeableObject = Javita::Render::createRendeableObject({
                                                                                        0.5f, 0.5f, 0.0f,   // top right
                                                                                        0.5f, -0.5f, 0.0f,  // bottom right
                                                                                        -0.5f, -0.5f, 0.0f, // bottom left
                                                                                        -0.5f, 0.5f, 0.0f   // top left
                                                                                    },
                                                                                    {
                                                                                        // note that we start from 0!
                                                                                        0, 1, 3, // first triangle
                                                                                        1, 2, 3  // second triangle
                                                                                    });

    Javita::run();
    return 0;
}