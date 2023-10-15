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

#include "ares/core/Scene.hpp"

namespace ares
{

namespace core
{
    Scene::Scene(const std::string& name, DrawingContextPtr drawingContext)
        : m_name(name)
        , m_drawingContext(drawingContext)
        , m_rootNode(NodePtr(new Node(std::string(), nullptr)))
    {
        /* Check for valid drawing context */
        if (nullptr == m_drawingContext)
        {
            throw std::runtime_error("Invalid drawing context");
        }

        /* Check for valid root node */
        if (nullptr == m_rootNode)
        {
            throw std::runtime_error("Could not create root node for scene");
        }
    }

    void Scene::activate()
    {
        /* Activate context */
        if (nullptr != m_drawingContext)
        {
            m_drawingContext->activate();
        }
    }

    void Scene::deactivate()
    {
        /* Deactivate context */
        if (nullptr != m_drawingContext)
        {
            m_drawingContext->deactivate();
        }
    }

    std::vector<LightNodePtr> Scene::getLightNodes() const
    {
        /* Parse all nodes for lights starting from root */
        std::vector<LightNodePtr> retval;
        parseNodeForLight(m_rootNode, retval);
        return retval;
    }

    void Scene::parseNodeForLight(NodePtr node, std::vector<LightNodePtr>& lightVec) const
    {
        /* Check for valid node */
        if (nullptr != node)
        {
            /* Check if it is a light node */
            if (Node::Type::Light == node->type())
            {
                /* Add light node to vector */
                LightNodePtr lightNodePtr = std::dynamic_pointer_cast<LightNode>(node);
                lightVec.push_back(lightNodePtr);
            }

            /* Recursion on children */
            for (auto& child : node->children())
            {
                parseNodeForLight(child, lightVec);
            }
        }
    }

}

}
