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

#include "ares/core/FPSCameraController.hpp"

#include <stdexcept>

namespace ares
{

namespace core
{
    FPSCameraController::FPSCameraController(EventDispatcherPtr dispatcher, CameraNodePtr node)
        : m_dispatcher(dispatcher)
        , m_cameraNode(node)
        , m_handle()
        , m_fwdPressed(false)
        , m_backPressed(false)
        , m_leftPressed(false)
        , m_rightPressed(false)
        , m_lastX(0)
        , m_lastY(0)
        , m_nextX(0)
        , m_nextY(0)
        , m_rotation()
    {
        /* Check validity */
        if ((nullptr == m_dispatcher) || (nullptr == m_cameraNode))
        {
            throw std::runtime_error("Invalid dispatcher or node");
        }

        /* Register event callback */
        port::Event::EventType filter =
            static_cast<port::Event::EventType>(
                static_cast<uint32_t>(port::Event::EventType::AllKeyEvents) |
                static_cast<uint32_t>(port::Event::EventType::AllTouchEvents)
            );
        m_handle = m_dispatcher->createHandle();
        m_dispatcher->registerHandler(m_handle, BIND_CALLBACK(&FPSCameraController::eventCallback), filter);
    }

    FPSCameraController::~FPSCameraController()
    {
        /* Unregister callback */
        m_dispatcher->unregisterHandler(m_handle);
        m_dispatcher->destroyHandle(m_handle);
    }

    void FPSCameraController::eventCallback(port::EventPtr event)
    {
        /* Check validity */
        if (nullptr == event)
        {
            return;
        }

        switch (event->type())
        {
        case port::Event::EventType::KeyPressEv:
        case port::Event::EventType::KeyReleaseEv:
        {
            /* Key event, update direction flags */
            auto keyEv = port::Event::getEvent<port::KeyEvent>(event);
            switch (keyEv->key())
            {
            case port::KeyEvent::KeyType::KeyW:
                m_fwdPressed = (port::Event::EventType::KeyPressEv == keyEv->type());
                break;
            case port::KeyEvent::KeyType::KeyA:
                m_leftPressed = (port::Event::EventType::KeyPressEv == keyEv->type());
                break;
            case port::KeyEvent::KeyType::KeyS:
                m_backPressed = (port::Event::EventType::KeyPressEv == keyEv->type());
                break;
            case port::KeyEvent::KeyType::KeyD:
                m_rightPressed = (port::Event::EventType::KeyPressEv == keyEv->type());
                break;
            default:
                break;
            }
            break;
        }
        case port::Event::EventType::TouchMoveEv:
        {
            static bool s_first = true;

            /* Touch event, store coordinates */
            auto touchEv = port::Event::getEvent<port::TouchEvent>(event);

            /* If these are the first coordinates, save previous as well */
            if (s_first)
            {
                m_lastX = touchEv->x();
                m_lastY = touchEv->y();
                s_first = false;
            }

            /* Save coordinates */
            m_nextX = touchEv->x();
            m_nextY = touchEv->y();

            break;
        }
        default:
            break;
        }
    }

    void FPSCameraController::process()
    {
        constexpr float PI = static_cast<float>(M_PI);
        constexpr float PI2 = static_cast<float>(M_PI_2);
        
        /* Get camera transform */
        auto xform = m_cameraNode->transformMatrix();
        auto translation = xform.translation();

        /* Reset */
        xform.setIdentity();

        /* Get delta from previous position */
        int32_t deltaX = m_nextX - m_lastX;
        int32_t deltaY = m_nextY - m_lastY;
        m_lastX = m_nextX;
        m_lastY = m_nextY;

        /* Convert into some rotation angles */
        //TODO these are quick values to make something work, needs calibration and configuration
        float angY = static_cast<float>(deltaX) / 1000.F * PI;
        float angX = static_cast<float>(deltaY) / 400.F * PI;

        /* Update rotation angles, clamping rotation on X between -PI/2 and PI/2*/
        m_rotation[0] -= angX;
        m_rotation[0] = std::min(std::max(m_rotation[0], static_cast<float>(-PI2)), static_cast<float>(PI2));
        m_rotation[1] -= angY;

        /* Rotate (YXZ order) and restore translation */
        xform.rotateX(m_rotation[0]);
        xform.rotateY(m_rotation[1]);
        xform.translate(translation[0], translation[1], translation[2]);

        /* Determine direction based on pressed keys */
        float moveZ = 0.F;
        moveZ += (m_fwdPressed)  ? (-.05F) : (0.F);
        moveZ += (m_backPressed) ? (.05F) : (0.F);
        float moveX = 0.F;
        moveX += (m_leftPressed)  ? (-.05F) : (0.F);
        moveX += (m_rightPressed) ? (.05F) : (0.F);

        /* Translate in the local coordinate system (i.e. after previous transforms) */
        xform.translateLocalXZ(moveX, moveZ);

        /* Apply new transform to camera node */
        m_cameraNode->setTransformMatrix(xform);
    }
}

}
