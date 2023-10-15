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

#ifndef ATTRIBUTE_HPP_INCLUDED
#define ATTRIBUTE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <GLES2/gl2.h>

namespace ares
{

namespace glutils
{
    class AttributeData;
    using AttributeDataPtr = std::shared_ptr<AttributeData>;

    class Attribute;
    using AttributePtr = std::shared_ptr<Attribute>;

    /*!
     * @brief Class representing an OpenGL vertex attribute
     * 
     * This class can be used to handle vertex attributes in the shader,
     * to set attribute location, size, stride and offset in the Vbo used
     * to contain the buffer data and to enable/disable the vertex arrays
     */
    class Attribute
    {
    public:
        /*!
         * @brief Class constructor
         */
        Attribute(const std::string& name, int32_t location);

        /*!
         * @brief Class destructor
         */
        virtual ~Attribute() = default;

        Attribute() = delete;
        Attribute(const Attribute&) = delete;
        Attribute& operator=(const Attribute&) = delete;

        /*!
         * @brief Activates the attribute and vertex buffer array
         *
         * @param[in] data - Data for the attribute
         */
        void activate(const AttributeDataPtr& data);

        /*!
         * @brief Deactivates the attribute and vertex buffer array
         *
         * @param[in] data - Data for the attribute
         */
        void deactivate(const AttributeDataPtr& data);

        /*!
         * @brief Name getter
         * 
         * @return Attribute name
         */
        const std::string& name() const { return m_name; }

        /*!
         * @brief Location getter
         * 
         * @return Attribute location
         */
        int32_t location() const { return m_location; }

    private:
        /*! Attribute name */
        std::string m_name;

        /*! Attribute location */
        int32_t m_location;
    };
}

}

#endif
