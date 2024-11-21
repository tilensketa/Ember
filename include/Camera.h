#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera {
public:
  Camera(int width, int height, glm::vec3 position);

  void updateMatrix();
  void Matrix(Shader &shader, const char *uniform);
  void Inputs(GLFWwindow *window, float ts);
  void OnResize(const glm::vec2 &newResolution);

  glm::vec3 &GetPosition() { return mPosition; }

private:
  glm::ivec2 mResolution;
  float mFOV;
  glm::vec3 mPosition;
  glm::vec3 mFront;
  glm::vec3 mUp;
  float mAspectRatio;
  glm::mat4 mMatrix;

  float mMoveSpeed = 5;
  float mRotateSpeed = 1;
  glm::vec2 mLastMousePosition;
};
