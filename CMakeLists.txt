cmake_minimum_required(VERSION 3.1)
project(Ember)



# Use FetchContent to download and include GLM
include(FetchContent)


FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    GIT_SHALLOW true
)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

# Use FetchContent to download and include GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.4
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# Use FetchContent to download and include GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.34
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()



# Define a function to download files if they don't exist
function(download_imgui_file url filename)
  set(file_path "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/${filename}")
  if(NOT EXISTS "${file_path}")
    file(DOWNLOAD "${url}" "${file_path}" STATUS download_status)
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
      message(FATAL_ERROR "Failed to download ${filename} from ${url}")
    endif()
  endif()
endfunction()

# Base URL for ImGui files
set(IMGUI_BASE_URL "https://raw.githubusercontent.com/ocornut/imgui/master")

# Define a list of ImGui files to download
set(IMGUI_FILES
  "imconfig.h"
  "imgui.cpp"
  "imgui.h"
  "imgui_draw.cpp"
  "imgui_internal.h"
  "imgui_tables.cpp"
  "imgui_widgets.cpp"
  "imstb_rectpack.h"
  "imstb_textedit.h"
  "imstb_truetype.h"
  "backends/imgui_impl_glfw.h"
  "backends/imgui_impl_glfw.cpp"
  "backends/imgui_impl_opengl3.h"
  "backends/imgui_impl_opengl3.cpp"
  "backends/imgui_impl_opengl3_loader.h"
  "imgui_demo.cpp"
)

# Loop through the list and download each file
foreach(file IN LISTS IMGUI_FILES)
  download_imgui_file("${IMGUI_BASE_URL}/${file}" "${file}")
endforeach()

# Define source files to compile
set(IMGUI_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_glfw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_opengl3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/imgui_demo.cpp
)



# Find necessary packages
find_package(OpenGL REQUIRED)
set(OpenGL_GL_PREFERENCE "GLVND")  # Specify GLVND preference



# Set the build type for PhysX
option(PHYSX_BUILD_TYPE "The build type of PhysX, i.e., one of {debug, checked, profile, release}" "checked")

# Set the base PhysX path relative to the current project structure
set(PHYSX_BASE "${CMAKE_SOURCE_DIR}/PhysX/physx")

# Add include directories for PhysX headers
include_directories(${PHYSX_BASE}/include)

# Configure the build type and library directory paths
if(CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building snippet in debug configuration")
    add_compile_definitions(_DEBUG)
    set(PHYSX_LIB_DIR "${PHYSX_BASE}/bin/linux.clang/debug")
else()
    message("Building snippet in release configuration with PhysX ${PHYSX_BUILD_TYPE} configuration")
    add_compile_definitions(NDEBUG)
    set(PHYSX_LIB_DIR "${PHYSX_BASE}/bin/linux.clang/${PHYSX_BUILD_TYPE}")
endif()

# Link to PhysX libraries
link_directories(${PHYSX_LIB_DIR})


# Set source files
set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/Camera.cpp
    src/EBO.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/Mesh.cpp
    src/Shader.cpp
    src/Entity.cpp
    # Add other source files here if any
)


# Define the executable
add_executable(Ember ${SOURCES} ${IMGUI_SOURCE_FILES})



target_include_directories(Ember PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${glad_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
)



# Link PhysX libraries and other necessary system libraries
target_link_libraries(Ember
    ${OPENGL_LIBRARIES}
    glfw
    glad
    ${glm_LIBRARY}

    PhysXExtensions_static_64
    PhysX_static_64
    PhysXPvdSDK_static_64
    PhysXCommon_static_64
    PhysXFoundation_static_64
    pthread
    dl
)
