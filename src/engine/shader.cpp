#include "shader.h"

std::string readFileToString(const std::string &filePath)
{

    std::string code;
    std::ifstream file;

    try
    {
        file.open(filePath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << file.rdbuf();

        file.close();

        code = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        std::cout << e.what() << std::endl;
    }

    return code;
}

Shader createShader(const std::string &vertexPath, const std::string &fragmentPath)
{
    Shader shader;

    // Read files into local std::string variables
    std::string vertexShaderCode = readFileToString(vertexPath);
    std::string fragmentShaderCode = readFileToString(fragmentPath);

    // Get C-style pointers valid for the duration of this function
    const char *vShaderSource = vertexShaderCode.c_str();
    const char *fShaderSource = fragmentShaderCode.c_str();

    shader.vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader.vertexShader, 1, &vShaderSource, NULL);
    glCompileShader(shader.vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader.vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader.vertexShader, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    shader.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader.fragmentShader, 1, &fShaderSource, NULL);
    glCompileShader(shader.fragmentShader);

    glGetShaderiv(shader.fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader.fragmentShader, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    shader.shaderProgram = glCreateProgram();
    glAttachShader(shader.shaderProgram, shader.vertexShader);
    glAttachShader(shader.shaderProgram, shader.fragmentShader);
    glLinkProgram(shader.shaderProgram);

    glGetProgramiv(shader.shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shader.shaderProgram, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }

    glDeleteShader(shader.vertexShader);
    glDeleteShader(shader.fragmentShader);

    return shader;
}
