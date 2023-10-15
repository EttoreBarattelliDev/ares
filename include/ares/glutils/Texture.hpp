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

#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/glutils/Image.hpp"

namespace ares
{

namespace glutils
{
    class Texture;
    using TexturePtr = std::shared_ptr<Texture>;

    /*!
     * @brief Texture to create and handle OpenGL textures
     * 
     * This class implements an OpenGL texture and provides
     * functionalities to create, activate and deactivate textures
     */
    class Texture
    {
    public:
        /*!
         * @brief Wrap type enumeration
         */
        enum class WrapType
        {
            Repeat = GL_REPEAT,
            ClampToEdge = GL_CLAMP_TO_EDGE,
            MirroredRepeat = GL_MIRRORED_REPEAT
        };

        /*!
         * @brief Filter type enumeration
         */
        enum class FilterType
        {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,
            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMipmapNearset = GL_LINEAR_MIPMAP_NEAREST,
            NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinerMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
        };

        /*!
         * @brief Class constructor
         * 
         * This constructor creates an OpenGL texture with
         * the provided image data
         * 
         * @param[in] image - Image for the texture
         * @param[in] wrapS - Wrap mode over X
         * @param[in] wrapT - Wrap mode over Y
         * @param[in] minF - Min Filter mode
         * @param[in] magF - Mag Filter mode
         */
        Texture(ImagePtr image, WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge, FilterType minF = FilterType::Nearest, FilterType magF = FilterType::Nearest);

        /*!
         * @brief Class destructor
         * 
         * Releases all allocated resources
         */
        virtual ~Texture();

        Texture() = delete;
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        /*!
         * @brief Activates the texture on the provided unit
         * 
         * @param[in] unit - Texture unit
         */
        void activate(int32_t unit);

        /*!
         * @brief Deactivates the texture
         */
        void deactivate();

        /*!
         * @brief OpenGL Texture ID getter
         * 
         * @return OpenGL Texture ID
         */
        GLuint tex() const { return m_tex; }

    private:
        /*! OpenGL Texture object ID */
        GLuint m_tex;


    };
}

}

#endif
