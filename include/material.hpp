#pragma once

#include <gl_caller.hpp>

struct Uniform
{
    GLenum type;
};

class Material
{
private:
    std::vector<Uniform> properties;

public:

    //template<GLenum type>
    void push(GLenum type)
    {
        properties.push_back({type});
    }

    //template<>
    //void push<GL_SAMPLER_2D>()
    //{
    //    properties.push_back({ GL_SAMPLER_2D });
    //}
};


static Material CreateMaterial(GLuint program)
{
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count));
    std::cout << "\nActive attributes: " << count << std::endl;

    Material mat;

    for (i = 0; i < count; i++)
    {
        GLCall(glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name));

        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
        mat.push(type);
    }

}