#include "Entity.h"

int Entity::sEntityCount = 0;

Entity::Entity(std::shared_ptr<Mesh> mesh, const glm::vec3 &position,
               const glm::quat &rotation, PxPhysics *physics, PxScene *scene,
               float mass)
    : id(sEntityCount++) {
  // Add RenderComponent
  RenderComponent renderComponent;
  renderComponent.mesh = mesh;
  addComponent(renderComponent);

  // Add TransformComponent
  TransformComponent transformComponent;
  transformComponent.position = position;
  transformComponent.rotation = rotation;
  addComponent(transformComponent);

  // Add PhysicsComponent
  if (physics && scene) {
    PxTransform pxTransform(
        PxVec3(position.x, position.y, position.z),
        PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
    if (!pxTransform.isValid()) {
      throw std::runtime_error("PxTransform is invalid!");
    }

    PxRigidDynamic *dynamicActor = physics->createRigidDynamic(pxTransform);
    if (!dynamicActor) {
      throw std::runtime_error("Failed to create RigidDynamic actor!");
    }

    PxMaterial *material = physics->createMaterial(0.5f, 0.5f, 0.6f);
    PxShape *shape =
        physics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *material);
    dynamicActor->attachShape(*shape);

    PxRigidBodyExt::updateMassAndInertia(*dynamicActor, mass);

    scene->addActor(*dynamicActor);

    PhysicsComponent physicsComponent;
    physicsComponent.actor = dynamicActor;
    addComponent(physicsComponent);
  }
}
