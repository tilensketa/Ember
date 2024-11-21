#pragma once

// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>
#include <string>
#include <unordered_map>

#include "Mesh.h"

#include "PxPhysicsAPI.h"
using namespace physx;

struct TransformComponent {
  glm::vec3 position;
  glm::quat rotation;
};

struct RenderComponent {
  std::shared_ptr<Mesh> mesh;
};

struct PhysicsComponent {
  PxRigidDynamic *actor;
};

class Entity {
public:
  static int sEntityCount;

  Entity() : id(sEntityCount++) {}

  Entity(std::shared_ptr<Mesh> mesh, const glm::vec3 &position,
         const glm::quat &rotation, PxPhysics *physics, PxScene *scene,
         float mass = 1.0f);

  int getId() const { return id; }

  // Add and get components
  template <typename T> void addComponent(T component) {
    components[typeid(T).name()] = std::make_shared<T>(component);
  }

  template <typename T> std::shared_ptr<T> getComponent() {
    return std::static_pointer_cast<T>(components[typeid(T).name()]);
  }

private:
  int id;
  std::unordered_map<std::string, std::shared_ptr<void>> components;
};
