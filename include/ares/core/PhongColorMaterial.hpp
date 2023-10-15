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

#ifndef PHONGCOLORMATERIAL_HPP_INCLUDED
#define PHONGCOLORMATERIAL_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/core/Material.hpp"
#include "ares/glutils/RGBAColor.hpp"

namespace ares
{

namespace core
{
    class PhongColorMaterial;
    using PhongColorMaterialPtr = std::shared_ptr<PhongColorMaterial>;

    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Class implementing a phong color material.
     */
    class PhongColorMaterial : public Material
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * @param[in] ambientColor - Ambient color
         * @param[in] diffuseColor - Diffuse color
         * @param[in] specularColor - Specular color
         * @param[in] ambientCoeff - Ambient coefficient
         * @param[in] diffuseCoeff - Diffuse coefficient
         * @param[in] specularCoeff - Specular coefficient
         * @param[in] shininess - Shininess coefficient
         */
        PhongColorMaterial(const glutils::RGBAColor& ambientColor,
                           const glutils::RGBAColor& diffuseColor,
                           const glutils::RGBAColor& specularColor,
                           float ambientCoeff,
                           float diffuseCoeff,
                           float specularCoeff,
                           float shininess
                          );

        /*!
         * @brief Class destructor
         */
        virtual ~PhongColorMaterial() = default;

        PhongColorMaterial(const PhongColorMaterial&) = delete;
        PhongColorMaterial& operator=(const PhongColorMaterial&) = delete;

        /*!
         * @brief Ambient color setter
         * 
         * @param[in] color - Ambient color
         */
        void setAmbientColor(const glutils::RGBAColor& color) { m_ambientColor = color; }

        /*!
         * @brief Diffuse color setter
         * 
         * @param[in] color - Diffuse color
         */
        void setDiffuseColor(const glutils::RGBAColor& color) { m_diffuseColor = color; }

        /*!
         * @brief Specular color setter
         * 
         * @param[in] color - Specular color
         */
        void setSpecularColor(const glutils::RGBAColor& color) { m_specularColor = color; }

        /*!
         * @brief Ambient coefficient setter
         * 
         * @param[in] coeff - Ambient coefficient
         */
        void setAmbientCoeff(float coeff) { m_ambientCoeff = coeff; }

        /*!
         * @brief Diffuse coefficient setter
         * 
         * @param[in] coeff - Diffuse coefficient
         */
        void setDiffuseCoeff(float coeff) { m_diffuseCoeff = coeff; }

        /*!
         * @brief Specular coefficient setter
         * 
         * @param[in] coeff - Specular coefficient
         */
        void setSpecularCoeff(float coeff) { m_specularCoeff = coeff; }

        /*!
         * @brief Shininess setter
         * 
         * @param[in] shininess - Shininess
         */
        void setShininess(float shininess) { m_shininess = shininess; }

        /*!
         * @brief Ambient color getter
         * 
         * @return Ambient color
         */
        const glutils::RGBAColor& ambientColor() const { return m_ambientColor; }

        /*!
         * @brief Diffuse color getter
         * 
         * @return Diffuse color
         */
        const glutils::RGBAColor& diffuseColor() const { return m_diffuseColor; }

        /*!
         * @brief Specular color getter
         * 
         * @return Specular color
         */
        const glutils::RGBAColor& specularColor() const { return m_specularColor; }

        /*!
         * @brief Ambient coefficient getter
         * 
         * @return Ambient coefficient
         */
        float ambientCoeff() const { return m_ambientCoeff; }

        /*!
         * @brief Diffuse coefficient getter
         * 
         * @return Diffuse coefficient
         */
        float diffuseCoeff() const { return m_diffuseCoeff; }

        /*!
         * @brief Specular coefficient getter
         * 
         * @return Specular coefficient
         */
        float specularCoeff() const { return m_specularCoeff; }

        /*!
         * @brief Shininess getter
         * 
         * @return Shininess
         */
        float shininess() const { return m_shininess; }

    protected:
        /*! Material ambient color */
        glutils::RGBAColor m_ambientColor;

        /*! Material diffuse color */
        glutils::RGBAColor m_diffuseColor;

        /*! Material diffuse color */
        glutils::RGBAColor m_specularColor;

        /*! Material ambient coefficient */
        float m_ambientCoeff;

        /*! Material diffuse coefficient */
        float m_diffuseCoeff;

        /*! Material specular coefficient */
        float m_specularCoeff;

        /*! Material shininess */
        float m_shininess;

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
