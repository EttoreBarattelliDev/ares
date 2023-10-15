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

#include "ares/glutils/Attribute.hpp"
#include "ares/glutils/AttributeData.hpp"
#include "ares/glutils/GlUtils.hpp"

namespace ares
{

namespace glutils
{
    Attribute::Attribute(const std::string& name, int32_t location)
        : m_name(name)
        , m_location(location)
    {
    }

    void Attribute::activate(const AttributeDataPtr& data)
    {
        /* Make sure attribute location is valid */
        if ((m_location >= 0) && (nullptr != data->vbo()))
        {
            /* Activate Vbo */
            data->vbo()->activate();

            /* Enable attribute */
            glEnableVertexAttribArray(static_cast<GLuint>(m_location));
            GlUtils::checkGLError("glEnableVertexAttribArray");

            /* Set attribute stride and offset in the VBO */
            glVertexAttribPointer(static_cast<GLuint>(m_location),
                                  static_cast<GLint>(data->size()),
                                  static_cast<GLenum>(data->type()),
                                  static_cast<GLboolean>(data->normalized()),
                                  static_cast<GLsizei>(data->stride()),
                                  (const void*)(intptr_t)data->offset());
            GlUtils::checkGLError("glVertexAttribPointer");
        }
    }

    void Attribute::deactivate(const AttributeDataPtr& data)
    {
        if ((m_location >= 0) && (nullptr != data->vbo()))
        {
            /* Disable attribute */
            glDisableVertexAttribArray(static_cast<GLuint>(m_location));
            GlUtils::checkGLError("glDisableVertexAttribArray");

            /* Deactivate Vbo */
            data->vbo()->deactivate();
        }
    }

}

}
