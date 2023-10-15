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

#include <stdexcept>
#include "ares/port/X11Display.hpp"

namespace ares
{

namespace port
{
    X11Display::X11Display(int32_t width, int32_t height)
        : DisplayDevice(width, height)
        , m_display(nullptr)
        , m_window()
    {
        /* Create display and window */
        createDisplay();
        createWindow();
        m_state = State::Open;
    }

    X11Display::~X11Display()
    {
        close();
    }

    void X11Display::close()
    {
        if (State::Open == m_state)
        {
            /* Destroy the window */
            if (m_window)
            {
                XDestroyWindow(m_display, m_window);
            }

            /* Release the display */
            if (m_display)
            {
                XCloseDisplay(m_display);
            }

            /* Set state */
            m_state = State::Closed;
        }
    }

    void X11Display::createDisplay()
    {
        /* Open the display */
        m_display = XOpenDisplay(nullptr);
        if (nullptr == m_display)
        {
            throw std::runtime_error("Error: Unable to open X display");
        }
    }

    void X11Display::createWindow()
    {
        /* Get the default screen for the display */
        int32_t defaultScreen = XDefaultScreen(m_display);

        /* Get the default depth of the display */
        int32_t defaultDepth = DefaultDepth(m_display, defaultScreen);

        /* Select a visual info */
        XVisualInfo visualInfo;
        Status visualInfoRet = XMatchVisualInfo(m_display, defaultScreen, defaultDepth, TrueColor, &visualInfo);
        if (0 == visualInfoRet)
        {
            throw std::runtime_error("Error: Unable to acquire visual");
        }

        /* Get the root window for the display and default screen */
        Window rootWindow = RootWindow(m_display, defaultScreen);

        /* Create a color map from the display, root window and visual info */
        Colormap colorMap = XCreateColormap(m_display, rootWindow, visualInfo.visual, AllocNone);

        /* Now setup the final window by specifying some attributes */
        XSetWindowAttributes windowAttributes;

        /* Set the color map that was just created */
        windowAttributes.colormap = colorMap;

        /* Set events that will be handled by the app, add to these for other events. */
        windowAttributes.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask;

        /* Create the window */
        m_window = XCreateWindow(m_display, // The display used to create the window
            rootWindow, // The parent (root) window - the desktop
            0, // The horizontal (x) origin of the window
            0, // The vertical (y) origin of the window
            m_width, // The width of the window
            m_height, // The height of the window
            0, // Border size - set it to zero
            visualInfo.depth, // Depth from the visual info
            InputOutput, // Window type - this specifies InputOutput.
            visualInfo.visual, // Visual to use
            CWEventMask | CWColormap, // Mask specifying these have been defined in the window attributes
            &windowAttributes); // Pointer to the window attribute structure

        /* Make the window viewable by mapping it to the display */
        XMapWindow(m_display, m_window);

        /* Set the window title */
        XStoreName(m_display, m_window, "ARES");
    }

    EGLNativeDisplayType X11Display::eglNativeDisplayType() const
    {
        return static_cast<EGLNativeDisplayType>(m_display);
    }

    EGLNativeWindowType X11Display::eglNativeWindowType() const
    {
        return static_cast<EGLNativeWindowType>(m_window);
    }
}

}
