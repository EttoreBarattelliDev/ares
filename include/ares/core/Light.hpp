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

#ifndef LIGHT_HPP_INCLUDED
#define LIGHT_HPP_INCLUDED

#include <cstdint>
#include <memory>

namespace ares
{

namespace core
{
    class Light;
    using LightPtr = std::shared_ptr<Light>;

    /*!
     * @brief Light classto implement a generic light
     * 
     * This class serves as a base class for all specialized
     * light classes. It cannot be directly instantiated
     */
    class Light
    {
    public:
        /*!
         * @brief Light type enumeration
         */
        enum class Type
        {
            Invalid,      /*!< Invalid type      */
            Point,        /*!< Point light       */
            Directional,  /*!< Directional light */
            Spotlight     /*!< Spotlight         */
        };

        /*!
         * @brief Class destructor
         */
        virtual ~Light() = default;

        Light(const Light&) = delete;
        Light& operator=(const Light&) = delete;

        /*!
         * @brief Light type getter
         * 
         * @return Light type
         */
        Type type() const { return m_type; }

    protected:
        /*! Light type */
        Type m_type;

        /*!
         * @brief Class constructor
         */
        Light();

    };
}

}

#endif
