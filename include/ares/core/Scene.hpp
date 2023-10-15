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

#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "ares/core/DrawingContext.hpp"
#include "ares/core/Node.hpp"
#include "ares/core/CameraNode.hpp"
#include "ares/core/LightNode.hpp"

namespace ares
{

namespace core
{
    class Scene;
    using ScenePtr = std::shared_ptr<Scene>;

    /*!
     * @brief Scene class that handles the scene graph
     * 
     * This class is the entry point for the scene graph.
     * It provides methods to create and manage nodes; all node objects
     * must be created using the owning scene object interfaces.
     * All nodes within a given scene must have unique names.
     * The scene must be activated before any other operation on nodes,
     * meshes or any other scene object is performed.
     */
    class Scene
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * This constructor initializes the scene and creates the root node
         * 
         * @param[in] name - Scene name
         * @param[in] drawingContext - Drawing context for the scene
         */
        Scene(const std::string& name, DrawingContextPtr drawingContext);

        /*!
         * @brief Class destructor
         */
        virtual ~Scene() = default;

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        /*!
         * @brief Active camera node setter
         * 
         * @param[in] cameraNode - Camera node to set as active
         */
        void setActiveCameraNode(CameraNodePtr cameraNode) { m_activeCameraNode = cameraNode; }

        /*!
         * @brief Name getter
         * 
         * @return Scene name
         */
        const std::string& name() const { return m_name; }

        /*!
         * @brief Drawing context getter
         * 
         * @return Drawing context
         */
        DrawingContextPtr drawingContext() const { return m_drawingContext; }

        /*!
         * @brief Root node getter
         * 
         * @return Scene root node
         */
        NodePtr rootNode() const { return m_rootNode; }

        /*!
         * @brief Active camera node getter
         * 
         * @return Active camera node
         */
        CameraNodePtr activeCameraNode() const { return m_activeCameraNode; }

        /*!
         * @brief Activates the scene and underlying drawing context
         */
        void activate();

        /*!
         * @brief Deactivates the scene and underlying drawing context
         */
        void deactivate();

        /*!
         * @brief Templated method to create a node to add to the scene
         * 
         * This method creates a new node of the specified type (must
         * be a valid Node subclass) and adds it to the scene graph.
         * The method throw a runtime error exception if the parent node
         * pointer is invalid or if there is already a node with the same name.
         * 
         * @param[in] name - New node name
         * @param[in] parent - New node parent
         * @return - New node pointer
         */
        template<class T>
        std::shared_ptr<T> createNode(const std::string& name, NodePtr parent)
        {
            /* Check parent pointer validity */
            if (nullptr == parent)
            {
                throw std::runtime_error("Invalid node parent");
            }

            /* Create node */
            std::shared_ptr<T> newNode = std::shared_ptr<T>(new T(name, parent));

            /* Add to parent */
            parent->addChild(newNode);

            return newNode;
        }

        /*!
         * @brief Method to get all light nodes in the scene
         * 
         * @return Vector with all light nodes in the scene
         */
        std::vector<LightNodePtr> getLightNodes() const;

    private:
        /*! Scene name */
        std::string m_name;

        /*! Drawing context */
        DrawingContextPtr m_drawingContext;

        /*! Root node */
        NodePtr m_rootNode;

        /*! Active camera node */
        CameraNodePtr m_activeCameraNode;

        /*!
         * @brief Helper method to parse a node for light nodes
         * 
         * This method checks if the input node is a light node, if so it adds it to the
         * vector passed as input, and then performs recursion on child nodes
         * 
         * @param[in]  node - Node to parse
         * @param[out] lightVec - Vector of light nodes to populate
         */
        void parseNodeForLight(NodePtr node, std::vector<LightNodePtr>& lightVec) const;
    };
}

}

#endif
