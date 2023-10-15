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

#ifndef ATTRIBUTE_DATA_HPP_INCLUDED
#define ATTRIBUTE_DATA_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include "ares/glutils/Vbo.hpp"

namespace ares
{

namespace glutils
{
    class AttributeData;
    using AttributeDataPtr = std::shared_ptr<AttributeData>;

    /*!
     * @brief Class representing an OpenGL vertex attribute
     * 
     * This class can be used to handle vertex attributes in the shader,
     * to set attribute location, size, stride and offset in the Vbo used
     * to contain the buffer data and to enable/disable the vertex arrays
     */
    class AttributeData
    {
    public:
        enum class AttributeType
        {
            Byte = GL_BYTE,
            Float = GL_FLOAT,
            Int = GL_INT,
            Short = GL_SHORT,
            UnsignedByte = GL_UNSIGNED_BYTE,
            UnsignedInt = GL_UNSIGNED_INT,
            UnsignedShort = GL_UNSIGNED_SHORT
        };

        /*!
         * @brief Class constructor
         */
        AttributeData(const std::string& name, VboPtr vbo, int32_t size, AttributeType type, bool normalized, int32_t stride, int32_t offset);

        /*!
         * @brief Class destructor
         */
        virtual ~AttributeData() = default;

        AttributeData() = delete;
        AttributeData(const AttributeData&) = delete;
        AttributeData& operator=(const AttributeData&) = delete;

        /*!
         * @brief Name getter
         * 
         * @return Attribute name
         */
        const std::string& name() const { return m_name; }

        /*!
         * @brief Vbo getter
         * 
         * @return Vbo object
         */
        VboPtr vbo() const { return m_vbo; }

        /*!
         * @brief Size getter
         * 
         * @return Attribute size
         */
        int32_t size() const { return m_size; }

        /*!
         * @brief Type getter
         * 
         * @return Attribute type
         */
        AttributeType type() const { return m_type; }

        /*!
         * @brief Normalized getter
         * 
         * @return Attribute normalized
         */
        bool normalized() const { return m_normalized; }

        /*!
         * @brief Stride getter
         * 
         * @return Attribute stride
         */
        int32_t stride() const { return m_stride; }

        /*!
         * @brief Offset getter
         * 
         * @return Attribute offset
         */
        int32_t offset() const { return m_offset; }

    private:
        /*! Attribute name */
        std::string m_name;

        /*! VBO */
        VboPtr m_vbo;

        /*! Attribute size */
        int32_t m_size;

        /*! Attribute type */
        AttributeType m_type;

        /*! Attribute normalized */
        bool m_normalized;

        /*! Attribute stride */
        int32_t m_stride;

        /*! Attribute offset */
        int32_t m_offset;
    };
}

}

#endif
