#include "Entity.h"
#include <glm/glm.hpp>
#include <vector>

class RenderSystem {
public:
  void render(Shader &shader, Camera &camera, std::vector<Entity> &entities) {
    // std::cout << "Renderer System - update" << std::endl;
    for (auto &entity : entities) {
      auto renderComp = entity.getComponent<RenderComponent>();
      auto transformComp = entity.getComponent<TransformComponent>();

      if (renderComp && transformComp) {
        renderComp->mesh->SetTransform(transformComp->position,
                                       transformComp->rotation);
        renderComp->mesh->Draw(shader, camera, GL_TRIANGLES);
        /* std::cout << transformComp->position.x << ","
                  << transformComp->position.y << ", "
                  << transformComp->position.z << std::endl; */
      }
    }
  }
};
