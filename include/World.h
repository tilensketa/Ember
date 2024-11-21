#include "PhysicsSystem.h"
#include "RenderSystem.h"

class World {
public:
  void AddEntity(Entity entity) { entities.push_back(entity); }

  void Update(float deltaTime, Shader &shader, Camera &camera) {
    mPhysicsSystem.update(deltaTime, entities);
    mRenderSystem.render(shader, camera, entities);
    // std::cout << "Entities: " << entities.size() << std::endl;
  }

  PhysicsSystem *GetPhysicsSystem() { return &mPhysicsSystem; }

  int GetEntitiesCount() { return entities.size(); }

private:
  std::vector<Entity> entities;
  PhysicsSystem mPhysicsSystem;
  RenderSystem mRenderSystem;
};
