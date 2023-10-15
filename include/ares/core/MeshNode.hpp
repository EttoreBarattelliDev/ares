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

#ifndef MESHNODE_HPP_INCLUDED
#define MESHNODE_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/core/Node.hpp"
#include "ares/core/Mesh.hpp"

namespace ares
{

namespace core
{
    class MeshNode;
    using MeshNodePtr = std::shared_ptr<MeshNode>;

    /*!
     * @brief Node class specialization for nodes containing a mesh
     */
    class MeshNode : public Node
    {
    public:
        /*!
         * @brief Class destructor
         */
        virtual ~MeshNode() = default;

        MeshNode(const MeshNode&) = delete;
        MeshNode& operator=(const MeshNode&) = delete;

        /*!
         * @brief Mesh setter
         * 
         * @param[in] mesh - Mesh to set in the node
         */
        void setMesh(MeshPtr mesh) { m_mesh = mesh; }

        /*!
         * @brief Mesh getter
         * 
         * @return Mesh object
         */
        MeshPtr mesh() const { return m_mesh; }

    private:
        /*! Mesh object */
        MeshPtr m_mesh;

        /*!
         * @brief Class constructor
         */
        MeshNode(const std::string& name, NodePtr parent);

        friend class Scene;
    };
}

}

#endif
