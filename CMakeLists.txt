cmake_minimum_required(VERSION 3.28)
project(OpenglTest C)

set(CMAKE_C_STANDARD 11)

# Add the shaders folder to the source directory
set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/test)

# Add the path to your GLFW installation
set(GLFW_DIR libs/glfw)
set(GLAD_DIR libs/glad)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(CGLM_DIR libs/cglm)

# Build GLFW as a static library
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Add GLFW
add_subdirectory(${GLFW_DIR} glfw)

include_directories(${GLFW_DIR}/include
        ${GLAD_DIR}/include
        ${SHADERS_DIR}
        ${LIBS_DIR}
        ${CGLM_DIR}/include)

# Ensure the shader files are copied to the build directory
add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SHADERS_DIR} ${CMAKE_BINARY_DIR}/shaders
        DEPENDS ${SHADERS_DIR}
        COMMENT "Copying shaders to build directory"
)

# Ensure the shader files are copied to the build directory
add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR} ${CMAKE_BINARY_DIR}/data
        DEPENDS ${DATA_DIR}
        COMMENT "Copying data to build directory"
)

add_executable(OpenglTest src/main.c ${GLAD_DIR}/src/glad.c
        src/shader.c
        src/shader.h
        src/vertex.c
        src/vertex.h
        libs/stb_image.h
        src/texture.c
        src/texture.h
        src/camera.c
        src/camera.h
)

# Ensure shaders are copied before building the executable
add_dependencies(OpenglTest copy_shaders
                 OpenglTest copy_data)

target_link_libraries(OpenglTest glfw ${GLFW_LIBRARIES})