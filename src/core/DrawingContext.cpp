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

#include "ares/core/DrawingContext.hpp"

#include <iostream>
#include <stdexcept>

namespace ares
{

namespace core
{
    DrawingContext::DrawingContext(port::DisplayDevicePtr device)
        : m_device(device)
        , m_eglDisplay(EGL_NO_DISPLAY)
        , m_eglConfig()
        , m_eglSurface(EGL_NO_SURFACE)
        , m_eglContext(EGL_NO_CONTEXT)
        , m_active(false)
    {
        /* Check device object validity */
        if ((nullptr == m_device) || (port::DisplayDevice::State::Closed == m_device->state()))
        {
            throw std::runtime_error("Invalid device for DrawingContext");
        }

        /* Create all needed objects */
        createEGLDisplay();
        chooseEGLConfig();
        createEGLSurface();
        createEGLContext();
        activate();
    }

    DrawingContext::~DrawingContext()
    {
        /* Deactivate and terminate context */
        deactivate();
        terminate();
    }

    void DrawingContext::activate()
    {
        /* Check context is not already active */
        if (!m_active)
        {
            /* Make context current */
            eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
            checkEGLError("eglMakeCurrent", true);
            m_active = true;
        }
    }

    void DrawingContext::deactivate()
    {
        /* Check context is actually active */
        if (m_active)
        {
            /* Make no context active */
            eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            checkEGLError("eglMakeCurrent", true);
            m_active = false;
        }
    }

    void DrawingContext::draw() const
    {
        /* Swap buffers to refresh screen */
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
        checkEGLError("eglSwapBuffers", true);
    }

    void DrawingContext::createEGLDisplay()
    {
        /* Get EGL display from native display */
        m_eglDisplay = eglGetDisplay(m_device->eglNativeDisplayType());
        
        /* Check we could get a valid EGL display */
        if (m_eglDisplay == EGL_NO_DISPLAY)
        {
            throw std::runtime_error("Failed to get an EGLDisplay");
        }

        /* Initialize EGL */
        EGLint eglMajorVersion = 0;
        EGLint eglMinorVersion = 0;
        if (!eglInitialize(m_eglDisplay, &eglMajorVersion, &eglMinorVersion))
        {
            throw std::runtime_error("Failed to initialize the EGLDisplay");
        }

        /* Bind the OpenGL ES API */
        const int32_t result = eglBindAPI(EGL_OPENGL_ES_API);
        if (result != EGL_TRUE)
        {
            throw std::runtime_error("Failed to bind OpenGL ES API");
        }
    }

    void DrawingContext::chooseEGLConfig()
    {
        /* Choose configuration */
        //TODO Make this configurable by user
        const EGLint configurationAttributes[] = {
                                                   EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
                                                   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                                                   EGL_DEPTH_SIZE, 16,
                                                   EGL_SAMPLE_BUFFERS, 1,
                                                   EGL_NONE
                                                 };
        EGLint configsReturned;
        EGLBoolean ret = eglChooseConfig(m_eglDisplay, configurationAttributes, &m_eglConfig, 1, &configsReturned);
        if (!ret || (configsReturned != 1))
        {
            throw std::runtime_error("Failed to choose a suitable config");
        }
    }

    void DrawingContext::createEGLSurface()
    {
        /* Create EGL surface from native device */
        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_device->eglNativeWindowType(), NULL);
        checkEGLError("eglCreateWindowSurface", true);
    }

    void DrawingContext::createEGLContext()
    {
        /* Bind OpenGL ES API */
        eglBindAPI(EGL_OPENGL_ES_API);
        checkEGLError("eglBindAPI", true);

        /* Create EGL context */
        const EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
        m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, contextAttributes);
        checkEGLError("eglCreateContext", true);
    }

    void DrawingContext::terminate()
    {
        /* Terminate display if valid */
        if (EGL_NO_DISPLAY != m_eglDisplay)
        {
            eglTerminate(m_eglDisplay);
        }
    }

    bool DrawingContext::checkEGLError(const char* functionLastCalled, bool throwExcpt)
    {
        /* Get last error and check if there is any */
        const EGLint lastError = eglGetError();
        if (lastError != EGL_SUCCESS)
        {
            /* Print message and throw exception if needed */
            std::cout << functionLastCalled << " failed (" << lastError << ")" << std::endl;
            if (throwExcpt)
            {
                throw std::runtime_error("EGL Error");
            }
            return false;
        }
        return true;
    }

}

}
