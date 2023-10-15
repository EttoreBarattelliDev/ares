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

#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/core/Scene.hpp"
#include "ares/glutils/RGBAColor.hpp"

namespace ares
{

namespace core
{
    class Renderer;
    using RendererPtr = std::shared_ptr<Renderer>;

    /*!
     * @brief Renderer class to render a scene
     * 
     * This class implements a renderer that can be used
     * to render a scene. The renderer retrieves all relevant
     * information to render the scene (active camera, mvp matrix,
     * lights, etc.) and walks the scene graph to render all meshes.
     */
    class Renderer
    {
    public:
        /*!
         * @brief Class constructor
         * 
         */
        Renderer();

        /*!
         * @brief Class destructor
         */
        virtual ~Renderer() = default;

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        /*!
         * @brief Background color setter
         * 
         * @param[in] bgColor - Background color
         */
        void setBgColor(const glutils::RGBAColor& bgColor) { m_bgColor = bgColor; }

        /*!
         * @brief Renders the scene
         * 
         * This method is the main entry point for the
         * scene rendering. It collects all relevant information,
         * activates the scene and renders all meshes found in the scene
         * 
         * @param[in] scene - Scene to render
         */
        void render(ScenePtr scene);

    private:
        /*! View matrix from the active camera */
        glutils::Mat4 m_viewMatrix;

        /*! Projection matrix from the active camera */
        glutils::Mat4 m_projectionMatrix;

        /*! Background/clear color for the framebuffer */
        glutils::RGBAColor m_bgColor;

        /*!
         * @brief Recursive method to render a node
         * 
         * This method calculates the node transform from the parent
         * transform and local node transform, calculates the mvp
         * matrix if the node has a mesh and draws such mesh.
         * 
         * @param[in] node - Current node
         * @param[in] parentXform - Cumulative parent node transform
         * @param[in] lightVec - Vector of lights for the drawing
         */
        void renderNode(NodePtr node, const glutils::Mat4& parentXform, const std::vector<LightNodePtr>& lightVec);
    };
}

}

#endif
