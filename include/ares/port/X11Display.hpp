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

#ifndef X11DISPLAY_HPP_INCLUDED
#define X11DISPLAY_HPP_INCLUDED

#include "ares/port/DisplayDevice.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace ares
{

namespace port
{
    class X11Display;
    using X11DisplayPtr = std::shared_ptr<X11Display>;

    /*!
     * @brief X11Display interface for X11 device implementation
     * 
     * The X11Display class implements the DisplayDevice interface
     * and can be used to create a window via the X11 client library..
     */
    class X11Display : public DisplayDevice
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * This contructor creates the X11 window that will be used
         * for rendering. If an error occurs when creating the X11
         * display or window, a runtime_error exception is thrown.
         * 
         * @param[in] width  - Window width
         * @param[in] height - Window height
         */
        X11Display(int32_t width, int32_t height);

        /*!
         * @brief Class destructor
         */
        virtual ~X11Display();

        X11Display(const X11Display&) = delete;
        X11Display& operator=(const X11Display&) = delete;


        /*!
         * @brief Method to close the display
         * 
         * This is a virtual method that must be implemented by
         * derived classes to close the native display
         */
        void close() override;

        /*!
         * @brief EGL native display type getter
         * 
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the native display type
         * to be used for EGL initialization
         * 
         * @return native display type to be used for EGL initialization
         */
        EGLNativeDisplayType eglNativeDisplayType() const override;

        /*!
         * @brief EGL native display type getter
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the native display type
         * to be used for EGL initialization
         * 
         * @return native display type to be used for EGL initialization
         */
        EGLNativeWindowType  eglNativeWindowType()  const override;

        /*!
         * @brief X11 display getter
         * 
         * @return X11 display
         */
        Display* display() const { return m_display; }

        /*!
         * @brief X11 window getter
         * 
         * @return X11 window
         */
        Window window() const { return m_window; }

    private:
        /*! X11 Display object pointer */
        Display* m_display;

        /* X11 Window ID */
        Window m_window;

        /*!
         * @brief Utility method to create a display via the X11
         *        client library
         * 
         * This method is called by the class constructor to open the
         * default display via the X11 client library. If an error
         * occurs, a runtime_error exception is thrown.
         */
        void createDisplay();

        /*!
         * @brief Utility method to create a window via the X11
         *        client library
         * 
         * This method is called by the class constructor to create a
         * window via the X11 client library. If an error
         * occurs, a runtime_error exception is thrown.
         */
        void createWindow();
    };
}

}

#endif
