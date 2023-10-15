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

#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <cstdint>
#include <memory>

namespace ares
{

namespace port
{
    class Event;
    using EventPtr = std::shared_ptr<Event>;

    /*!
     * @brief Event class representing an input or internal event,
     *        serving as a base class for specialized event types
     */
    class Event
    {
    public:

        /*! Event type enumeration */
        enum class EventType : uint32_t
        {
            NoEvent = 0x0,
            CloseEv = 0x1,
            AllSystemEvents = 0xF,
            KeyPressEv = 0x10,
            KeyReleaseEv = 0x20,
            AllKeyEvents = 0xF0,
            TouchPressEv = 0x100,
            TouchReleaseEv = 0x200,
            TouchMoveEv = 0x300,
            AllTouchEvents = 0xF00,
            Custom0Ev = 0x1000,
            Custom1Ev = 0x2000,
            Custom2Ev = 0x3000,
            Custom3Ev = 0x4000,
            Custom4Ev = 0x5000,
            Custom5Ev = 0x6000,
            Custom6Ev = 0x7000,
            Custom7Ev = 0x8000,
            Custom8Ev = 0x9000,
            Custom9Ev = 0xA000,
            Custom10Ev = 0xB000,
            Custom11Ev = 0xC000,
            Custom12Ev = 0xD000,
            Custom13Ev = 0xE000,
            AllCustomEvents = 0xF000,
            AllEvents = 0xFFFF,
        };

        /*!
         * @brief Class constructor
         * 
         * @param[in] type - Event type
         */
        Event(EventType type = EventType::NoEvent)
            : m_type(type)
        {
        }

        /*!
         * @brief Class destructor
         */
        virtual ~Event() = default;

        /*!
         * @brief Type getter
         * 
         * @return Event type
         */
        EventType type() const { return m_type; }

        /*!
         * @brief Helper function to convert event to required type
         * 
         * @param[in] ev - Input event
         * @return Pointer to new event type
         */
        template<class T>
        static std::shared_ptr<T> getEvent(EventPtr ev) { return std::dynamic_pointer_cast<T>(ev); }

    protected:
        /*! Event type */
        EventType m_type;

    };

    class SystemEvent;
    using SystemEventPtr = std::shared_ptr<SystemEvent>;

    /*!
     * @brief System event class
     */
    class SystemEvent : public Event
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * @param[in] type - Event type
         */
        SystemEvent(EventType type = EventType::NoEvent)
            : Event(type)
        {
        }

        /*!
         * @brief Class destructor
         */
        virtual ~SystemEvent() = default;
    };

    class KeyEvent;
    using KeyEventPtr = std::shared_ptr<KeyEvent>;

    /*!
     * @brief Key event class
     */
    class KeyEvent : public Event
    {
    public:
        /*! Key type enumeration */
        enum class KeyType
        {
            KeyInvalid,
            KeyA,
            KeyB,
            KeyC,
            KeyD,
            KeyE,
            KeyF,
            KeyG,
            KeyH,
            KeyI,
            KeyJ,
            KeyK,
            KeyL,
            KeyM,
            KeyN,
            KeyO,
            KeyP,
            KeyQ,
            KeyR,
            KeyS,
            KeyT,
            KeyU,
            KeyV,
            KeyW,
            KeyX,
            KeyY,
            KeyZ,
            Key0,
            Key1,
            Key2,
            Key3,
            Key4,
            Key5,
            Key6,
            Key7,
            Key8,
            Key9,
            KeyLeft,
            KeyRight,
            KeyUp,
            KeyDown,
            KeyEsc
        };

        /*!
         * @brief Class constructor
         * 
         * @param[in] type - Event type
         * @param[in] key - Key type
         */
        KeyEvent(EventType type = EventType::NoEvent, KeyType key = KeyType::KeyInvalid)
            : Event(type)
            , m_key(key)
        {
        }

        /*!
         * @brief Class destructor
         */
        virtual ~KeyEvent() = default;

        /*!
         * @brief Key getter
         * 
         * @return Key type
         */
        KeyType key() const { return m_key; }

    private:
        /*! Key */
        KeyType m_key;
    };

    class TouchEvent;
    using TouchEventPtr = std::shared_ptr<TouchEvent>;

    /*!
     * @brief Touch event class
     */
    class TouchEvent : public Event
    {
    public:

        /*! Touch type enumeration */
        enum class TouchType
        {
            TouchInvalid,
            TouchLeft,
            TouchMiddle,
            TouchRight,
            TouchScrollUp,
            TouchScrollDown,
        };

        /*!
         * @brief Class constructor
         * 
         * @param[in] type - Event type
         * @param[in] touchType - Touch type
         * @param[in] x - X coordinate
         * @param[in] y - Y coordinate
         */
        TouchEvent(EventType type = EventType::NoEvent,
                   TouchType touchType = TouchType::TouchInvalid,
                   int32_t x = 0,
                   int32_t y = 0)
            : Event(type)
            , m_touchType(touchType)
            , m_x(x)
            , m_y(y)
        {
        }

        /*!
         * @brief Class destructor
         */
        virtual ~TouchEvent() = default;

        /*!
         * @brief Touch type getter
         * 
         * @return Touch type
         */
        TouchType touchType() const { return m_touchType; }

        /*!
         * @brief X coordinate getter
         * 
         * @return X coordinate
         */
        int32_t x() const { return m_x; }

        /*!
         * @brief Y coordinate getter
         * 
         * @return Y coordinate
         */
        int32_t y() const { return m_y; }

    private:
        /*! Touch type */
        TouchType m_touchType;

        /*! X coordinate */
        int32_t m_x;

        /*! Y coordinate */
        int32_t m_y;
    };

}

}

#endif
