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

#ifndef DISPLAYDEVICE_HPP_INCLUDED
#define DISPLAYDEVICE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <EGL/egl.h>

namespace ares
{

namespace port
{
    class DisplayDevice;
    using DisplayDevicePtr = std::shared_ptr<DisplayDevice>;

    /*!
     * @brief DisplayDevice interface for target-independent device implementation
     * 
     * The DisplayDevice class defines a generic interface for a
     * rendering device. This class must be derived to implement a
     * target-specific rendering device.
     * The constructor of the derived class should implement the logic
     * to create a display and/or a window for the rendering.
     * The derived class must also implement the eglNativeDisplayType and
     * eglNativeWindowType methods to return the EGL native display and
     * window types used for EGL initialization.
     */
    class DisplayDevice
    {
    public:
        /*! Device state enumeration */
        enum class State
        {
            Closed,
            Open
        };

        /*!
         * @brief Class constructor
         * 
         * This contructor does not do any "real" display or window
         * initialization, which must be implemented by the derived classes
         * for target-dependent window creation.
         * 
         * @param[in] width  - Window width
         * @param[in] height - Window height
         */
        DisplayDevice(int32_t width, int32_t height)
        : m_width(width)
        , m_height(height)
        , m_state(State::Closed)
        {
        }

        /*!
         * @brief Class destructor
         */
        virtual ~DisplayDevice() = default;

        DisplayDevice(const DisplayDevice&) = delete;
        DisplayDevice& operator=(const DisplayDevice&) = delete;

        /*!
         * @brief Width getter method
         * 
         * @return Device width
         */
        int32_t width() const { return m_width; }

        /*!
         * @brief Height getter method
         * 
         * @return Device height
         */
        int32_t height() const { return m_height; }

        /*!
         * @brief Method to close the display
         * 
         * This is a virtual method that must be implemented by
         * derived classes to close the native display
         */
        virtual void close() = 0;

        /*!
         * @brief EGL native display type getter
         * 
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the native display type
         * to be used for EGL initialization
         * 
         * @return native display type to be used for EGL initialization
         */
        virtual EGLNativeDisplayType eglNativeDisplayType() const = 0;

        /*!
         * @brief EGL native display type getter
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the native display type
         * to be used for EGL initialization
         * 
         * @return native display type to be used for EGL initialization
         */
        virtual EGLNativeWindowType  eglNativeWindowType()  const = 0;

        /*!
         * @brief State getter
         * 
         * @return Display state
         */
        State state() const { return m_state; }

    protected:
        /*! Window width */
        int32_t m_width;

        /*! Window height */
        int32_t m_height;

        /*! Display state */
        State m_state;
    };
}

}

#endif
