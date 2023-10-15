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

#ifndef RGBACOLOR_HPP_INCLUDED
#define RGBACOLOR_HPP_INCLUDED

#include <cstdint>

#include "ares/glutils/LinearAlgebra.hpp"

namespace ares
{

namespace glutils
{
    /*!
     * @brief Class representing a RGBA color
     * 
     * This is a utility class that can be used to store a color
     * with RGBA components as floats.
     * It provides some utilities to create and convert colors.
     */
    class RGBAColor
    {
    public:
        /*!
         * @brief Default class constructor
         * 
         * Initializes all components to 0 (transparent black color)
         */
        RGBAColor()
            : m_red(0.F)
            , m_green(0.F)
            , m_blue(0.F)
            , m_alpha(0.F)
        {
        }

        /*!
         * @brief Class constructor for float values
         * 
         * @param[in] r - Red component
         * @param[in] g - Green component
         * @param[in] b - Blue component
         * @param[in] a - Alpha component
         */
        RGBAColor(float r, float g, float b, float a)
            : m_red(r)
            , m_green(g)
            , m_blue(b)
            , m_alpha(a)
        {
        }

        /*!
         * @brief Class constructor for double values
         * 
         * @param[in] r - Red component
         * @param[in] g - Green component
         * @param[in] b - Blue component
         * @param[in] a - Alpha component
         */
        RGBAColor(double r, double g, double b, double a)
            : m_red(static_cast<float>(r))
            , m_green(static_cast<float>(g))
            , m_blue(static_cast<float>(b))
            , m_alpha(static_cast<float>(a))
        {
        }

        /*!
         * @brief Class constructor for uint8 values
         * 
         * @param[in] r - Red component
         * @param[in] g - Green component
         * @param[in] b - Blue component
         * @param[in] a - Alpha component
         */
        RGBAColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
            : m_red(static_cast<float>(r) / 255.F)
            , m_green(static_cast<float>(g) / 255.F)
            , m_blue(static_cast<float>(b) / 255.F)
            , m_alpha(static_cast<float>(a) / 255.F)
        {
        }

        /*!
         * @brief Red component getter
         * 
         * @return Red component
         */
        float red()    const { return m_red;   }

        /*!
         * @brief Green component getter
         * 
         * @return Green component
         */
        float green()  const { return m_green; }

        /*!
         * @brief Blue component getter
         * 
         * @return Blue component
         */
        float blue()   const { return m_blue;  }

        /*!
         * @brief Alpha component getter
         * 
         * @return Alpha component
         */
        float alpha()  const { return m_alpha; }

        /*!
         * @brief Converts color to a Vec4
         * 
         * @return Color in Vec4 format
         */
        Vec4 toVec4() const { return Vec4(m_red, m_green, m_blue, m_alpha); }

        /*!
         * @brief Converts color to a Vec3 (discards alpha)
         * 
         * @return Color in Vec3 format
         */
        Vec3 toVec3() const { return Vec3(m_red, m_green, m_blue); }

    private:
        /*! Red component */
        float m_red;

        /*! Green component */
        float m_green;

        /*! Blue component */
        float m_blue;

        /*! Alpha component */
        float m_alpha;
    };

}

}

#endif
