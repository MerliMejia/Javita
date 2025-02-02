#if !defined(__SHADER__)
#define __SHADER__

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

struct Shader
{
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
};

Shader createShader(const std::string &vertexPath, const std::string &fragmentPath);

#endif // __SHADER__
