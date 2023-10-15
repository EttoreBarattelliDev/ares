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

#ifndef LIGHTNODE_HPP_INCLUDED
#define LIGHTNODE_HPP_INCLUDED

#include <cstdint>
#include <memory>

#include "ares/core/Node.hpp"
#include "ares/core/Light.hpp"

namespace ares
{

namespace core
{
    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Node class specialization for nodes containing a light
     */
    class LightNode : public Node
    {
    public:
        /*!
         * @brief Class destructor
         */
        virtual ~LightNode() = default;

        LightNode(const LightNode&) = delete;
        LightNode& operator=(const LightNode&) = delete;

        /*!
         * @brief Light setter
         * 
         * @param[in] light - Light to set in the node
         */
        void setLight(LightPtr light) { m_light = light; }

        /*!
         * @brief Sets the position of the light node in view coordinates system
         * 
         * This method is called by the Renderer to set the light position in the view
         * coordinates system during scene rendering. The light position in the view
         * system can be used by other classes (eg. materials) to directly get the
         * light position without a need to re-compute for each mesh needing it.
         *
         * @param[in] lightPosition - Position of the light node in view coordinates system
         */
        void setLightPosition(const glutils::Vec3& lightPosition) { m_lightPosition = lightPosition; }

        /*!
         * @brief Light getter
         * 
         * @return Light object
         */
        LightPtr light() const { return m_light; }

        /*!
         * @brief Light position getter in the view coordinate system
         * 
         * This method returns the position of the node in the view coordinate system
         * previously set by the renderer during scene rendering.
         * 
         * @return Light position in the view coordinate system
         */
        const glutils::Vec3& lightPosition() const { return m_lightPosition; }

    private:
        /*! Light object */
        LightPtr m_light;

        /*! Light node position in the view coordinate system  */
        glutils::Vec3 m_lightPosition;

        /*!
         * @brief Class constructor
         */
        LightNode(const std::string& name, NodePtr parent);

        friend class Scene;
    };
}

}

#endif
