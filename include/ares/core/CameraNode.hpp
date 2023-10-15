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

#ifndef CAMERANODE_HPP_INCLUDED
#define CAMERANODE_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/core/Node.hpp"
#include "ares/core/Camera.hpp"

namespace ares
{

namespace core
{
    class CameraNode;
    using CameraNodePtr = std::shared_ptr<CameraNode>;

    /*!
     * @brief Node class specialization for nodes containing a camera
     */
    class CameraNode : public Node
    {
    public:
        /*!
         * @brief Class destructor
         */
        virtual ~CameraNode() = default;

        CameraNode(const CameraNode&) = delete;
        CameraNode& operator=(const CameraNode&) = delete;

        /*!
         * @brief Camera setter
         * 
         * @param[in] camera - Camera to set in the node
         */
        void setCamera(CameraPtr camera) { m_camera = camera; }

        /*!
         * @brief Camera getter
         * 
         * @return camera object
         */
        CameraPtr camera() const { return m_camera; }

    private:
        /*! Camera object */
        CameraPtr m_camera;

        /*!
         * @brief Class constructor
         */
        CameraNode(const std::string& name, NodePtr parent);

        friend class Scene;
    };
}

}

#endif
