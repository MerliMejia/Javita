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

        Javita::addGuiCallback([floor]()
                               {
                                   // For floor settings.
                                   // ImGui::Begin("Placing the floor");

                                   // ImGui::Text("Position");
                                   // ImGui::SliderFloat("Position X", &floor->transform.position.x, -10.0f, 10.0f);
                                   // ImGui::SliderFloat("Position Y", &floor->transform.position.y, -10.0f, 10.0f);
                                   // ImGui::SliderFloat("Position Z", &floor->transform.position.z, -10.0f, 10.0f);

                                   // ImGui::Text("Rotation");
                                   // ImGui::SliderFloat("Rotation X", &floor->transform.rotation.x, -180.0f, 180.0f);
                                   // ImGui::SliderFloat("Rotation Y", &floor->transform.rotation.y, -180.0f, 180.0f);
                                   // ImGui::SliderFloat("Rotation Z", &floor->transform.rotation.z, -180.0f, 180.0f);

                                   // ImGui::Text("Scale");
                                   // ImGui::SliderFloat("Scale X", &floor->transform.scale.x, 0.1f, 10.0f);
                                   // ImGui::SliderFloat("Scale Y", &floor->transform.scale.y, 0.1f, 10.0f);
                                   // ImGui::SliderFloat("Scale Z", &floor->transform.scale.z, 0.1f, 10.0f);

                                   // ImGui::End();
                               });

        Javita::run();
    }
} // namespace Builder
