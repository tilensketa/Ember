#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
  mResolution = glm::ivec2(width, height);
  mFOV = 45.0f;
  mPosition = glm::vec3(4, 2, 4);
  mFront = glm::vec3(-1, -0.5, -1);
  mUp = glm::vec3(0, 1, 0);
  mAspectRatio = (float)mResolution.x / (float)mResolution.y;
}

void Camera::updateMatrix() {
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  view = glm::lookAt(mPosition, mPosition + mFront, mUp);
  float mNearPlane = 0.1f;
  float mFarPlane = 100.0f;
  projection =
      glm::perspective(glm::radians(mFOV), (float)mResolution.x / mResolution.y,
                       mNearPlane, mFarPlane);

  mMatrix = projection * view;
}

void Camera::Matrix(Shader &shader, const char *uniform) {
  shader.setMat4(uniform, mMatrix);
}

void Camera::Inputs(GLFWwindow *window, float ts) {

  double x;
  double y;
  glfwGetCursorPos(window, &x, &y);
  glm::vec2 mousePos(x, y);
  glm::vec2 delta = (mousePos - mLastMousePosition) * 0.002f;
  mLastMousePosition = mousePos;

  if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glm::vec3 right = glm::cross(mFront, mUp);

  // Position
  if (glfwGetKey(window, GLFW_KEY_W)) {
    mPosition += mFront * mMoveSpeed * ts;
  } else if (glfwGetKey(window, GLFW_KEY_S)) {
    mPosition -= mFront * mMoveSpeed * ts;
  }

  if (glfwGetKey(window, GLFW_KEY_A)) {
    mPosition -= right * mMoveSpeed * ts;
  } else if (glfwGetKey(window, GLFW_KEY_D)) {
    mPosition += right * mMoveSpeed * ts;
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE)) {
    mPosition += mUp * mMoveSpeed * ts;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
    mPosition -= mUp * mMoveSpeed * ts;
  }

  // Rotation
  if (delta.x != 0.0f || delta.y != 0.0f) {
    float pitchDelta = delta.y * mRotateSpeed;
    float yawDelta = delta.x * mRotateSpeed;

    glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, right),
                                            glm::angleAxis(-yawDelta, mUp)));
    mFront = glm::rotate(q, mFront);
    updateMatrix();
  }
}

void Camera::OnResize(const glm::vec2 &newResolution) {
  mResolution = newResolution;
  updateMatrix();
}
