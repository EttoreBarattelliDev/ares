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

#ifndef X11INPUT_HPP_INCLUDED
#define X11INPUT_HPP_INCLUDED

#include "ares/port/InputDevice.hpp"
#include "ares/port/X11Display.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

namespace ares
{

namespace port
{
    class X11Input;
    using X11InputPtr = std::shared_ptr<X11Input>;

    /*!
     * @brief X11Input interface for X11 input implementation
     * 
     * The X11Input class implements the InputDevice interface
     * and can be used to retrieve events from the X11 window
     */
    class X11Input : public InputDevice
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * This contructor registers for events in the provided
         * X11Display window
         * 
         * @param[in] display - X11Display object
         */
        X11Input(X11DisplayPtr display);

        /*!
         * @brief Class destructor
         */
        virtual ~X11Input();

        X11Input(const X11Input&) = delete;
        X11Input& operator=(const X11Input&) = delete;

        /*!
         * @brief Method to close the device
         * 
         * This is a virtual method that must be implemented by
         * derived classes to close the device
         */
        void close() override;

        /*!
         * @brief Pending events getter
         * 
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the number of pending input events
         * 
         * @return Number of pending events
         */
        int32_t pending() const override;

        /*!
         * @brief Next event getter
         *
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the next event
         * 
         * @return Next event
         */
        EventPtr nextEvent() const override;

    private:
        /*! X11Display object pointer */
        X11DisplayPtr m_display;

        /*! Window manager delete */
        Atom m_windowManagerDelete;

        /*!
         * @brief Utility method to convert X11 key to Ares key
         *
         * @param[in] keySym - X11 key
         *
         * @return Ares key type
         */
        static KeyEvent::KeyType xKeyToKeyType(KeySym keySym);

        /*!
         * @brief Utility method to convert X11 button to touch type
         *
         * @param[in] button - X11 button
         *
         * @return Ares touch type
         */
        static TouchEvent::TouchType xButtonToTouchType(unsigned long button);
    };
}

}

#endif
