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

#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

#include "ares/glutils/LinearAlgebra.hpp"

namespace ares
{

namespace core
{
    class Node;
    using NodePtr = std::shared_ptr<Node>;

    /*!
     * @brief Class that represents each node in the scene graph.
     * 
     * This class represents a generic node in the scene graph and serves as a
     * base class for specialized node types like mesh, camera and light nodes.
     * The Node class has a pointer to the parent node and a list of child nodes,
     * which allows to build the scene tree structure.
     * The Node class also provides facilities to handle transforms, in terms of
     * translations and rotations and providing the overall transform matrix.
     */
    class Node
    {
    public:
        /*!
         * @brief Node type
         */
        enum class Type
        {
            Empty,   /*!< Empty node  */
            Mesh,    /*!< Mesh node   */
            Camera,  /*!< Camera node */
            Light    /*!< Light node  */
        };

        /*!
         * @brief Class destructor
         */
        virtual ~Node() = default;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        /*!
         * @brief Position setter
         * 
         * @param[in] x - Node x position
         * @param[in] y - Node y position
         * @param[in] z - Node z position
         */
        void setPosition(float x, float y, float z);

        /*!
         * @brief Rotation setter with Euler angles
         * 
         * @param[in] x - Node x rotation
         * @param[in] y - Node y rotation
         * @param[in] z - Node z rotation
         */
        void setRotationEuler(float x, float y, float z);

        /*!
         * @brief Rotation setter with quaternion
         * 
         * @param[in] x - Node x rotation
         * @param[in] y - Node y rotation
         * @param[in] z - Node z rotation
         * @param[in] w - Node w rotation
         */
        void setRotationQuaternion(float x, float y, float z, float w);

        /*!
         * @brief Scaling setter
         * 
         * @param[in] x - Node x scaling
         * @param[in] y - Node y scaling
         * @param[in] z - Node z scaling
         */
        void setScaling(float x, float y, float z);

        /*!
         * @brief Transform matrix setter
         * 
         * @param[in] transformMatrix - Node transform matrix
         */
        void setTransformMatrix(const glutils::Mat4& transformMatrix) { m_transformMatrix = transformMatrix; }

        /*!
         * @brief Name getter
         * 
         * @return Node name
         */
        const std::string& name() const { return m_name; }

        /*!
         * @brief Type getter
         * 
         * @return Node type
         */
        Type type() const { return m_type; }

        /*!
         * @brief Position getter
         * 
         * @return Node position
         */
        const glutils::Vec3& position() const { return m_position; }

        /*!
         * @brief Rotation getter
         * 
         * @return Node rotation
         */
        const glutils::Vec4& rotation() const { return m_rotation; }

        /*!
         * @brief Scaling getter
         * 
         * @return Node scaling
         */
        const glutils::Vec3& scaling() const { return m_scaling; }

        /*!
         * @brief Transform matrix getter
         * 
         * @return Node transform matrix
         */
        const glutils::Mat4& transformMatrix() const { return m_transformMatrix; }

        /*!
         * @brief Transform matrix getter from root node
         * 
         * @return Node transform matrix from root node
         */
        glutils::Mat4 totalTransformMatrix() const;

        /*!
         * @brief Parent node getter
         * 
         * @return Parent node
         */
        NodePtr parent() const { return m_parent.lock(); }

        /*!
         * @brief Child nodes getter
         * 
         * @return List of children
         */
        const std::vector<NodePtr>& children() const { return m_children; }

    protected:
        /*! Node name */
        std::string m_name;

        /*! Node type */
        Type m_type;

        /*! Node position */
        glutils::Vec3 m_position;

        /*! Node rotation */
        glutils::Vec4 m_rotation;

        /*! Node scaling */
        glutils::Vec3 m_scaling;

        /*! Transform matrix */
        glutils::Mat4 m_transformMatrix;

        /*! Node parent */
        std::weak_ptr<Node> m_parent;

        /*! Node children */
        std::vector<NodePtr> m_children;

        /*!
         * @brief Class constructor
         * 
         * This constructor is private as nodes must always be created
         * through the owning Scene object.
         * This constructor creates the node by adding it to the parent
         * list of children and initializing the transform to an identity.
         * 
         * @param[in] name - Node name
         * @param[in] parent - Parent node
         */
        Node(const std::string& name, NodePtr parent);

        /*!
         * @brief Helper method to add a child to the node
         * 
         * @param[in] child - Child node to add
         */
        void addChild(NodePtr child);

        /*!
         * @brief Helper method to update transform matrix
         */
        void updateTransformMatrix();

        friend class Scene;
    };
}

}

#endif
