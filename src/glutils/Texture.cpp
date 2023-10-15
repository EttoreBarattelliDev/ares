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

#include "ares/glutils/Texture.hpp"
#include "ares/glutils/GlUtils.hpp"

#include <stdexcept>

namespace ares
{

namespace glutils
{
    Texture::Texture(ImagePtr image, WrapType wrapS, WrapType wrapT, FilterType minF, FilterType magF)
    {
        /* Check for valid image */
        if (nullptr == image)
        {
            throw std::runtime_error("Invalid image");
        }

        /* Create texture object */
        glGenTextures(1, &m_tex);
        GlUtils::checkGLError("glGenTextures");

        /* Bind texture */
        glBindTexture(GL_TEXTURE_2D, m_tex);
        GlUtils::checkGLError("glBindTexture");

        /* Set texture wrapping parameters */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
        GlUtils::checkGLError("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));
        GlUtils::checkGLError("glTexParameteri");

        /* Set texture filtering parameters */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minF));
        GlUtils::checkGLError("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magF));
        GlUtils::checkGLError("glTexParameteri");

        /* Create texture image */
        glTexImage2D(GL_TEXTURE_2D, 0, image->glFormat(), image->width(), image->height(), 0, image->glFormat(), GL_UNSIGNED_BYTE, image->imageData().data());
        GlUtils::checkGLError("glTexImage2D");
        glGenerateMipmap(GL_TEXTURE_2D);
        GlUtils::checkGLError("glGenerateMipmap");

        /* Unbind */
        deactivate();
    }

    Texture::~Texture()
    {
        /* Unbind */
        deactivate();

        /* Delete Texture */
        glDeleteTextures(1, &m_tex);
        GlUtils::checkGLError("glDeleteTextures");
    }

    void Texture::activate(int32_t unit)
    {
        /* Activate and bind texture */
        glActiveTexture(GL_TEXTURE0 + unit);
        GlUtils::checkGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, m_tex);
        GlUtils::checkGLError("glBindTexture");
    }

    void Texture::deactivate()
    {
        /* Unbind */
        glBindTexture(GL_TEXTURE_2D, 0);
        GlUtils::checkGLError("glBindTexture");
    }

}

}
