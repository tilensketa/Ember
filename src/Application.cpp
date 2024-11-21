#include <glad/glad.h>

#include "Application.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <iostream>
#include <unistd.h>

Application::Application(unsigned int width, unsigned int height,
                         bool fullscreen)
    : mWindow(nullptr, glfwDestroyWindow) {
  initWindow(width, height, fullscreen);
  mResolution = glm::vec2(width, height);
  mShader =
      std::make_unique<Shader>("../shaders/vert.glsl", "../shaders/frag.glsl");
  mCamera =
      std::make_unique<Camera>(width, height, glm::vec3(0.0f, 1.0f, 2.0f));
  mWorld = std::make_unique<World>();

  initImGui();

  mCubeMesh = std::make_shared<Mesh>(Mesh::CreateCube(1.0f));

  // Entity 1
  Entity cubeEntity1(mCubeMesh, glm::vec3(0.0f, 0.0f, 2.0f),
                     glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                     mWorld->GetPhysicsSystem()->GetPhysics(),
                     mWorld->GetPhysicsSystem()->GetScene(), 10.0f);

  // Entity 2
  Entity cubeEntity2(
      mCubeMesh, glm::vec3(1.0f, 2.0f, 5.0f),
      glm::angleAxis(glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
      mWorld->GetPhysicsSystem()->GetPhysics(),
      mWorld->GetPhysicsSystem()->GetScene(), 5.0f);

  Entity cubeEntity3(
      mCubeMesh, glm::vec3(2.0f, 2.0f, 5.0f),
      glm::angleAxis(glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
      mWorld->GetPhysicsSystem()->GetPhysics(),
      mWorld->GetPhysicsSystem()->GetScene(), 5.0f);

  // Add entities to the world
  mWorld->AddEntity(cubeEntity1);
  mWorld->AddEntity(cubeEntity2);
  mWorld->AddEntity(cubeEntity3);
}

void Application::initWindow(unsigned int width, unsigned int height,
                             bool fullscreen) {
  std::cout << "----------CREATING WINDOW----------" << std::endl;
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    assert(false);
  }
  std::cout << "Successfully initialized GLFW" << std::endl;

  // Set GLFW window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFW window with a unique_ptr
  mWindow.reset(glfwCreateWindow(width, height, "EMBER", nullptr, nullptr));
  if (!mWindow) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    assert(false);
  }
  std::cout << "Successfully created GLFW window" << std::endl;

  // Make the context of the specified window current on the calling thread
  glfwMakeContextCurrent(mWindow.get());

  // Set the framebuffer size callback
  glfwSetWindowSizeCallback(mWindow.get(), framebuffer_size_callback);

  gladLoadGL();

  // Maximize the window if requested
  if (fullscreen)
    glfwMaximizeWindow(mWindow.get());

  // Set viewport size and enable depth testing
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);

  // Enable blending once during initialization
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::cout << "Successfully initialized GLAD" << std::endl;
  std::cout << "----------WINDOW CREATED----------" << std::endl;
}

void Application::initImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(mWindow.get(), true);
  ImGui_ImplOpenGL3_Init("#version 430");
}

void Application::Run() {
  std::cout << "Application Run" << std::endl;

  float ts = 0.0f;

  // Render loop
  while (!glfwWindowShouldClose(mWindow.get())) {
    int width, height;
    glfwGetWindowSize(mWindow.get(), &width, &height);
    mResolution = glm::vec2(width, height);
    mCamera->OnResize(mResolution);

    processInput();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ts = 1.0f / ImGui::GetIO().Framerate;
    mCamera->Inputs(mWindow.get(), ts);
    mCamera->updateMatrix();

    ts = 1.0f / 60.0f;
    // std::cout << ts << std::endl;
    mWorld->Update(ts, *mShader, *mCamera);
    renderImGui();

    glfwSwapBuffers(mWindow.get());
    glfwPollEvents();
  }
}

void Application::renderImGui() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  {
    ImGui::Begin("INFO");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Entities count: %i", mWorld->GetEntitiesCount());
    ImGui::End();
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::processInput() {
  if (glfwGetKey(mWindow.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwGetKey(mWindow.get(), GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(mWindow.get(), true);
  if (glfwGetKey(mWindow.get(), GLFW_KEY_A) == GLFW_PRESS) {
    Entity cubeEntity4(
        mCubeMesh, glm::vec3(2.0f, 2.0f, 5.0f),
        glm::angleAxis(glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        mWorld->GetPhysicsSystem()->GetPhysics(),
        mWorld->GetPhysicsSystem()->GetScene(), 5.0f);
    mWorld->AddEntity(cubeEntity4);
  }
}

void Application::framebuffer_size_callback(GLFWwindow *window, int newWidth,
                                            int newHeight) {
  glViewport(0, 0, newWidth, newHeight);
}

void Application::Close() {
  std::cout << "Application Close" << std::endl;
  glfwTerminate();
}
