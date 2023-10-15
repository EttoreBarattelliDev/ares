/******************************************************************************/
/*!
 * @file
 * @author Ettore Barattelli
 * @copyright
 * This file is part of ARES, distributed under MIT license
 * \n\n
 * MIT License
 * \n\n
 * Copyright (c) 2023 Ettore Barattelli
 * \n\n
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * \n\n
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#include <functional>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "ares/glutils/ShaderManager.hpp"
#include "ares/glutils/GlUtils.hpp"

/* Shader ID pair alias */
using ShaderIDPair = std::pair<GLuint, GLuint>;

/* Define a specialization of hash for a pair of shader IDs */
template<>
struct std::hash<ShaderIDPair>
{
    std::size_t operator()(const ShaderIDPair& s) const noexcept
    {
        std::hash<GLuint> hasher{};
        std::size_t h1 = hasher(s.first);
        std::size_t h2 = hasher(s.second);
        return h1 ^ (h2 << 1);
    }
};

namespace ares
{

namespace glutils
{

namespace ShaderManager
{
    static std::unordered_map<std::size_t, GLuint>  sg_vertShaderMap;
    static std::unordered_map<std::size_t, GLuint>  sg_fragShaderMap;
    static std::unordered_map<ShaderIDPair, GLuint> sg_shaderProgMap;
    static std::unordered_map<GLuint, ShaderPtr>    sg_shaderPtrMap;

    static GLuint compileShader(const char* shaderSource, GLenum shaderType)
    {
        /* Create shader */
        GLuint retval = glCreateShader(shaderType);

        /* Load the source code */
        glShaderSource(retval, 1, (const char**)&shaderSource, NULL);

        /* Compile the source code */
        glCompileShader(retval);

        /* Check that the shader compiled */
        GLint status;
        glGetShaderiv(retval, GL_COMPILE_STATUS, &status);
        if (0 == status)
        {
            /* If an error happened, first retrieve the length of the log message */
            int32_t infoLogLength = 0;
            int32_t charactersWritten = 0;
            glGetShaderiv(retval, GL_INFO_LOG_LENGTH, &infoLogLength);

            /* Allocate enough space for the message and retrieve it */
            std::vector<char> infoLog;
            infoLog.resize(infoLogLength);
            glGetShaderInfoLog(retval, infoLogLength, &charactersWritten, infoLog.data());

            /* Print message */
            if (infoLogLength > 1)
            {
                std::cout << infoLog.data() << std::endl;
            }
            else
            {
                std::cout << "Failed to compile shader" << std::endl;
            }

            /* Throw exception */
            throw std::runtime_error("Failed to compile shader");
        }

        return retval;
    }

    static GLuint linkShader(GLuint vertShader, GLuint fragShader)
    {
        /* Create program */
        GLuint retval = glCreateProgram();

        /* Attach the fragment and vertex shaders to it */
        glAttachShader(retval, vertShader);
        glAttachShader(retval, fragShader);

        /* Link the program */
        glLinkProgram(retval);

        /* Check if linking succeeded */
        GLint status;
        glGetProgramiv(retval, GL_LINK_STATUS, &status);
        if (0 == status)
        {
            /* If an error happened, first retrieve the length of the log message */
            int32_t infoLogLength = 0;
            int32_t charactersWritten = 0;
            glGetShaderiv(retval, GL_INFO_LOG_LENGTH, &infoLogLength);

            /* Allocate enough space for the message and retrieve it */
            std::vector<char> infoLog;
            infoLog.resize(infoLogLength);
            glGetProgramInfoLog(retval, infoLogLength, &charactersWritten, infoLog.data());

            /* Print error message */
            if (infoLogLength > 1)
            {
                std::cout << infoLog.data() << std::endl;
            }
            else
            {
                std::cout << "Failed to compile shader" << std::endl;
            }

            /* Throw exception */
            throw std::runtime_error("Failed to compile shader");
        }

        return retval;
    }

    ShaderPtr getShader(const char* vertShaderSource, const char* fragShaderSource)
    {
        /* Assume failure */
        ShaderPtr retval = nullptr;

        /* Create hasher */
        const std::hash<const char*> hasher{};

        /* Vertex shader */
        GLuint vertShader = 0;
        const std::size_t vertHash = hasher(vertShaderSource);
        if (sg_vertShaderMap.end() != sg_vertShaderMap.find(vertHash))
        {
            /* We already had a shader for this code, re-use it */
            vertShader = sg_vertShaderMap.at(vertHash);
        }
        else
        {
            /* Compile vertex shader and add it to the map */
            vertShader = compileShader(vertShaderSource, GL_VERTEX_SHADER);
            sg_vertShaderMap.emplace(vertHash, vertShader);
        }

        /* Fragment shader */
        GLuint fragShader = 0;
        const std::size_t fragHash = hasher(fragShaderSource);
        if (sg_fragShaderMap.end() != sg_fragShaderMap.find(fragHash))
        {
            /* We already had a shader for this code, re-use it */
            fragShader = sg_fragShaderMap.at(fragHash);
        }
        else
        {
            /* Compile fragment shader and add it to the map */
            fragShader = compileShader(fragShaderSource, GL_FRAGMENT_SHADER);
            sg_fragShaderMap.emplace(fragHash, fragShader);
        }

        /* Shader program */
        GLuint shaderProg = 0;
        const ShaderIDPair shaderIDPair = std::make_pair(vertShader, fragShader);
        if (sg_shaderProgMap.end() != sg_shaderProgMap.find(shaderIDPair))
        {
            /* We already had a program for these shaders, re-use it */
            shaderProg = sg_shaderProgMap.at(shaderIDPair);
        }
        else
        {
            /* Link program and add it to the map */
            shaderProg = linkShader(vertShader, fragShader);
            sg_shaderProgMap.emplace(shaderIDPair, shaderProg);
        }

        if (sg_shaderPtrMap.end() != sg_shaderPtrMap.find(shaderProg))
        {
            /* We already had a shader object for this program, re-use it */
            retval = sg_shaderPtrMap.at(shaderProg);
        }
        else
        {
            /* Create shader object and add it to the map */
            retval = std::make_shared<Shader>(shaderProg);
            sg_shaderPtrMap.emplace(shaderProg, retval);
        }

        return retval;
    }

}

}

}
