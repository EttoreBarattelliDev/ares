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

#include "ares/core/EventDispatcher.hpp"

namespace ares
{

namespace core
{
    EventDispatcher::EventDispatcher(port::InputDevicePtr device)
        : m_device(device)
        , m_nextHandle(0)
        , m_clbkMap()
    {
    }

    EventDispatcher::Handle EventDispatcher::createHandle()
    {
        /* Increment handle and add to map with empty callback */
        Handle handle = m_nextHandle++;
        m_clbkMap.emplace(handle, std::make_pair(EventCallback(), port::Event::EventType()));
        return handle;
    }

    void EventDispatcher::destroyHandle(Handle handle)
    {
        /* Erase handle entry from map */
        if (m_clbkMap.end() != m_clbkMap.find(handle))
        {
            m_clbkMap.erase(handle);
        }
    }

    void EventDispatcher::registerHandler(Handle handle, EventCallback clbk, port::Event::EventType filter)
    {
        /* Save callback in the map */
        m_clbkMap[handle] = std::make_pair(clbk, filter);
    }

    void EventDispatcher::unregisterHandler(Handle handle)
    {
        /* Reset callback in the map */
        m_clbkMap[handle] = std::make_pair(EventCallback(), port::Event::EventType());
    }

    void EventDispatcher::processEvents() const
    {
        /* Get all events from device and dispatch */
        while (
               (nullptr != m_device) &&
               (port::InputDevice::State::Open == m_device->state()) &&
               (m_device->pending() > 0)
              )
        {
            auto ev = m_device->nextEvent();
            dispatchEvent(ev);
        }
    }

    void EventDispatcher::dispatchEvent(port::EventPtr event) const
    {
        if (nullptr != event)
        {
            /* Get event type */
            uint32_t evType = static_cast<uint32_t>(event->type());

            /* Iterate through all handlers */
            for (auto it = m_clbkMap.begin(); it != m_clbkMap.end(); it++)
            {
                /* Check if handler needs the event */
                const auto& func = it->second.first;
                const auto& filter = it->second.second;
                if (
                    (func) &&
                    (0 != (evType & static_cast<uint32_t>(filter)))
                   )
                {
                    /* Trigger handler */
                    func(event);
                }
            }
        }
    }
}

}
