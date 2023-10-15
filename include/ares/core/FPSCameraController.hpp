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

#ifndef FPSCAMERACONTROLLER_HPP_INCLUDED
#define FPSCAMERACONTROLLER_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/core/CameraNode.hpp"
#include "ares/core/EventDispatcher.hpp"

namespace ares
{

namespace core
{
    class FPSCameraController;
    using FPSCameraControllerPtr = std::shared_ptr<FPSCameraController>;

    /*!
     * @brief Class that implements a FPS-style camera
     *
     * This class implements a controller to manipulate the camera using
     * WASD key and mouse events, in the style of a first person shooter
     */
    class FPSCameraController
    {
    public:
        /*!
         * @brief Class constructor
         *
         * @param[in] dispatcher - Event dispatcher
         * @param[in] node - Camera node to control
         */
        FPSCameraController(EventDispatcherPtr dispatcher, CameraNodePtr node);

        /*!
         * @brief Class destructor
         */
        virtual ~FPSCameraController();

        FPSCameraController(const FPSCameraController&) = delete;
        FPSCameraController& operator=(const FPSCameraController&) = delete;

        /*!
         * @brief Processes events received previously and updates
         *        the camera transform accordingly. Must be called
         *        periodically in the main render loop to update
         *        the camera continuously.
         */
        void process();

    private:
        /*! Event dispatcher */
        EventDispatcherPtr m_dispatcher;

        /*! Camera node */
        CameraNodePtr m_cameraNode;

        /*! Handle for callback */
        EventDispatcher::Handle m_handle;

        /* Event data */
        bool m_fwdPressed;
        bool m_backPressed;
        bool m_leftPressed;
        bool m_rightPressed;
        int32_t m_lastX;
        int32_t m_lastY;
        int32_t m_nextX;
        int32_t m_nextY;
        glutils::Vec3 m_rotation;

        /*!
         * @brief Event callback for key and touch events
         *
         * @param[in] event - Input event
         */
        void eventCallback(port::EventPtr event);
    };
}

}

#endif
