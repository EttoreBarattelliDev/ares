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

#ifndef DRAWINGCONTEXT_HPP_INCLUDED
#define DRAWINGCONTEXT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <EGL/egl.h>

#include "ares/port/DisplayDevice.hpp"

namespace ares
{

namespace core
{
    class DrawingContext;
    using DrawingContextPtr = std::shared_ptr<DrawingContext>;

    /*!
     * @brief DrawingContext class implementing the context for drawing operations
     * 
     * The DrawingContext class implements the context that is used for any
     * drawing operations. It creates a drawing window and render surface and
     * provides methods to activate/deactivate the context and flush any
     * previous drawing operations on screen.
     * It provides an abstraction for the EGL API. The class constructor performs
     * any initializations needed to create the EGL window, render surface and
     * context. The activate/deactivate methods select the EGL context.
     * The draw method performs a buffer swap operation.
     */
    class DrawingContext
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * This contructor performs all initializations required to create the
         * render window, surface and context from the input native device object.
         * The context is not activated in the constructor, therefore the caller
         * must call the activate method if the context must be activated.
         * 
         * @param[in] device - Native device on which the drawing context must be created
         */
        DrawingContext(port::DisplayDevicePtr device);

        /*!
         * @brief Class destructor
         *
         * This destructor deletes all resources (window, render surface, context)
         * that were created when the object was constructed.
         */
        virtual ~DrawingContext();

        DrawingContext(const DrawingContext&) = delete;
        DrawingContext& operator=(const DrawingContext&) = delete;

        /*!
         * @brief Checks if device is open
         * 
         * @return true if device is open, false otherwise
         */
        bool isDeviceOpen() const { return (port::DisplayDevice::State::Open == m_device->state()); }

        /*!
         * @brief Device getter method
         * 
         * @return Device pointer
         */
        port::DisplayDevicePtr device() const { return m_device; }

        /*!
         * @brief Method to check if context is active
         * 
         * @return Activation state
         */
        bool isActive() const { return m_active; }

        /*!
         * @brief Method to activate the context
         * 
         * This method activates the drawing context, if not already active
         */
        void activate();

        /*!
         * @brief Method to deactivate the context
         * 
         * This method deactivates the drawing context, if not already inactive
         */
        void deactivate();

        /*!
         * @brief Method to finalize the drawing of the context
         * 
         * This method flushes the context and finalizes the drawing on screen.
         */
        void draw() const;

    private:
        /*! Native device associated to the drawing context */
        port::DisplayDevicePtr m_device;

        /*! EGL Display */
        EGLDisplay m_eglDisplay;

        /*! EGL Configuration */
        EGLConfig m_eglConfig;

        /*! EGL Surface */
        EGLSurface m_eglSurface;

        /*! EGL Context */
        EGLContext m_eglContext;

        /*! Flag indicating if context is active */
        bool m_active;

        /*!
         * @brief Helper method to create an EGL Display
         * 
         * This method creates an EGL display with the provided
         * native device object. Throws a runtime error in case of errors
         */
        void createEGLDisplay();

        /*!
         * @brief Helper method to create an EGL configuration
         * 
         * This method creates an EGL configuration (pre-defined).
         * Throws a runtime error in case of errors
         */
        void chooseEGLConfig();

        /*!
         * @brief Helper method to create an EGL Surface
         * 
         * This method creates an EGL surface with the provided
         * native device object. Throws a runtime error in case of errors
         */
        void createEGLSurface();

        /*!
         * @brief Helper method to create an EGL Context
         * 
         * This method creates an EGL context with the provided
         * native device object. Throws a runtime error in case of errors
         */
        void createEGLContext();

        /*!
         * @brief Helper method to terminate the EGL Context
         * 
         * This method deletes the EGL window and all resources created
         * previously. Throws a runtime error in case of errors
         */
        void terminate();

        /*!
         * @brief Helper method to check for EGL errors
         * 
         * This method checks if an EGL error occurred, prints a message
         * in console if so, and throws an exception if the input flag is set.
         */
        static bool checkEGLError(const char* functionLastCalled, bool throwExcpt);
    };
}

}

#endif
