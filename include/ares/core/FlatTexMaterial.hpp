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

#ifndef FLATTEXMATERIAL_HPP_INCLUDED
#define FLATTEXMATERIAL_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/core/Material.hpp"
#include "ares/glutils/Texture.hpp"

namespace ares
{

namespace core
{
    class FlatTexMaterial;
    using FlatTexMaterialPtr = std::shared_ptr<FlatTexMaterial>;

    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Class implementing a flat texture material.
     */
    class FlatTexMaterial : public Material
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * @param[in] tex - Texture
         */
        FlatTexMaterial(glutils::TexturePtr tex);

        /*!
         * @brief Class destructor
         */
        virtual ~FlatTexMaterial() = default;

        FlatTexMaterial(const FlatTexMaterial&) = delete;
        FlatTexMaterial& operator=(const FlatTexMaterial&) = delete;

        /*!
         * @brief Texture setter
         * 
         * @param[in] tex - Texture
         */
        void setTexture(glutils::TexturePtr tex) { m_texture = tex; }

        /*!
         * @brief Texture getter
         * 
         * @return Texture
         */
        glutils::TexturePtr texture() const { return m_texture; }

    protected:
        /*! Texture */
        glutils::TexturePtr m_texture;

        /*!
         * @brief Method to setup the material and prepare the shader for drawing
         * 
         * @param[in] mvMatrix - Model-View matrix for drawing
         * @param[in] projectionMatrix - Projection matrix for drawing
         * @param[in] normalMatrix - Normal matrix for drawing
         * @param[in] lightVec - Vector of lights for drawing
         */
        void onSetup(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec) override;
    };
}

}

#endif
