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

#ifndef EVENTDISPATCHER_HPP_INCLUDED
#define EVENTDISPATCHER_HPP_INCLUDED

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

#include "ares/port/InputDevice.hpp"

namespace ares
{

namespace core
{

#define BIND_CALLBACK(x) (std::bind((x), this, std::placeholders::_1))

    class EventDispatcher;
    using EventDispatcherPtr = std::shared_ptr<EventDispatcher>;

    /*!
     * @brief Event dispatcher class to process and distribute events to handlers
     *
     * This class can be used to create internal communication mechanisms between
     * objects. It can optionally have an InputDevice to get events from the
     * system. Objects can register for specific event types and their callback
     * is called when events of that type are dispatched through the dispatcher.
     */
    class EventDispatcher
    {
    public:
        using EventCallback = std::function<void(port::EventPtr)>;
        using EventClbkPair = std::pair<EventCallback, port::Event::EventType>;
        using Handle = int32_t;

        /*!
         * @brief Class constructor
         *
         * @param[in] device - Optional input device
         */
        EventDispatcher(port::InputDevicePtr device = nullptr);

        /*!
         * @brief Class destructor
         */
        virtual ~EventDispatcher() = default;

        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;

        /*!
         * @brief Method to create a handle to register/unregister callbacks
         *
         * @return Created handle
         */
        Handle createHandle();

        /*!
         * @brief Method to destroy a handle
         *
         * @param[in] handle - Handle to destroy
         */
        void destroyHandle(Handle handle);

        /*!
         * @brief Method to register a callback
         *
         * This method can be used to register a callback for specific event types.
         * When an event matching the provided filter is received from the input device
         * or through the dispatchEvent method, the callback is called synchronously.
         * If a callback is already registered with the same handle, it will be overwritten.
         * 
         * @param[in] handle - Handle to register the callback
         * @param[in] clbk - Callback function to register
         * @param[in] filter - Event filter to select events to trigger the callback
         */
        void registerHandler(Handle handle, EventCallback clbk, port::Event::EventType filter = port::Event::EventType::AllEvents);

        /*!
         * @brief Method to unregister a callback
         *
         * @param[in] handle - Handle to unregister the callback
         */
        void unregisterHandler(Handle handle);

        /*!
         * @brief Retrieves and dispatches all events from input device.
         *
         * This method retrieves all pending events from the input device and
         * dispatches them through the dispatchEvent method.
         * If the input device is invalid or closed, this method does nothing.
         */
        void processEvents() const;

        /*!
         * @brief Dispatches an event to the handlers
         *
         * This method calls all registered callback that have a filter
         * matching the event type. The callbacks are called synchronously in
         * the context of this method.
         */
        void dispatchEvent(port::EventPtr event) const;

    private:
        /*! Input device */
        port::InputDevicePtr m_device;

        /*! Next available handle */
        Handle m_nextHandle;

        /*! Mesh name */
        std::unordered_map<Handle, EventClbkPair> m_clbkMap;
    };
}

}

#endif
