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
#include <unordered_map>
#include "ares/port/X11Input.hpp"

namespace ares
{

namespace port
{
    X11Input::X11Input(X11DisplayPtr display)
        : InputDevice()
        , m_display(display)
    {
        if ((nullptr == m_display) || (DisplayDevice::State::Open != m_display->state()))
        {
            throw std::runtime_error("Invalid display");
        }

        /* Setup the window manager protocols to handle window deletion events */
        m_windowManagerDelete = XInternAtom(m_display->display(), "WM_DELETE_WINDOW", True);
        XSetWMProtocols(m_display->display(), m_display->window(), &m_windowManagerDelete, 1);

        /* Select input */
        XSelectInput(m_display->display(), m_display->window(), 
            KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
            EnterWindowMask | LeaveWindowMask | PointerMotionMask);

        /* Set state */
        m_state = State::Open;
    }

    X11Input::~X11Input()
    {
        /* Close the device */
        close();
    }

    void X11Input::close()
    {
        /* Close only if open */
        if (State::Open == m_state)
        {
            /* Unselect input */
            if ((nullptr != m_display) && (DisplayDevice::State::Open == m_display->state()))
            {
                XSelectInput(m_display->display(), m_display->window(), NoEventMask);
            }

            /* Set state */
            m_state = State::Closed;
        }
    }

    int32_t X11Input::pending() const
    {
        /* Assume failure */
        int32_t retval = -1;

        /* Check device and display are open */
        if (State::Open == m_state)
        {
            if ((nullptr != m_display) && (DisplayDevice::State::Open == m_display->state()))
            {
                /* Get pending events */
                retval = XPending(m_display->display());
            }
        }

        return retval;
    }

    EventPtr X11Input::nextEvent() const
    {
        /* Assume failure */
        EventPtr retval;

        /* Check device and display are open */
        if (
            (State::Open == m_state) &&
            (nullptr != m_display) &&
            (DisplayDevice::State::Open == m_display->state())
           )
        {
            /* Get event */
            XEvent event;
            Display* display = m_display->display();
            XNextEvent(display, &event);
         
            switch (event.type)
            {
            case KeyPress:
            {
                /* Get key and create event */
                KeySym keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
                KeyEvent::KeyType keyType = xKeyToKeyType(keySym);
                retval = std::make_shared<KeyEvent>(Event::EventType::KeyPressEv, keyType);
                break;
            }
            case KeyRelease:
            {
                /* Check if the key was really released */
                bool released = true;
                if (XEventsQueued(display, QueuedAfterReading))
                {
                    XEvent nev;
                    XPeekEvent(display, &nev);

                    released = (nev.type != KeyPress) ||
                               (nev.xkey.time != event.xkey.time) ||
                               (nev.xkey.keycode != event.xkey.keycode);
                }
                
                if (released)
                {
                    /* Get key and create event */
                    KeySym keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
                    KeyEvent::KeyType keyType = xKeyToKeyType(keySym);
                    retval = std::make_shared<KeyEvent>(Event::EventType::KeyReleaseEv, keyType);
                }
                break;
            }
            case ButtonPress:
            {
                /* Get button and create event */
                TouchEvent::TouchType touchType = xButtonToTouchType(event.xbutton.button);
                retval = std::make_shared<TouchEvent>(Event::EventType::TouchPressEv, touchType, event.xbutton.x, event.xbutton.y);
                break;
            }
            case ButtonRelease:
            {
                /* Get button and create event */
                TouchEvent::TouchType touchType = xButtonToTouchType(event.xbutton.button);
                retval = std::make_shared<TouchEvent>(Event::EventType::TouchReleaseEv, touchType, event.xbutton.x, event.xbutton.y);
                break;
            }
            case MotionNotify:
            {
                /* Get coordinates and create event */
                retval = std::make_shared<TouchEvent>(Event::EventType::TouchMoveEv, TouchEvent::TouchType::TouchInvalid, event.xmotion.x, event.xmotion.y);
                break;
            }
            case ClientMessage:
            {
                /* Check if it is a window manager delete event */
                if (event.xclient.data.l[0] == m_windowManagerDelete)
                {
                    /* Create event */
                    retval = std::make_shared<SystemEvent>(Event::EventType::CloseEv);

                    //TODO This is a shortcut, someone else should be responsible to close the display
                    m_display->close();
                }
                break;
            }
            default:
            {
                /* Create an invalid event for this unsupported case */
                retval = std::make_shared<Event>();
                break;
            }
            }
        }

        return retval;
    }

    KeyEvent::KeyType X11Input::xKeyToKeyType(KeySym keySym)
    {
        static const std::unordered_map<KeySym, KeyEvent::KeyType> s_keyMap =
        {
            { XK_a,      KeyEvent::KeyType::KeyA     },
            { XK_b,      KeyEvent::KeyType::KeyB     },
            { XK_c,      KeyEvent::KeyType::KeyC     },
            { XK_d,      KeyEvent::KeyType::KeyD     },
            { XK_e,      KeyEvent::KeyType::KeyE     },
            { XK_f,      KeyEvent::KeyType::KeyF     },
            { XK_g,      KeyEvent::KeyType::KeyG     },
            { XK_h,      KeyEvent::KeyType::KeyH     },
            { XK_i,      KeyEvent::KeyType::KeyI     },
            { XK_j,      KeyEvent::KeyType::KeyJ     },
            { XK_k,      KeyEvent::KeyType::KeyK     },
            { XK_l,      KeyEvent::KeyType::KeyL     },
            { XK_m,      KeyEvent::KeyType::KeyM     },
            { XK_n,      KeyEvent::KeyType::KeyN     },
            { XK_o,      KeyEvent::KeyType::KeyO     },
            { XK_p,      KeyEvent::KeyType::KeyP     },
            { XK_q,      KeyEvent::KeyType::KeyQ     },
            { XK_r,      KeyEvent::KeyType::KeyR     },
            { XK_s,      KeyEvent::KeyType::KeyS     },
            { XK_t,      KeyEvent::KeyType::KeyT     },
            { XK_u,      KeyEvent::KeyType::KeyU     },
            { XK_v,      KeyEvent::KeyType::KeyV     },
            { XK_w,      KeyEvent::KeyType::KeyW     },
            { XK_x,      KeyEvent::KeyType::KeyX     },
            { XK_y,      KeyEvent::KeyType::KeyY     },
            { XK_z,      KeyEvent::KeyType::KeyZ     },
            { XK_0,      KeyEvent::KeyType::Key0     },
            { XK_1,      KeyEvent::KeyType::Key1     },
            { XK_2,      KeyEvent::KeyType::Key2     },
            { XK_3,      KeyEvent::KeyType::Key3     },
            { XK_4,      KeyEvent::KeyType::Key4     },
            { XK_5,      KeyEvent::KeyType::Key5     },
            { XK_6,      KeyEvent::KeyType::Key6     },
            { XK_7,      KeyEvent::KeyType::Key7     },
            { XK_8,      KeyEvent::KeyType::Key8     },
            { XK_9,      KeyEvent::KeyType::Key9     },
            { XK_Left,   KeyEvent::KeyType::KeyLeft  },
            { XK_Right,  KeyEvent::KeyType::KeyRight },
            { XK_Up,     KeyEvent::KeyType::KeyUp    },
            { XK_Down,   KeyEvent::KeyType::KeyDown  },
            { XK_Escape, KeyEvent::KeyType::KeyEsc   }

        };

        KeyEvent::KeyType retval = KeyEvent::KeyType::KeyInvalid;

        if (s_keyMap.end() != s_keyMap.find(keySym))
        {
            retval = s_keyMap.at(keySym);
        }

        return retval;
    }

    TouchEvent::TouchType X11Input::xButtonToTouchType(unsigned long button)
    {
        static const std::unordered_map<unsigned long, TouchEvent::TouchType> s_btnMap =
        {
            { 1, TouchEvent::TouchType::TouchLeft       },
            { 2, TouchEvent::TouchType::TouchMiddle     },
            { 3, TouchEvent::TouchType::TouchRight      },
            { 4, TouchEvent::TouchType::TouchScrollUp   },
            { 5, TouchEvent::TouchType::TouchScrollDown },
        };

        TouchEvent::TouchType retval = TouchEvent::TouchType::TouchInvalid;

        if (s_btnMap.end() != s_btnMap.find(button))
        {
            retval = s_btnMap.at(button);
        }

        return retval;
    }
}

}
