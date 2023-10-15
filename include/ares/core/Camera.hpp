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

#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/glutils/LinearAlgebra.hpp"

namespace ares
{

namespace core
{
    class Camera;
    using CameraPtr = std::shared_ptr<Camera>;

    /*!
     * @brief Camera class to represent a camera for the scene
     * 
     * This class provides an abstract interface for camera objects.
     * Derived classes must implement and call the updateProjectionMatrix
     * method to store the projection matrix based on the camera type.
     */
    class Camera
    {
    public:
        /*!
         * @brief Class constructor
         */
        Camera();

        /*!
         * @brief Class destructor
         */
        virtual ~Camera() = default;

        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;

        /*!
         * @brief Projection matrix setter
         * 
         * @return Projection matrix
         */
        const glutils::Mat4& projectionMatrix() const { return m_projectionMatrix; }

    protected:
        /*! Projection matrix */
        glutils::Mat4 m_projectionMatrix;

        /*!
         * @brief Method to re-compute and update projection matrix
         * 
         * This method must be implemented and called by derived classes
         * to compute the projection matrix for the camera.
         */
        virtual void updateProjectionMatrix() = 0;
    };
}

}

#endif
