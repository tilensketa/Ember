#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char *filename) {
  std::ifstream in(filename, std::ios::binary);
  if (in) {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return (contents);
  }
  throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile) {
  // Read vertexFile and fragmentFile and store the strings
  std::string vertexCode = get_file_contents(vertexFile);
  std::string fragmentCode = get_file_contents(fragmentFile);

  // Convert the shader source strings into character arrays
  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  // Create Vertex Shader Object and get its reference
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // Attach Vertex Shader source to the Vertex Shader Object
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  // Compile the Vertex Shader into machine code
  glCompileShader(vertexShader);
  // Checks if Shader compiled succesfully
  compileErrors(vertexShader, "VERTEX");

  // Create Fragment Shader Object and get its reference
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // Attach Fragment Shader source to the Fragment Shader Object
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  // Compile the Vertex Shader into machine code
  glCompileShader(fragmentShader);
  // Checks if Shader compiled succesfully
  compileErrors(fragmentShader, "FRAGMENT");

  // Create Shader Program Object and get its reference
  mID = glCreateProgram();
  // Attach the Vertex and Fragment Shaders to the Shader Program
  glAttachShader(mID, vertexShader);
  glAttachShader(mID, fragmentShader);
  // Wrap-up/Link all the shaders together into the Shader Program
  glLinkProgram(mID);
  // Checks if Shaders linked succesfully
  compileErrors(mID, "PROGRAM");

  // Delete the now useless Vertex and Fragment Shader objects
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate() { glUseProgram(mID); }

// Deletes the Shader Program
void Shader::Delete() { glDeleteProgram(mID); }

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char *type) {
  // Stores status of compilation
  GLint hasCompiled;
  // Character array to store error message in
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
                << infoLog << std::endl;
    } else {
      std::cout << "SHADER_COMPILATION_COMPLETE" << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
                << infoLog << std::endl;
    }

    else {
      std::cout << "SHADER_LINIKING_COMPLETE" << std::endl;
    }
  }
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(getLocation(name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(getLocation(name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(getLocation(name.c_str()), value);
}
void Shader::setVec3(const std::string &name, glm::vec3 &value) const {
  glUniform3f(getLocation(name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4(const std::string &name, glm::vec4 &value) const {
  glUniform4f(getLocation(name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::setMat4(const std::string &name, glm::mat4 &value) const {
  glUniformMatrix4fv(getLocation(name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

int Shader::getLocation(const std::string &name) const {
  int location = glGetUniformLocation(mID, name.c_str());
  return location;
}
