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

#ifndef INPUTDEVICE_HPP_INCLUDED
#define INPUTDEVICE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include "ares/port/Event.hpp"

namespace ares
{

namespace port
{
    class InputDevice;
    using InputDevicePtr = std::shared_ptr<InputDevice>;

    /*!
     * @brief InputDevice interface for target-independent input device implementation
     */
    class InputDevice
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
         * This contructor does not do any "real" input
         * initialization, which must be implemented by the derived classes
         * for target-dependent input handling.
         */
        InputDevice()
            : m_state(State::Closed)
        {
        }

        /*!
         * @brief Class destructor
         */
        virtual ~InputDevice() = default;

        InputDevice(const InputDevice&) = delete;
        InputDevice& operator=(const InputDevice&) = delete;

        /*!
         * @brief Method to close the device
         * 
         * This is a virtual method that must be implemented by
         * derived classes to close the device
         */
        virtual void close() = 0;

        /*!
         * @brief Pending events getter
         * 
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the number of pending input events
         * 
         * @return Number of pending events
         */
        virtual int32_t pending() const = 0;

        /*!
         * @brief Next event getter
         *
         * This is a virtual method that must be implemented by
         * derived classes to retrieve the next event
         * 
         * @return Next event
         */
        virtual EventPtr nextEvent() const = 0;

        /*!
         * @brief State getter
         * 
         * @return Device state
         */
        State state() const { return m_state; }

    protected:
        /*! Device state */
        State m_state;
    };
}

}

#endif
