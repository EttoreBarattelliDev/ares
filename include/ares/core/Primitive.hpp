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

#ifndef PRIMITIVE_HPP_INCLUDED
#define PRIMITIVE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/core/Material.hpp"
#include "ares/glutils/AttributeData.hpp"
#include "ares/glutils/LinearAlgebra.hpp"

namespace ares
{

namespace core
{
    class Primitive;
    using PrimitivePtr = std::shared_ptr<Primitive>;

    class LightNode;
    using LightNodePtr = std::shared_ptr<LightNode>;

    /*!
     * @brief Primitive class implementing a drawable primitive.
     * 
     * This class implements a drawable primitive. The geometry is provided
     * as a vector of AttributeData which contains the Vbo and configuration
     * data for the primitive attributes. If attribute data for the indices
     * is provided, the primitive is considered as indexed
     */
    class Primitive
    {
    public:
        /*!
         * @brief Primitive type enumeration
         */
        enum class PrimitiveType
        {
            Triangles     = GL_TRIANGLES,       /*!< Triangles primitive      */
            TriangleStrip = GL_TRIANGLE_STRIP,  /*!< Triangle strip primitive */
            TriangleFan   = GL_TRIANGLE_FAN     /*!< Triangle fan primitive   */
        };

        /*!
         * @brief Class constructor
         * 
         * Creates a Primitive object with the provided Vbo. When the primitive is drawn, the Vbo
         * is drawn using the provided primitive type and vertex count.
         * 
         * @param[in] attributeData - Vector of attributes for the primitive
         * @param[in] primitiveType - Primitive type for the drawing
         * @param[in] vertexCount - Number of vertices
         * @param[in] material - Primitive material
         * @param[in] indicesData - Optional indices data, the primitive is considered indexed if provided
         */
        Primitive(const std::vector<glutils::AttributeDataPtr>& attributeData, PrimitiveType primitiveType, GLsizei vertexCount, MaterialPtr material, glutils::AttributeDataPtr indicesData = nullptr);

        /*!
         * @brief Class destructor
         */
        virtual ~Primitive() = default;

        Primitive(const Primitive&) = delete;
        Primitive& operator=(const Primitive&) = delete;

        /*!
         * @brief Attribute data getter
         * 
         * @return Attribute data
         */
        const std::vector<glutils::AttributeDataPtr>& attributeData() const { return m_attributeData; }

        /*!
         * @brief Primitive type getter
         * 
         * @return Primitive type
         */
        PrimitiveType   primitiveType() const { return m_primitiveType; }

        /*!
         * @brief Vertex count getter
         * 
         * @return Vertex count
         */
        GLsizei         vertexCount()   const { return m_vertexCount;   }

        /*!
         * @brief Material getter
         * 
         * @return Material object
         */
        MaterialPtr     material()      const { return m_material;      }

        /*!
         * @brief Attribute data getter
         * 
         * @return Attribute data
         */
        const glutils::AttributeDataPtr& indicesData() const { return m_indicesData; }

        /*!
         * @brief Method to draw the primitive
         *
         * @param[in] mvMatrix - Model-View matrix for the drawing
         * @param[in] projectionMatrix - Projection matrix for the drawing
         * @param[in] normalMatrix - Normal matrix for the drawing
         * @param[in] lightVec - Vector of light for the drawing
         */
        void draw(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec);

    protected:
        /*! Attribute data */
        std::vector<glutils::AttributeDataPtr> m_attributeData;

        /*! Primitive type */
        PrimitiveType m_primitiveType;

        /*! Vertex count */
        GLsizei m_vertexCount;

        /*! Material object */
        MaterialPtr m_material;

        /*! Attribute data for indices */
        glutils::AttributeDataPtr m_indicesData;
    };
}

}

#endif
