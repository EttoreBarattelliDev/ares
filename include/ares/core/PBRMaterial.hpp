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

#ifndef PBRMATERIAL_HPP_INCLUDED
#define PBRMATERIAL_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/core/Material.hpp"
#include "ares/glutils/Texture.hpp"

namespace ares
{

namespace core
{
    class PBRMaterial;
    using PBRMaterialPtr = std::shared_ptr<PBRMaterial>;

    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Class implementing a PBR material.
     */
    class PBRMaterial : public Material
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * @param[in] baseColorFactor - Base color factor
         * @param[in] emissiveFactor - Emissive factor
         * @param[in] metallicFactor - Metallic factor
         * @param[in] roughnessFactor - Roughness factor
         * @param[in] baseColorTex - Base color texture
         * @param[in] emissiveTex - Emissive texture
         * @param[in] normalTex - Normal texture
         * @param[in] occlusionTex - Occlusion texture
         * @param[in] metallicRoughnessTex - Metallic roughness texture
         */
        PBRMaterial(
            const glutils::Vec3& baseColorFactor,
            const glutils::Vec3& emissiveFactor,
            float metallicFactor,
            float roughnessFactor,
            const glutils::TexturePtr& baseColorTex,
            const glutils::TexturePtr& emissiveTex,
            const glutils::TexturePtr& normalTex,
            const glutils::TexturePtr& occlusionTex,
            const glutils::TexturePtr& metallicRoughnessTex
        );

        /*!
         * @brief Class destructor
         */
        virtual ~PBRMaterial() = default;

        PBRMaterial(const PBRMaterial&) = delete;
        PBRMaterial& operator=(const PBRMaterial&) = delete;

        /*!
         * @brief Base color factor getter
         *
         * @return Base color factor
         */
        const glutils::Vec3&       baseColorFactor()      const { return m_baseColorFactor;      }

        /*!
         * @brief Emissive factor getter
         *
         * @return Emissive factor
         */
        const glutils::Vec3&       emissiveFactor()       const { return m_emissiveFactor;       }

        /*!
         * @brief Metallic factor getter
         *
         * @return Metallic factor
         */
        float                      metallicFactor()       const { return m_metallicFactor;       }

        /*!
         * @brief Roughness factor getter
         *
         * @return Roughness factor
         */
        float                      roughnessFactor()      const { return m_roughnessFactor;      }

        /*!
         * @brief Base color texture getter
         *
         * @return Base color texture
         */
        const glutils::TexturePtr& baseColorTex()         const { return m_baseColorTex;         }

        /*!
         * @brief Emissive texture getter
         *
         * @return Emissive texture
         */
        const glutils::TexturePtr& emissiveTex()          const { return m_emissiveTex;          }

        /*!
         * @brief Normal texture getter
         *
         * @return Normal texture
         */
        const glutils::TexturePtr& normalTex()            const { return m_normalTex;            }

        /*!
         * @brief Occlusion texture getter
         *
         * @return Occlusion texture
         */
        const glutils::TexturePtr& occlusionTex()         const { return m_occlusionTex;         }

        /*!
         * @brief Metallic roughness texture getter
         *
         * @return Metallic roughness texture
         */
        const glutils::TexturePtr& metallicRoughnessTex() const { return m_metallicRoughnessTex; }

    protected:
        /*! Base color factor */
        glutils::Vec3       m_baseColorFactor;

        /*! Emissive factor */
        glutils::Vec3       m_emissiveFactor;

        /*! Metallic factor */
        float               m_metallicFactor;

        /*! Roughness factor */
        float               m_roughnessFactor;

        /*! Base color texture */
        glutils::TexturePtr m_baseColorTex;

        /*! Emissive texture */
        glutils::TexturePtr m_emissiveTex;

        /*! Normal texture */
        glutils::TexturePtr m_normalTex;

        /*! Occlusion texture */
        glutils::TexturePtr m_occlusionTex;

        /*! Metallic roughness texture */
        glutils::TexturePtr m_metallicRoughnessTex;

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
