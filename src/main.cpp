#include "./engine/core.h"

int main()
{

  Javita::RendeableObject triangle = Javita::Render::Primitives::createTriangle();
  Javita::RendeableObject triangle2 = Javita::Render::Primitives::createTriangle();
  Javita::RendeableObject quad = Javita::Render::Primitives::createQuad();
  Javita::RendeableObject quad2 = Javita::Render::Primitives::createQuad();

  triangle.transform.position.x = -0.5f;
  quad.transform.position.x = 0.5f;
  triangle2.transform.position.x = -0.8f;
  quad2.transform.position.x = 0.8f;

  triangle.color = glm::vec3(1.0f, 0.6f, 0.8f);
  quad.color = glm::vec3(0.8f, 1.0f, 0.0f);
  triangle2.color = glm::vec3(0.3f, 1.0f, 0.3f);
  quad2.color = glm::vec3(0.1f, 0.6f, 1.0f);

  Javita::addOnUpdateCallback([&quad, &triangle, &quad2, &triangle2](float deltaTime)
                              {
                                quad.transform.rotation.y += 100.0f * deltaTime;
                                triangle.transform.rotation.x += 100.0f * deltaTime;
                                quad2.transform.rotation.z += 100.0f * deltaTime;
                                triangle2.transform.rotation.y += 100.0f * deltaTime;
                                triangle2.transform.rotation.x += 100.0f * deltaTime;
                                triangle2.transform.rotation.z += 100.0f * deltaTime; });

  Javita::run();
  return 0;
}