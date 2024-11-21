#pragma once

#include <string>

#include "Camera.h"
#include "EBO.h"
#include "VAO.h"

class Mesh {
public:
  Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices);

  static Mesh CreateCube(float size);

  // Draws the mesh
  void Draw(Shader &shader, Camera &camera, GLuint mode);

  // Setters for position, rotation, and scale
  void SetTransform(const glm::mat4 &mat);
  void SetTransform(const glm::vec3& pos, const glm::quat& rot);

  std::vector<Vertex> GetVertices() { return mVertices; }
  std::vector<GLuint> GetIndices() { return mIndices; }

private:
  std::vector<Vertex> mVertices;
  std::vector<GLuint> mIndices;
  VAO mVAO;

  glm::mat4 mModel = glm::mat4(1.0f);
};
