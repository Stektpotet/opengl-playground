#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl_caller.hpp>
#include <renderer.hpp>
#include <vertexArray.hpp>
#include <vertexBuffer.hpp>
#include <elementBuffer.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <matrixUtil.hpp>
#include <shaderParser.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct ShaderFile
{
    std::string vert;
    std::string frag;
    std::string geom;
    ///Shader ...
};

static ShaderFile ParseShader(const std::string& shader)
{
    enum ShaderType{ NONE = -1, VERT = 0, FRAG = 1, GEOM = 2 };

    std::ifstream stream(shader);
    ShaderType type;
    std::string line;
    std::stringstream ss[3];
    
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vert") != std::string::npos)
            {
                type = VERT;
            }
            else if(line.find("frag") != std::string::npos)
            {
                type = FRAG;
            }
            else if (line.find("geom") != std::string::npos)
            {
                type = GEOM;
            }
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }

    return ShaderFile
    {
        ss[(int)VERT].str(),
        ss[(int)FRAG].str(),
        ss[(int)GEOM].str() 
    };
}
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id;
    GLCall(id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr)); //TODO understand
    GLCall(glCompileShader(id));
    
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile shader: " << ShaderTypeName(type) << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        std::cin.get();
        return 0;
    }
    
    return id;
}
static unsigned int CreateShader(const std::string& vert, const std::string& frag, const std::string& geom)
{
    unsigned int program; GLCall(program = glCreateProgram());
    unsigned int vs, fs, gs;
    if (!vert.empty())
    {
        vs = CompileShader(GL_VERTEX_SHADER, vert);
        GLCall(glAttachShader(program, vs));

        GLCall(glDeleteShader(vs));
    }
    if (!frag.empty())
    {
        fs = CompileShader(GL_FRAGMENT_SHADER, frag);
        GLCall(glAttachShader(program, fs));

        GLCall(glDeleteShader(fs));
    }
    if (!geom.empty())
    {
        gs = CompileShader(GL_GEOMETRY_SHADER, geom);
        GLCall(glAttachShader(program, gs));

        GLCall(glDeleteShader(gs));
    }

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    
    return program;
}

static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods)
{
    printf("Pressing %i, as char: %c\n", keyCode, char(keyCode));
    if (keyCode == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, 1);
    }
   
}
static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods)
{
    printf("Holding %i, as char: %c\n", keyCode, char(keyCode));
    if (keyCode == GLFW_KEY_W)
    {

    }
    if (keyCode == GLFW_KEY_S)
    {

    }
}
static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods)
{
    printf("Unpressed %i, as char: %c\n", keyCode, char(keyCode));
}
static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        OnInputKeyPress(window, keyCode, scanCode, mods);
        break;
    case GLFW_REPEAT:
        OnInputKeyHold(window, keyCode, scanCode, mods);
        break;
    case GLFW_RELEASE:
        OnInputKeyUnpress(window, keyCode, scanCode, mods);
        break;
    }
}

static float fovy = 90, aspect = 1;
static void OnCursorHover(GLFWwindow* window, double x, double y)
{
    fovy = (y / 512) + 32;
}

int main(int argc, char* args[])
{

    if (!glfwInit())
    {
        std::cout << "Failed initializing GLFW" << std::endl;
        std::cin.get();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 16);
    auto window = glfwCreateWindow(1024, 1024, "Learning OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!window)
    {
        std::cout << "Failed initializing glfwWindow" << std::endl;
        std::cin.get();
        glfwTerminate();
        return -1;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed initializing GLEW" << std::endl;
        std::cin.get();
        glfwTerminate();
        return -1;
    }
    //setup
    glfwSwapInterval(1);
    GLCall(glClearColor(0.05, 0.06, 0.075, 0));

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

    Vertex vertices[] = {
        { { -0.5f, -0.5f,  0.5f }, { 1 },  { 0, 0 }, { 255,     255-96,  255-192, 255 } },
        { {  0.5f, -0.5f,  0.5f }, { 1 },  { 1, 0 }, { 255-16,  255-112, 255-208, 255 } },
        { {  0.5f,  0.5f,  0.5f }, { 1 },  { 1, 1 }, { 255-32,  255-128, 255-224, 255 } },
        { { -0.5f,  0.5f,  0.5f }, { 1 },  { 0, 1 }, { 255-48,  255-144, 255-240, 255 } },
                                   
        { { -0.5f, -0.5f, -0.5f }, { 1 },  { 1, 0 }, { 255-64,  255-160, 255-255, 255 } },
        { { -0.5f,  0.5f, -0.5f }, { 1 },  { 1, 1 }, { 255-80,  255-176, 255, 255 } },
        
        { {  0.5f, -0.5f, -0.5f }, { 1 },  { 0, 0 }, { 255-96,  255-192, 255-16, 255 } },
        { {  0.5f,  0.5f, -0.5f }, { 1 },  { 0, 1 }, { 255-112, 255-208, 255-32, 255 } },

        { { -0.5f, -0.5f, -0.5f }, { 1 },  { 0, 1 }, { 255-128, 255-224, 255-48, 255 } },
        { {  0.5f, -0.5f, -0.5f }, { 1 },  { 1, 1 }, { 255-144, 255-240, 255-64, 255 } },

        { { -0.5f,  0.5f, -0.5f }, { 1 },  { 0, 0 }, { 255-160, 255-255,  255-80, 255 } },
        { {  0.5f,  0.5f, -0.5f }, { 1 },  { 1, 0 }, { 255-176, 255,      255-96, 255 } },
        
        { { -0.5f,  0.5f,  0.5f }, { 1 },  { 0, 1 }, { 255-192, 255-16,   255-112, 255 } },
        { {  0.5f,  0.5f,  0.5f }, { 1 },  { 1, 1 }, { 255-208, 255-32,   255-128, 255 } },




        { { -1.0f, -1.0f,  1.0f }, { 1 },  { 0, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f, -1.0f,  1.0f }, { 1 },  { 1, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f,  1.0f }, { 1 },  { 1, 1 }, { 255, 255, 255, 80 } },
        { { -1.0f,  1.0f,  1.0f }, { 1 },  { 0, 1 }, { 255, 255, 255, 80 } },
        
        { { -1.0f, -1.0f, -1.0f }, { 1 },  { 1, 0 }, { 255, 255, 255, 80 } },
        { { -1.0f,  1.0f, -1.0f }, { 1 },  { 1, 1 }, { 255, 255, 255, 80 } },
        
        { {  1.0f, -1.0f, -1.0f }, { 1 },  { 0, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f, -1.0f }, { 1 },  { 0, 1 }, { 255, 255, 255, 80 } },
        
        { { -1.0f, -1.0f, -1.0f }, { 1 },  { 0, 1 }, { 255, 255, 255, 80 } },
        { {  1.0f, -1.0f, -1.0f }, { 1 },  { 1, 1 }, { 255, 255, 255, 80 } },

        { { -1.0f,  1.0f, -1.0f }, { 1 },  { 0, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f, -1.0f }, { 1 },  { 1, 0 }, { 255, 255, 255, 80 } },

        { { -1.0f,  1.0f,  1.0f }, { 1 },  { 0, 1 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f,  1.0f }, { 1 },  { 1, 1 }, { 255, 255, 255, 80 } },

    };

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 5, 4,

        1, 6, 7,
        7, 2, 1,

        8, 9, 1,
        1, 0, 8,

        10, 11, 9,
        9,  8,  10,

        12, 13, 11,
        11, 10, 12,




        14, 15, 16,
        16, 17, 14,

        18, 14, 3+14,
        3 + 14, 5 + 14, 4 + 14,

        1+14, 6+14, 7+14,
        7+14, 2+14, 1+14,

        8+14, 9+14, 1+14,
        1+14, 0+14, 8+14,

        10 + 14, 11 + 14, 9 + 14,
        9 + 14,  8 + 14,  10 + 14,

        12+14, 13+14, 11+14,
        11+14, 10+14, 12+14,
    };

    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glFrontFace(GL_CCW));
    GLCall(glCullFace(GL_BACK));

    GLCall(glEnable(GL_BLEND));
    //GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_MULTISAMPLE_ARB));

    auto renderer = new EdgeRenderer();

    auto va = new VertexArray();

    auto vbuf = VertexBuffer(vertices, 14*2 * sizeof(Vertex));
    
    //TODO
    // VetexBufferLayout
    // glVertexAttribPointer(location, items, type, normalized, stride, start);
    
    auto vbufLayout = VertexBufferAttribLayout();

    vbufLayout.push(3, GL_FLOAT);                       //position;
    vbufLayout.push(1, GL_UNSIGNED_INT);                //normal
    vbufLayout.push(2, GL_SHORT);                       //uv
    vbufLayout.push(4, GL_UNSIGNED_BYTE, GL_TRUE);      //color;

    Shader shader("../include/shaders/base.shader");

    //auto vbufLayout = CreateFromProgram(GLuint(shader));

    va->addBuffer(vbuf, vbufLayout);


    auto ebuf = new ElementBuffer(indicies, 36*2);

    //ParseShaderFile("../include/shaders/base.shader");

    //auto[vert, frag, geom] = ParseShader("../include/shaders/base.shader");
    

    //auto shader = CreateShader(vert, frag, geom);

    //auto[v2, f2, g2] = ParseShader("../include/shaders/edge.shader");
    //auto shader2 = CreateShader(v2, f2, g2);
   
    glm::mat4 projection = glm::perspective(90.0f, (GLfloat)1024 / (GLfloat)1024, 0.1f, 100.0f);

    //SCALE -> ROTATE -> TRANSLATE

    //GLCall(glSetUn)
    GLint uniformMVP, uniformTime;
    GLint uniformMVP2, uniformTime2;
    
    shader.bind({});
    uniformMVP  = shader.getUniformLocation("projection");
    uniformTime = shader.getUniformLocation("time");

    //GLCall(uniformMVP =  glGetUniformLocation(GLuint(shader), "projection"));
    //GLCall(uniformTime = glGetUniformLocation(GLuint(shader), "time"));
    GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
    /*
    GLCall(glUseProgram(shader2));
    GLCall(uniformMVP2 = glGetUniformLocation(shader2, "projection"));
    GLCall(uniformTime2 = glGetUniformLocation(shader2, "time"));
    GLCall(glUniformMatrix4fv(uniformMVP2, 1, GL_FALSE, glm::value_ptr(projection)));*/

    auto tex = Texture("../res/textures/Checkers.jpg");
    tex.bind();
    GLint uniformTex;
    shader.bind({});

    uniformTex = shader.getUniformLocation("mainTex");

    //GLCall(uniformTex = glGetUniformLocation(GLuint(shader), "mainTex"));
    GLCall(glUniform1i(uniformTex, 0));
    
    glfwSetKeyCallback(window, OnInputKey); //set upp callbacks
    glfwSetCursorPosCallback(window, OnCursorHover);
    
    while (!glfwWindowShouldClose(window))
    {
        renderer->clear();
        renderer->draw(*va, *ebuf, shader);
        //renderer->drawEdged(*va, *ebuf, shader, shader2);

        projection = glm::perspective(fovy, aspect, 0.1f, -100.0f);
        shader.bind({});
        GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        GLCall(glUniform1f(uniformTime, (float)glfwGetTime()));
        
        //GLCall(glUseProgram(shader2));
        //GLCall(glUniformMatrix4fv(uniformMVP2, 1, GL_FALSE, glm::value_ptr(projection)));
        //GLCall(glUniform1f(uniformTime2, (float)glfwGetTime()));
        glfwSwapBuffers(window);

        glfwPollEvents(); //handles all input callbacks
    }

    //GLCall(glDeleteShader(shader));
    glfwTerminate();
    delete va;
    delete ebuf;
    delete renderer;
    return 0;
}