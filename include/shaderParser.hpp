#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

using std::string;
using std::regex;
using std::regex_match;

bool isLayoutLocationSpecifier(const string& value)
{
    //printf("REGEX TESTING ON \"%s\"\n", value.c_str());
    constexpr char regexLayoutLocationSpec[] = R"(\s*layout\s*\(\s*location\s*=\s*\d+\)\s+in\s+\w{3,}\s+\w+;\s*)";

    const static regex compiledRegex(regexLayoutLocationSpec, regex::icase);
    return regex_match(value, compiledRegex);
}

bool isUniformVar(const string& value)
{
    //printf("REGEX TESTING ON \"%s\"\n", value.c_str());
    constexpr char regexUniformVar[] = R"(\s*uniform\s+\w{3,}\s+\w+.*)";

    const static regex compiledRegex(regexUniformVar);
    return regex_match(value, compiledRegex);
}


struct ParsedShader
{
    enum ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
        GEOMETRY = 2
    };

    struct ShaderAttribute
    {
        unsigned int    location;
        unsigned int    type; //<- might be a bit of a pain in the ass...
        bool            isOutAttrib;
        std::string     name;
    };

    ShaderType                      type;
    std::vector<ShaderAttribute>    attributes;
    std::vector<std::string>        functions;

    /// <summary>   Rebuilds the shader to easily and automatically 
    ///             interface the shader's vertex & attribute spec.
    /// </summary>
    void rebuild();
    //no support for structs?

    //std::string frag;
    //std::string geom;
};

//file -> PARSE -> shader encapsulation/abstraction (ParsedShader) -> 
// override the layout to optimize attrib pointer defs -> compile the newly overridden shader -> 
//construct optimal shader attrib pointers

struct ShaderAttributeLayoutSpecifier
{
    std::string name;
    unsigned int location;

};

//template <typename T>
struct ShaderParameter
{
    int aaaaaaa;

};

static ShaderParameter parseShaderUniform(const std::string& line)
{
    auto temp = line;
    temp.erase(temp.begin(), temp.begin() + temp.find_first_not_of(' ')); //erase leading spaces
    temp.erase(temp.begin(), temp.begin() + 8);
    std::cout << "erased \"uniform\"" << std::endl;

    auto type = temp.substr(0, temp.find_first_of(' '));
    std::cout << "type: " << type << std::endl;

    temp.erase(temp.begin(), temp.begin() + temp.find_first_of(' ')); //erase leading spaces
    temp.erase(temp.begin(), temp.begin() + temp.find_first_not_of(' '));
    temp.erase(temp.begin() + temp.find_first_of(' '), temp.end());
    std::cout << "name: " << temp << std::endl;
    return { 0 };
}

static ShaderAttributeLayoutSpecifier parseLayoutLocationSpecifier(const std::string& line)
{
    auto& loc = line.substr(line.find_first_of("0123456789"), line.find_first_of(')') - line.find_first_of("0123456789"));
    loc.erase(std::remove(loc.begin(), loc.end(), ' '), loc.end()); //erase spaces
    std::cout << "location: " << std::stoi(loc) << std::endl;

    auto& typeAndName = line.substr(line.find("in") + 2, line.find(";") - (line.find("in")+2)); //type and name remains
    typeAndName.erase(typeAndName.begin(), typeAndName.begin() + typeAndName.find_first_not_of(' ')); //erase leading spaces

    auto& type = typeAndName.substr(0, typeAndName.find_first_of(' '));
    std::cout << "type: " << type.c_str() << std::endl;
    auto& name = typeAndName.substr(typeAndName.find_first_of(' '));
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end()); //erase spaces
    std::cout << "name: " << name.c_str() << std::endl;

    //std::stringstream ss(line);
    //std::string l;
    //if (!std::getline(ss, l, '='))
    //{
    //    printf("DANGER, missing part of attribute location layout!!!");
    //    exit(-1);
    //}
    //std::getline(ss, l, ')');
    //std::cout << "location: " << l << std::endl;
    //if (!std::getline(ss, l, 'n'))
    //{
    //    printf("DANGER, missing part of attribute location layout!!!");
    //    exit(-1);
    //}
    //std::getline(ss, l, ' ');
    //std::getline(ss, l, ' ');

    //std::cout << "type: " << l << std::endl;
    //std::getline(ss, l, ';');

    //std::cout << "name: " << l << std::endl;

    return { "", 0 };
}


template <typename T> 
static T convert_to(const std::string &str)
{
    std::istringstream ss(str);
    T val;
    ss >> val;
    return val;
}

static /*ParsedShader */ void ParseShaderFile(const std::string& shaderFile)
{
    std::string line;
    std::ifstream stream(shaderFile);
    if (!stream.is_open())
    {
        printf("\"%s\" could not be read...\n", shaderFile.c_str());
    }
    while (getline(stream, line))
    {
        if (isLayoutLocationSpecifier(line))
        {
            printf("\nLayout specifier found: %s\n", line.c_str());
            parseLayoutLocationSpecifier(line);
        }
        if (isUniformVar(line))
        {
            printf("\nuniform variable found: %s\n", line.c_str());
           // parseShaderUniform(line);
        }
        //regex on the line to check if it's an attribute
        //if not look for a function def and read till the function end.
        // --- Note: functions will still remain as is 
        // (or with the side effect of having to change in/out variable names and types)
    }
}
