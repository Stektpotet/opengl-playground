project(GLprog)
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)

# Application
add_executable(GLprog "${CMAKE_SOURCE_DIR}/src/main.cpp")

target_sources(GLprog
	PRIVATE "${CMAKE_SOURCE_DIR}/src/shader.cpp"
	PRIVATE "${CMAKE_SOURCE_DIR}/src/gl_util.cpp"
	PRIVATE "${CMAKE_SOURCE_DIR}/src/texture.cpp"
	PRIVATE "${CMAKE_SOURCE_DIR}/src/renderer.cpp"
	PRIVATE "${CMAKE_SOURCE_DIR}/src/gl_caller.cpp"
    PRIVATE "${CMAKE_SOURCE_DIR}/src/vertexArray.cpp"
    PRIVATE "${CMAKE_SOURCE_DIR}/src/vertexBuffer.cpp"
    PRIVATE "${CMAKE_SOURCE_DIR}/src/elementBuffer.cpp"
)

set_target_properties(GLprog
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)
if (DARWIN)
target_compile_options(GLprog
    PRIVATE "-Wno-invalid-source-encoding" # Problem with encoding of norwegian characters cross platform. PRobably something to do with UTF-8 vs UTF-16 on Windows(Visual Studio) vs Linux/Mac.
)
endif(DARWIN)

target_include_directories(GLprog
    PRIVATE "${CMAKE_SOURCE_DIR}/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/SOIL/src/"
    PRIVATE "${CMAKE_SOURCE_DIR}/glew/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/glfw/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/glm"
)


find_package(OpenGL REQUIRED)

find_library(GLEW_LIBRARY
	glew32
	HINTS ${CMAKE_SOURCE_DIR}/glew/lib
)


set(GLFW_BUILD_EXAMPLES OFF) 
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)
link_directories(${CMAKE_BINARY_DIR})
link_directories(${GLFW_BINARY_DIR})


add_subdirectory(SOIL)
link_directories(${CMAKE_BINARY_DIR})
link_directories(${SOIL_BINARY_DIR})

if(MSVC)
file(COPY  ${CMAKE_SOURCE_DIR}/dll/glew32.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)
configure_file(${CMAKE_SOURCE_DIR}/include/shaders/base.shader ${CMAKE_BINARY_DIR}/include/shaders/base.shader COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/include/shaders/edge.shader ${CMAKE_BINARY_DIR}/include/shaders/edge.shader COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/res/textures/BrickWall.jpg ${CMAKE_BINARY_DIR}/res/textures/BrickWall.jpg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/res/textures/Checkers.jpg ${CMAKE_BINARY_DIR}/res/textures/Checkers.jpg COPYONLY)

#add_definitions(-DGLEW_STATIC)
target_link_libraries(GLprog 
	glfw
	SOIL
	${GLEW_LIBRARY} 
	${GLFW_LIBRARIES}
	${SOIL_LIBRARIES}
) 
endif(MSVC)
