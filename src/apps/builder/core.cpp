#include "../../engine/core.h"

namespace Builder
{
    void run()
    {
        Javita::Rendeable *floor = Javita::Render::Primitives::createQuad();

        floor->color = glm::vec3(0.5f, 0.5f, 0.5f);
        floor->transform.position.y = -0.659f;
        floor->transform.rotation.x = -90.0f;
        floor->transform.scale = glm::vec3(10.0f, 10.0f, 1.0f);

        glm::vec3 point1 = glm::vec3(-1.0f, 0.0f, 0.0f);
        glm::vec3 point2 = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 point3 = glm::vec3(1.0f, 0.0f, 0.0f);

        Javita::Rendeable *line = Javita::Render::Primitives::createLine({point1, point2, point3});

        // Javita::addGuiCallback([floor]()
        //                        {
        //                            //For floor settings.
        //                            ImGui::Begin("Placing the floor");

        //                            ImGui::Text("Position");
        //                            ImGui::SliderFloat("Position X", &floor->transform.position.x, -10.0f, 10.0f);
        //                            ImGui::SliderFloat("Position Y", &floor->transform.position.y, -10.0f, 10.0f);
        //                            ImGui::SliderFloat("Position Z", &floor->transform.position.z, -10.0f, 10.0f);

        //                            ImGui::Text("Rotation");
        //                            ImGui::SliderFloat("Rotation X", &floor->transform.rotation.x, -180.0f, 180.0f);
        //                            ImGui::SliderFloat("Rotation Y", &floor->transform.rotation.y, -180.0f, 180.0f);
        //                            ImGui::SliderFloat("Rotation Z", &floor->transform.rotation.z, -180.0f, 180.0f);

        //                            ImGui::Text("Scale");
        //                            ImGui::SliderFloat("Scale X", &floor->transform.scale.x, 0.1f, 10.0f);
        //                            ImGui::SliderFloat("Scale Y", &floor->transform.scale.y, 0.1f, 10.0f);
        //                            ImGui::SliderFloat("Scale Z", &floor->transform.scale.z, 0.1f, 10.0f);

        //                            ImGui::End();
        //                        });

        Javita::addOnUpdateCallback([line, &point1, &point2, &point3](float deltaTime)
                                    {  
                                        point2.y += 0.1f * deltaTime;
                                        point3.x -= 0.1f * deltaTime;

                                        Javita::Render::Primitives::updateLine(line, {point1, point2, point3});
                                    });

        Javita::run();
    }
} // namespace Builder
