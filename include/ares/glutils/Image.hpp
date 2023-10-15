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

#ifndef IMAGE_HPP_INCLUDED
#define IMAGE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <GLES2/gl2.h>

namespace ares
{

namespace glutils
{

    class Image;
    using ImagePtr = std::shared_ptr<Image>;

    /*!
     * @brief Image class to store image data
     * 
     * This class holds image data, including raw
     * color data/bitmap, format, dimensions.
     */
    class Image
    {
    public:
        /*!
         * @brief Image format
         */
        enum class Format
        {
            Invalid, /*!< Invalid format */
            RGB,     /*!< RGB format     */
            RGBA     /*!< RGBA format    */
        };

        /*!
         * @brief Class constructor
         * 
         * @param[in] imageData - Raw image data
         * @param[in] format - Image format
         * @param[in] width - Image width
         * @param[in] height - Image height
         */
        Image(const std::vector<uint8_t>& imageData, Format format, int32_t width, int32_t height);

        /*!
         * @brief Class destructor
         */
        virtual ~Image() = default;

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        /*!
         * @brief Image data getter
         * 
         * @return Raw image data
         */
        const std::vector<uint8_t>& imageData() const { return m_imageData; }

        /*!
         * @brief Image format getter
         * 
         * @return Image format
         */
        Format format() const { return m_format; }

        /*!
         * @brief Image width getter
         * 
         * @return Image width
         */
        int32_t width() const { return m_width; }

        /*!
         * @brief Image height getter
         * 
         * @return Image height
         */
        int32_t height() const { return m_height; }

        /*!
         * @brief Image format getter for OpenGL
         * 
         * @return OpenGL image format
         */
        GLenum glFormat() const;

    private:
        /*! Image data */
        std::vector<uint8_t> m_imageData;

        /*! Image format */
        Format m_format;

        /*! Image width */
        int32_t m_width;

        /*! Image height */
        int32_t m_height;
    };

}

}

#endif
