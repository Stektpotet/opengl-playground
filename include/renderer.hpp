#pragma once
#include <gl_caller.hpp>

#include <vertexArray.hpp>
#include <elementBuffer.hpp>
#include <shader.hpp>

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const ElementBuffer& eb, const Shader& shader) const;
};

class EdgeRenderer : public Renderer
{
public:
    void drawEdged(const VertexArray& va, const ElementBuffer& eb, const Shader& shader, const Shader& edgeShader) const;
};