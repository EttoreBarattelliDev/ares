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

#include "ares/glutils/Shader.hpp"
#include "ares/glutils/GlUtils.hpp"

namespace ares
{

namespace glutils
{
    Shader::Shader(GLuint prog)
        : m_program(prog)
        , m_uniformMap()
    {
    }

    void Shader::activate(const std::vector<glutils::AttributeDataPtr>& attributeData)
    {
        /* Use program */
        glUseProgram(m_program);
        GlUtils::checkGLError("glUseProgram");

        /* Setup all attributes */
        for (const auto& attrData : attributeData)
        {
            auto attr = addAttribute(attrData->name());
            if ((nullptr != attr) && (nullptr != attrData->vbo()))
            {
                /* Setup attribute with data */
                attr->activate(attrData);
            }
        }
    }

    void Shader::deactivate(const std::vector<glutils::AttributeDataPtr>& attributeData)
    {
        /* Deactivate all attributes */
        for (const auto& attrData : attributeData)
        {
            auto attr = addAttribute(attrData->name());
            if ((nullptr != attr) && (nullptr != attrData->vbo()))
            {
                /* Setup attribute with data */
                attr->deactivate(attrData);
            }
        }

        /* Reset program */
        glUseProgram(0);
        GlUtils::checkGLError("glUseProgram");
    }

    GLint Shader::getAttribLocation(const std::string& attribName) const
    {
        /* Get attribute location */
        GLint retval = glGetAttribLocation(m_program, attribName.c_str());
        GlUtils::checkGLError("glGetAttribLocation");
        return retval;
    }

    GLint Shader::getUniformLocation(const std::string& unifName) const
    {
        /* Get uniform location */
        GLint retval = glGetUniformLocation(m_program, unifName.c_str());
        GlUtils::checkGLError("glGetUniformLocation");
        return retval;
    }

    AttributePtr Shader::getAttribute(const std::string& attribName) const
    {
        /* Check if requested attribute is in the map */
        AttributePtr retval = nullptr;
        if (m_attributeMap.end() != m_attributeMap.find(attribName))
        {
            /* Return attribute object */
            retval = m_attributeMap.at(attribName);
        }
        return retval;
    }

    AttributePtr Shader::addAttribute(const std::string& name)
    {
        /* Try to get attribute first */
        AttributePtr retval = getAttribute(name);
        if (nullptr == retval)
        {
            /* Not found, create a new one and add to the map */
            retval = std::make_shared<Attribute>(name, getAttribLocation(name));
            m_attributeMap.emplace(name, retval);
        }
        return retval;
    }

    void Shader::addAttributes(const std::vector<std::string>& names)
    {
        for (const auto& name : names)
        {
            addAttribute(name);
        }
    }

    UniformPtr Shader::getUniform(const std::string& unifName) const
    {
        /* Check if requested uniform is in the map */
        UniformPtr retval = nullptr;
        if (m_uniformMap.end() != m_uniformMap.find(unifName))
        {
            /* Return uniform object */
            retval = m_uniformMap.at(unifName);
        }
        return retval;
    }

}

}
