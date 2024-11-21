#include "Entity.h"
#include "PxPhysicsAPI.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

using namespace physx;

struct PxFoundationDeleter {
  void operator()(PxFoundation *foundation) const {
    if (foundation) {
      foundation->release();
    }
  }
};

struct PxPhysicsDeleter {
  void operator()(PxPhysics *physics) const {
    if (physics) {
      physics->release();
    }
  }
};

struct PxSceneDeleter {
  void operator()(PxScene *scene) const {
    if (scene) {
      scene->release();
    }
  }
};

struct PxMaterialDeleter {
  void operator()(PxMaterial *material) const {
    if (material) {
      material->release();
    }
  }
};

class PhysicsSystem {
public:
  PhysicsSystem() {
    // Create foundation
    mFoundation = std::unique_ptr<PxFoundation, PxFoundationDeleter>(
        PxCreateFoundation(PX_PHYSICS_VERSION, mAllocator, mErrorCallback));
    if (!mFoundation) {
      throw std::runtime_error("Failed to create PhysX Foundation.");
    }

    // Create physics
    mPhysics = std::unique_ptr<PxPhysics, PxPhysicsDeleter>(PxCreatePhysics(
        PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true));
    if (!mPhysics) {
      throw std::runtime_error("Failed to create PhysX Physics.");
    }

    // Create CPU dispatcher
    mDispatcher = std::unique_ptr<PxDefaultCpuDispatcher>(
        PxDefaultCpuDispatcherCreate(2));
    if (!mDispatcher) {
      throw std::runtime_error("Failed to create CPU dispatcher.");
    }

    // Create scene
    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, 0.0f, -9.81f);
    sceneDesc.cpuDispatcher = mDispatcher.get();
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    mScene = std::unique_ptr<PxScene, PxSceneDeleter>(
        mPhysics->createScene(sceneDesc));
    if (!mScene) {
      throw std::runtime_error("Failed to create PhysX Scene.");
    }

    // Create ground plane
    createGroundPlane();
  }

  ~PhysicsSystem() = default;

  void update(float deltaTime, std::vector<Entity> &entities) {
    // std::cout << "PhysicsSystem - update" << std::endl;
    // Step the simulation
    mScene->simulate(deltaTime);
    mScene->fetchResults(true);

    // Update entities with physics components
    for (auto &entity : entities) {
      auto transformComp = entity.getComponent<TransformComponent>();
      auto physicsComp = entity.getComponent<PhysicsComponent>();

      if (transformComp && physicsComp) {
        PxRigidDynamic *actor = physicsComp->actor;
        if (actor) {
          PxTransform pose = actor->getGlobalPose();
          transformComp->position = glm::vec3(pose.p.x, pose.p.y, pose.p.z);
          transformComp->rotation =
              glm::quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z);
          // std::cout << pose.p.x << ", " << pose.p.y << ", " << pose.p.z << std::endl;
        }
      }
    }
  }

  PxPhysics *GetPhysics() { return mPhysics.get(); }
  PxScene *GetScene() { return mScene.get(); }

private:
  void createGroundPlane() {
    // Create material for the ground
    auto groundMaterial = std::unique_ptr<PxMaterial, PxMaterialDeleter>(
        mPhysics->createMaterial(0.5f, 0.5f, 0.6f));
    if (!groundMaterial) {
      throw std::runtime_error("Failed to create ground material.");
    }

    // Create ground plane
    PxRigidStatic *groundPlane =
        PxCreatePlane(*mPhysics, PxPlane(0, 0, 1, 0), *groundMaterial);
    if (!groundPlane) {
      throw std::runtime_error("Failed to create ground plane.");
    }

    mScene->addActor(*groundPlane);
  }

  std::unique_ptr<PxFoundation, PxFoundationDeleter> mFoundation;
  std::unique_ptr<PxPhysics, PxPhysicsDeleter> mPhysics;
  std::unique_ptr<PxScene, PxSceneDeleter> mScene;
  std::unique_ptr<PxDefaultCpuDispatcher> mDispatcher;

  PxDefaultAllocator mAllocator;
  PxDefaultErrorCallback mErrorCallback;
};
