#if !defined(__SHADER__)
#define __SHADER__

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "core.h"

namespace Javita
{
    namespace Shaders
    {
        struct Shader
        {
            unsigned int vertexShader;
            unsigned int fragmentShader;
            unsigned int shaderProgram;
        };

        std::string readFileToString(const std::string &filePath);

        Shader createShader(const std::string &vertexPath, const std::string &fragmentPath);

    } // namespace Shader
} // namespace Javita

#endif // __SHADER__
