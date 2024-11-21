#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Color;
};

class VBO {
public:
  GLuint ID;

  VBO(std::vector<Vertex> &vertices);

  void Bind();
  void Unbind();
  void Delete();
};
