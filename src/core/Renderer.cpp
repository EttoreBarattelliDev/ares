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

#include "ares/core/Renderer.hpp"
#include "ares/core/MeshNode.hpp"
#include "ares/glutils/GlUtils.hpp"

#include <stdexcept>

namespace ares
{

namespace core
{
    Renderer::Renderer()
        : m_viewMatrix()
        , m_projectionMatrix()
        , m_bgColor()
    {
    }

    void Renderer::render(ScenePtr scene)
    {
        /* Check for valid scene */
        if (nullptr == scene)
        {
            throw std::runtime_error("Invalid scene");
        }

        /* Check for valid drawing context */
        DrawingContextPtr drawingContext = scene->drawingContext();
        if (nullptr == drawingContext)
        {
            throw std::runtime_error("Invalid drawing context");
        }

        /* Do nothing if device is not open */
        if (!drawingContext->isDeviceOpen())
        {
            return;
        }

        /* Activate the scene */
        scene->activate();

        /* Check for valid active camera */
        CameraNodePtr cameraNode = scene->activeCameraNode();
        if (nullptr == cameraNode)
        {
            throw std::runtime_error("Invalid camera node");
        }

        /* Get view matrix as inverse of camera node transform */
        m_viewMatrix = cameraNode->totalTransformMatrix();
        m_viewMatrix.invert();

        /* Check for valid camera */
        CameraPtr camera = cameraNode->camera();
        if (nullptr == camera)
        {
            throw std::runtime_error("Invalid camera");
        }

        /* Get projection matrix from camera */
        m_projectionMatrix = camera->projectionMatrix();

        /* Get light vector from scene and set their position in the view */
        std::vector<LightNodePtr> lightVec = scene->getLightNodes();
        for (auto& lightNode : lightVec)
        {
            /* Apply view matrix to total light node transform (i.e. model matrix) */
            glutils::Mat4 lightMVMx(m_viewMatrix);
            lightMVMx *= lightNode->totalTransformMatrix();

            /* Transform light node local origin with model-view matrix */
            glutils::Vec4 lightPos(0.F, 0.F, 0.F, 1.F);
            lightPos = lightMVMx * lightPos;
            lightPos /= lightPos[3];

            /* Set light position in the view coordinate system */
            lightNode->setLightPosition(glutils::Vec3(lightPos[0], lightPos[1], lightPos[2]));
        }

        /* Enable back-face culling */
        glEnable(GL_CULL_FACE);
        glutils::GlUtils::checkGLError("glEnable");
        glCullFace(GL_BACK);
        glutils::GlUtils::checkGLError("glCullFace");
        glFrontFace(GL_CCW);
        glutils::GlUtils::checkGLError("glFrontFace");

        /* Enable depth test */
        glEnable(GL_DEPTH_TEST);
        glutils::GlUtils::checkGLError("glEnable");
        glDepthFunc(GL_LEQUAL);
        glutils::GlUtils::checkGLError("glDepthFunc");

        /* Clear color and depth buffers */
        glClearColor(m_bgColor.red(), m_bgColor.green(), m_bgColor.blue(), m_bgColor.alpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutils::GlUtils::checkGLError("glClear");

        /* Render nodes starting from root */
        glutils::Mat4 ident;
        ident.setIdentity();
        renderNode(scene->rootNode(), ident, lightVec);

        /* Finalize the draw */
        drawingContext->draw();
    }

    void Renderer::renderNode(NodePtr node, const glutils::Mat4& parentXform, const std::vector<LightNodePtr>& lightVec)
    {
        /* Check for valid node */
        if (nullptr != node)
        {
            /* Get world transform */
            glutils::Mat4 modelMatrix = parentXform * node->transformMatrix();

            /* Check for mesh type */
            if (Node::Type::Mesh == node->type())
            {
                /* Get mesh */
                MeshNodePtr meshNode = std::dynamic_pointer_cast<MeshNode>(node);
                MeshPtr mesh = meshNode->mesh();
                if (nullptr != mesh)
                {
                    /* Calculate model-view matrix */
                    glutils::Mat4 mvMatrix(m_viewMatrix);
                    mvMatrix *= modelMatrix;

                    /* Calculate normal matrix */
                    glutils::Mat4 normalMatrix(modelMatrix);
                    normalMatrix.invert();
                    normalMatrix.transpose();

                    /* Draw mesh */
                    mesh->draw(mvMatrix, m_projectionMatrix, normalMatrix, lightVec);
                }
            }

            /* Recursion on children */
            for (auto& childNode : node->children())
            {
                renderNode(childNode, modelMatrix, lightVec);
            }
        }
        else
        {
            throw std::runtime_error("Found null node during rendering");
        }
    }
}

}
