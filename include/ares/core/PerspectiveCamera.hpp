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

#ifndef PERSPECTIVECAMERA_HPP_INCLUDED
#define PERSPECTIVECAMERA_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/core/Camera.hpp"

namespace ares
{

namespace core
{
    class PerspectiveCamera;
    using PerspectiveCameraPtr = std::shared_ptr<PerspectiveCamera>;

    /*!
     * @brief Perspective camera implementation
     * 
     * This class implements a perspective camera and extends the
     * base Camera interface.
     */
    class PerspectiveCamera : public Camera
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a perspective camera based on the provided camera parameters
         * 
         * @param[in] aspectRatio - Aspect ratio of the camera (width / height)
         * @param[in] yfov - Field of view of the camera in the Y direction in radians
         * @param[in] znear - Distance of the near Z plane
         * @param[in] zfar - Distance of the far Z plane
         */
        PerspectiveCamera(float aspectRatio, float yfov, float znear, float zfar);

        /*!
         * @brief Class destructor
         */
        virtual ~PerspectiveCamera() = default;

        PerspectiveCamera(const PerspectiveCamera&) = delete;
        PerspectiveCamera& operator=(const PerspectiveCamera&) = delete;

    protected:
        /*!
         * @brief Method to re-compute and update projection matrix
         * 
         * Base class method override to compute perspective projection matrix
         */
        void updateProjectionMatrix() override;

    private:
        /*! Aspect ratio */
        float m_aspectRatio;

        /*! Y FoV */
        float m_yfov;

        /*! Z near plane */
        float m_znear;

        /*! Z far plane */
        float m_zfar;
    };
}

}

#endif
