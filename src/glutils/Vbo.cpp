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

#include "ares/glutils/Vbo.hpp"
#include "ares/glutils/GlUtils.hpp"

namespace ares
{

namespace glutils
{
    Vbo::Vbo(const void* data, int32_t dataSize, TargetType target)
        : m_target(target)
    {
        /* Generate a buffer object */
        glGenBuffers(1, &m_vbo);
        GlUtils::checkGLError("glGenBuffers");

        /* Bind buffer */
        glBindBuffer(static_cast<GLenum>(m_target), m_vbo);
        GlUtils::checkGLError("glBindBuffer");

        /* Set buffer data */
        glBufferData(static_cast<GLenum>(m_target), static_cast<GLuint>(dataSize), data, GL_STATIC_DRAW);
        GlUtils::checkGLError("glBufferData");

        /* Unbind */
        deactivate();
    }

    Vbo::~Vbo()
    {
        /* Unbind */
        deactivate();

        /* Delete VBO */
        glDeleteBuffers(1, &m_vbo);
        GlUtils::checkGLError("glDeleteBuffers");
    }

    void Vbo::activate()
    {
        /* Bind buffer */
        glBindBuffer(static_cast<GLenum>(m_target), m_vbo);
        GlUtils::checkGLError("glBindBuffer");
    }

    void Vbo::deactivate()
    {
        /* Unbind */
        glBindBuffer(static_cast<GLenum>(m_target), 0);
        GlUtils::checkGLError("glBindBuffer");
    }

    

}

}
