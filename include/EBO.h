#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class EBO {
public:
  GLuint ID;

  EBO(std::vector<GLuint>& indices);

  void Bind();
  void Unbind();
  void Delete();
};
