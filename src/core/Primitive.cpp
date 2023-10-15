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

#include "ares/core/Primitive.hpp"
#include "ares/glutils/GlUtils.hpp"

#include <stdexcept>

namespace ares
{

namespace core
{
    Primitive::Primitive(const std::vector<glutils::AttributeDataPtr>& attributeData, PrimitiveType primitiveType, GLsizei vertexCount, MaterialPtr material, glutils::AttributeDataPtr indicesData)
        : m_attributeData(attributeData)
        , m_primitiveType(primitiveType)
        , m_vertexCount(vertexCount)
        , m_material(material)
        , m_indicesData(indicesData)
    {
        /* Check material validity */
        if (nullptr == material)
        {
            throw std::runtime_error("Invalid material");
        }
    }

    void Primitive::draw(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec)
    {
        /* Check data validity */
        if (nullptr != m_material)
        {
            /* Setup material */
            m_material->setup(m_attributeData, mvMatrix, projectionMatrix, normalMatrix, lightVec);

            /* Check if this is an indexed primitive */
            if ((nullptr != m_indicesData) && (nullptr != m_indicesData->vbo()))
            {
                /* Activate Vbo for indices */
                m_indicesData->vbo()->activate();

                /* Draw */
                glDrawElements(static_cast<GLenum>(m_primitiveType), m_vertexCount, static_cast<GLenum>(m_indicesData->type()), (const void*)(intptr_t)m_indicesData->offset());
                glutils::GlUtils::checkGLError("glDrawElements");

                /* Deactivate indices */
                m_indicesData->vbo()->deactivate();
            }
            else
            {
                /* Draw */
                glDrawArrays(static_cast<GLenum>(m_primitiveType), 0, m_vertexCount);
                glutils::GlUtils::checkGLError("glDrawArrays");
            }

            /* Deactivate material */
            m_material->deactivate(m_attributeData);
        }
    }
}

}
