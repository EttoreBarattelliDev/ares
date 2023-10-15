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

#ifndef NORMALMAPMATERIAL_HPP_INCLUDED
#define NORMALMAPMATERIAL_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/core/Material.hpp"
#include "ares/glutils/Texture.hpp"

namespace ares
{

namespace core
{
    class NormalMapMaterial;
    using NormalMapMaterialPtr = std::shared_ptr<NormalMapMaterial>;

    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Class implementing a normal map material.
     */
    class NormalMapMaterial : public Material
    {
    public:
        /*!
         * @brief Class constructor
         */
        NormalMapMaterial(glutils::TexturePtr diffuseTex, glutils::TexturePtr normalTex);

        /*!
         * @brief Class destructor
         */
        virtual ~NormalMapMaterial() = default;

        NormalMapMaterial(const NormalMapMaterial&) = delete;
        NormalMapMaterial& operator=(const NormalMapMaterial&) = delete;

        /*!
         * @brief Diffuse texture setter
         * 
         * @param[in] diffuseTex - Diffuse texture
         */
        void setDiffuseTex(glutils::TexturePtr diffuseTex) { m_diffuseTex = diffuseTex; }

        /*!
         * @brief Normal map texture setter
         * 
         * @param[in] normalTex - Normal map texture
         */
        void setNormalTex(glutils::TexturePtr normalTex) { m_normalTex = normalTex; }

        /*!
         * @brief Diffuse texture getter
         * 
         * @return Diffuse texture
         */
        glutils::TexturePtr diffuseTex() const { return m_diffuseTex; }

        /*!
         * @brief Normal map texture getter
         * 
         * @return Normal map texture
         */
        glutils::TexturePtr normalTex() const { return m_normalTex; }

    protected:
        /*! Diffuse texture */
        glutils::TexturePtr m_diffuseTex;

        /*! Normal map texture */
        glutils::TexturePtr m_normalTex;

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
