#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "World.h"

#include <memory>
#include <vector>

class Application {
public:
  Application(unsigned int width, unsigned int height, bool fullscreen);
  void Run();
  void Close();

private:
  void initWindow(unsigned int width, unsigned int height, bool fullscreen);
  void processInput();

  void initImGui();
  void renderImGui();

  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);

private:
  glm::vec2 mResolution;
  std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> mWindow;
  std::unique_ptr<Shader> mShader;
  std::unique_ptr<Camera> mCamera;

  std::unique_ptr<World> mWorld;

  std::shared_ptr<Mesh> mCubeMesh;
};
