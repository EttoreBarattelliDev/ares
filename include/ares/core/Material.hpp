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

#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <GLES2/gl2.h>

#include "ares/glutils/AttributeData.hpp"
#include "ares/glutils/Shader.hpp"
#include "ares/glutils/Vbo.hpp"

namespace ares
{

namespace core
{
    class Material;
    using MaterialPtr = std::shared_ptr<Material>;

    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Abstract class to handle materials.
     * 
     * This class defines an interface for materials.
     * The Material class provides methods to setup and deactivate the material.
     * This class must be derived by specializations that implement the actual
     * materials. The derived classes must assign a valid shader pointer in the
     * constructor, as well as implementing the onSetup method to setup the
     * material, activate the provided vbo and configure the shader.
     * The onSetup method is called in the context of the Material::setup method
     */
    class Material
    {
    public:
        /*!
         * @brief Class constructor
         */
        Material();

        /*!
         * @brief Class destructor
         */
        virtual ~Material() = default;

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        /*!
         * @brief Shader getter method
         * 
         * @return Shader object
         */
        glutils::ShaderPtr shader() const { return m_shader; }

        /*!
         * @brief Method to setup the material
         * 
         * This method can be called by the material owner to configure the
         * material for drawing. This method activates the shader and calls
         * the virtual onSetup interface, that must be implemented by derived
         * classes to perform any shader configuration.
         * 
         * @param[in] attributeData - Vector of attribute data for the shader
         * @param[in] mvMatrix - Model-View matrix for the drawing
         * @param[in] projectionMatrix - Projection matrix for the drawing
         * @param[in] normalMatrix - Normal matrix for the drawing
         * @param[in] lightVec - Vector of light for the drawing
         */
        void setup(const std::vector<glutils::AttributeDataPtr>& attributeData,
                   const glutils::Mat4& mvMatrix,
                   const glutils::Mat4& projectionMatrix,
                   const glutils::Mat4& normalMatrix,
                   const std::vector<LightNodePtr>& lightVec);

        /*!
         * @brief Method to deactivate the material
         * 
         * This method deactivates the shader attached to the material.
         * 
         * @param[in] attributeData - Vector of attribute data for the shader 
         */
        void deactivate(const std::vector<glutils::AttributeDataPtr>& attributeData);

    protected:
        /*!
         * @brief Virtual interface to setup the material
         * 
         * This method must be implemented by derived classes to perform any shader
         * configuration.
         * 
         * @param[in] mvMatrix - Model-View matrix for the drawing
         * @param[in] projectionMatrix - Projection matrix for the drawing
         * @param[in] normalMatrix - Normal matrix for the drawing
         * @param[in] lightVec - Vector of light for the drawing
         */
        virtual void onSetup(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec) = 0;

        /*! Shader object */
        glutils::ShaderPtr m_shader;
    };
}

}

#endif
