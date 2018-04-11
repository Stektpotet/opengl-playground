#pragma once
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static glm::mat4 modelToWorld(const Transform t)
{
    auto m = glm::mat4(1);      //origin

    m = glm::scale(m, t.scale);   //scale

    m = glm::rotate(m, t.eulerRotation.x, glm::vec3{ 1,0,0 });
    m = glm::rotate(m, t.eulerRotation.y, glm::vec3{ 0,1,0 });
    m = glm::rotate(m, t.eulerRotation.z, glm::vec3{ 0,0,1 });
    m = glm::translate(m, t.position);

    return m;
}

class Mesh
{
    struct Vertex
    {
        GLfloat position[3];
        GLuint  normal;//normalized integers (decimal floating point number): https://www.khronos.org/opengl/wiki/Normalized_Integer + https://stackoverflow.com/questions/14036892/using-gl-int-2-10-10-10-rev-in-glvertexattribpointer
        GLshort  uv_1[2];
        //GLhalf  uv_2[2];
        //GLhalf  uv_3[2];
        // etc.
        GLubyte  color[4];
    };
 
    

//private:
//    Vertex* vertices;
//
//public:
//
};