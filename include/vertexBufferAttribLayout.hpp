#pragma once
#include <gl_util.hpp>
#include <vector>

struct VertexBufferAttrib
{
    GLuint      count;
    GLenum      type;
    GLboolean   normalized;
};
//(GLuint index, 
// GLint size, 
// GLenum type, 
// GLboolean normalized, 
// GLsizei stride, 
// const void* pointer);
struct VertexBufferAttribLayout
{
private:
    std::vector<VertexBufferAttrib> m_attributes;
    GLuint m_stride;
public:
    VertexBufferAttribLayout() : m_stride(0) {}

    void push(GLuint count, GLenum type, bool normalized = false)
    {
        m_attributes.push_back({ count, type, normalized });
        m_stride += count * GLTypeSize(type);
    }

    inline const std::vector<VertexBufferAttrib> getAttributes() const { return m_attributes; }
    inline GLuint getStride() const { return m_stride; }
};

static VertexBufferAttribLayout CreateFromProgram(GLuint program) //CRASHES :(
{
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    GLCall(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count));
    std::cout << "\nActive attributes: " << count << std::endl;
    
    auto layout = VertexBufferAttribLayout();

    for (i = 0; i < count; i++)
    {
        GLCall(glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name));

        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
        layout.push(1, type);
    }
    return layout;
}