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

#include "ares/core/Node.hpp"

namespace ares
{

namespace core
{
    Node::Node(const std::string& name, NodePtr parent)
        : m_name(name)
        , m_type(Type::Empty)
        , m_position(0.F, 0.F, 0.F)
        , m_rotation(0.F, 0.F, 0.F, 1.0F)
        , m_scaling(1.F, 1.F, 1.F)
        , m_transformMatrix()
        , m_parent(parent)
        , m_children()
    {
        /* Initialize transform to an identity */
        m_transformMatrix.setIdentity();
    }

    void Node::setPosition(float x, float y, float z)
    {
        /* Store value and update matrix */
        m_position = glutils::Vec3(x, y, z);
        updateTransformMatrix();
    }

    void Node::setRotationEuler(float x, float y, float z)
    {
        /* Store value and update matrix */
        m_rotation = glutils::eulerToQuaternion(glutils::Vec3(x, y, z));
        updateTransformMatrix();
    }

    void Node::setRotationQuaternion(float x, float y, float z, float w)
    {
        /* Store value and update matrix */
        m_rotation = glutils::Vec4(x, y, z, w);
        updateTransformMatrix();
    }

    void Node::setScaling(float x, float y, float z)
    {
        /* Store value and update matrix */
        m_scaling = glutils::Vec3(x, y, z);
        updateTransformMatrix();
    }

    glutils::Mat4 Node::totalTransformMatrix() const
    {
        /* Start from current transform */
        glutils::Mat4 retval(m_transformMatrix);

        /* Get parent if any */
        NodePtr parentNode = parent();
        if (nullptr != parentNode)
        {
            /* Apply parent transform */
            retval = parentNode->totalTransformMatrix() * retval;
        }

        return retval;
    }

    void Node::updateTransformMatrix()
    {
        /* Apply TRS */
        m_transformMatrix.setIdentity();
        m_transformMatrix.scale(m_scaling[0], m_scaling[1], m_scaling[2]);
        m_transformMatrix.rotateXYZW(m_rotation[0], m_rotation[1], m_rotation[2], m_rotation[3]);
        m_transformMatrix.translate(m_position[0], m_position[1], m_position[2]);
    }

    void Node::addChild(NodePtr node)
    {
        /* Add child to list */
        m_children.push_back(node);
    }
}

}
