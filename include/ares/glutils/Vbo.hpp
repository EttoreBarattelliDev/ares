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

#ifndef VBO_HPP_INCLUDED
#define VBO_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <GLES2/gl2.h>

namespace ares
{

namespace glutils
{
    class Vbo;
    using VboPtr = std::shared_ptr<Vbo>;

    /*!
     * @brief Vbo class to wrap OpenGL VBO functionality
     * 
     * This class provides a C++ interface for the functionality related
     * to VBO in OpenGL.
     * The class allows to create an OpenGL VBO with buffer data,
     * activate the buffers during a draw operation and free any resources
     * when destroyed.
     */
    class Vbo
    {
    public:
        /*!
         * @brief Target type enumeration
         */
        enum class TargetType
        {
            Invalid = -1,
            ArrayBuffer = GL_ARRAY_BUFFER,
            ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
        };

        /*!
         * @brief Class constructor
         * 
         * This constructor creates an OpenGL buffer object and populates it
         * with the provided data. The buffer is not bound at the end of the constructor.
         * 
         * @param[in] data     - Buffer data to be set in the OpenGL VBO
         * @param[in] dataSize - Buffer size in bytes
         * @param[in] target - Buffer target
         */
        Vbo(const void* data, int32_t dataSize, TargetType target);

        /*!
         * @brief Class destructor
         * 
         * This destructor deletes the OpenGL VBO created for this object
         */
        virtual ~Vbo();

        Vbo() = delete;
        Vbo(const Vbo&) = delete;
        Vbo& operator=(const Vbo&) = delete;

        /*!
         * @brief Method to activate the VBO for drawing
         * 
         * This method binds the OpenGL VBO object
         */
        void activate();

        /*!
         * @brief Method to deactivate the VBO
         * 
         * This method unbinds the OpenGL VBO object
         */
        void deactivate();

        /*!
         * @brief OpenGL VBO ID getter
         */
        GLuint vbo() const { return m_vbo; }

        /*!
         * @brief VBO target
         */
        TargetType target() const { return m_target; }

    private:
        /*! OpenGL VBO object ID */
        GLuint m_vbo;

        /*! Target for the buffer */
        TargetType m_target;
    };
}

}

#endif
