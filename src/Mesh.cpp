#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices) {
  this->mVertices = vertices;
  this->mIndices = indices;

  mVAO.Bind();
  VBO vbo(vertices);
  EBO ebo(indices);
  mVAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
  mVAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex),
                  (void *)(3 * sizeof(float)));
  mVAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex),
                  (void *)(6 * sizeof(float)));

  mVAO.Unbind();
  vbo.Unbind();
  ebo.Unbind();
}

void Mesh::Draw(Shader &shader, Camera &camera, GLuint mode) {
  shader.Activate();
  mVAO.Bind();

  shader.setMat4("model", mModel);

  shader.setVec3("camPos", camera.GetPosition());
  camera.Matrix(shader, "camMatrix");

  glDrawElements(mode, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::SetTransform(const glm::mat4 &mat) {
  mModel = mat;
  // Rotate to go from my coords to opengl coords
  glm::mat4 transformation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
                                         glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 transformation1 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
  mModel = transformation1 * transformation * mModel;
}

void Mesh::SetTransform(const glm::vec3 &pos, const glm::quat &rot) {
  glm::mat4 rotationMatrix = glm::toMat4(rot);
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), pos);
  glm::mat4 transform = translationMatrix * rotationMatrix;
  mModel = transform;

  // Rotate to go from my coords to opengl coords
  glm::mat4 transformation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
                                         glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 transformation1 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
  mModel = transformation1 * transformation * mModel;
}

Mesh Mesh::CreateCube(float size) {
  glm::vec3 color = glm::vec3(0.3f);
  float d = size / 2.0f;

  // Define positions for the eight cube vertices
  glm::vec3 ppp = glm::vec3(d, d, d);
  glm::vec3 npp = glm::vec3(-d, d, d);
  glm::vec3 pnp = glm::vec3(d, -d, d);
  glm::vec3 nnp = glm::vec3(-d, -d, d);
  glm::vec3 ppn = glm::vec3(d, d, -d);
  glm::vec3 npn = glm::vec3(-d, d, -d);
  glm::vec3 pnn = glm::vec3(d, -d, -d);
  glm::vec3 nnn = glm::vec3(-d, -d, -d);

  // Define reusable normals for each face direction
  glm::vec3 normals[] = {
      {0.0f, 0.0f, 1.0f},  // Front face
      {0.0f, 0.0f, -1.0f}, // Back face
      {-1.0f, 0.0f, 0.0f}, // Left face
      {1.0f, 0.0f, 0.0f},  // Right face
      {0.0f, 1.0f, 0.0f},  // Top face
      {0.0f, -1.0f, 0.0f}  // Bottom face
  };

  // Define reusable texture coordinates for each vertex in a face
  glm::vec2 texCoords[] = {
      {1.0f, 1.0f}, // Top Right
      {1.0f, 0.0f}, // Bottom Right
      {0.0f, 0.0f}, // Bottom Left
      {0.0f, 1.0f}  // Top Left
  };

  // Define vertices for the cube, using normals and texCoords
  std::vector<Vertex> cubeVertices = {
      // Front face
      {ppp, color, normals[0]}, // Top Right
      {pnp, color, normals[0]}, // Bottom Right
      {nnp, color, normals[0]}, // Bottom Left
      {npp, color, normals[0]}, // Top Left

      // Back face
      {ppn, color, normals[1]}, // Top Right
      {pnn, color, normals[1]}, // Bottom Right
      {nnn, color, normals[1]}, // Bottom Left
      {npn, color, normals[1]}, // Top Left

      // Left face
      {npp, color, normals[2]}, // Top Right
      {nnp, color, normals[2]}, // Bottom Right
      {nnn, color, normals[2]}, // Bottom Left
      {npn, color, normals[2]}, // Top Left

      // Right face
      {ppp, color, normals[3]}, // Top Right
      {pnp, color, normals[3]}, // Bottom Right
      {pnn, color, normals[3]}, // Bottom Left
      {ppn, color, normals[3]}, // Top Left

      // Top face
      {ppp, color, normals[4]}, // Top Right
      {npp, color, normals[4]}, // Top Left
      {npn, color, normals[4]}, // Bottom Left
      {ppn, color, normals[4]}, // Bottom Right

      // Bottom face
      {pnp, color, normals[5]}, // Top Right
      {nnp, color, normals[5]}, // Top Left
      {nnn, color, normals[5]}, // Bottom Left
      {pnn, color, normals[5]}  // Bottom Right
  };

  // Define indices for the cube (same as before)
  std::vector<unsigned int> cubeIndices = {// Front face
                                           0, 1, 3, 1, 2, 3,
                                           // Back face
                                           4, 5, 7, 5, 6, 7,
                                           // Left face
                                           8, 9, 11, 9, 10, 11,
                                           // Right face
                                           12, 13, 15, 13, 14, 15,
                                           // Top face
                                           16, 17, 19, 17, 18, 19,
                                           // Bottom face
                                           20, 21, 23, 21, 22, 23};

  // Initialize the Mesh with cube data
  return Mesh(cubeVertices, cubeIndices);
}
