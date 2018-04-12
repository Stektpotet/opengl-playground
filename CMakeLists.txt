project(GLprog)
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

# Set global variables
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_COMPILER clang++-5.0)


# Turning off parts of GLFW we do not want
# Also I have deleted these folders from the repo,
# so we could not build them anymore if we wanted to - jonasjso 01.04.18
set(GLFW_BUILD_EXAMPLES 0)
set(GLFW_BUILD_TESTS 0)
set(GLFW_BUILD_DOCS 0)

# Adding subdirectory, building the glfw library which we will
# link to later on.
add_subdirectory("${CMAKE_SOURCE_DIR}/glfw")

add_subdirectory("${CMAKE_SOURCE_DIR}/SOIL")


# Find system packages
find_package(OpenGL REQUIRED)
if(NOT MSVC)
    find_package(GLEW REQUIRED)
endif()

# Configure main executeable
add_executable(GLprog "${CMAKE_SOURCE_DIR}/src/main.cpp")

set_target_properties(
    GLprog
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_sources(
    GLprog
    PRIVATE ${CMAKE_SOURCE_DIR}/src/gl_caller.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/vertexArray.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/vertexBuffer.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/renderer.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/texture.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/shader.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/gl_util.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/elementBuffer.cpp

)

###############################
# Microsoft Visual studio 2017
###############################
if(MSVC)

target_include_directories(
    GLprog
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glew-2.1.0/include
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glm-0.9.8.5
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glfw-3.2.1/include
)

file(
    COPY ${CMAKE_SOURCE_DIR}/lib/glew-2.1.0/bin/glew32.dll
    DESTINATION ${PROJECT_BINARY_DIR}/bin
)
file(
    COPY ${CMAKE_SOURCE_DIR}/lib/glew-2.1.0/lib/glew32.lib
    DESTINATION ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    GLprog
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw ${glfw_LIBRARIES}
    PRIVATE glew32
)

################################
# Apple, linux, others...
################################
else()

target_compile_options(
    GLprog
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
)

target_include_directories(
    GLprog
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/glm         #Not glm/gml??
    PRIVATE ${CMAKE_SOURCE_DIR}/glfw/include
    PRIVATE ${CMAKE_SOURCE_DIR}/SOIL/src

)

target_link_libraries(
    GLprog
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE ${GLEW_LIBRARY}
    PRIVATE glfw X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor
    PRIVATE SOIL ${SOIL_LIBRARIES}
)
endif()

message(OPENGL_LIBRARIES ${OPENGL_LIBRARIES})
message(GLEW_LIBRARY ${GLEW_LIBRARY})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message("CMAKE_CXX_CMAKE_COMPILER: " ${CMAKE_CXX_COMPILER})
