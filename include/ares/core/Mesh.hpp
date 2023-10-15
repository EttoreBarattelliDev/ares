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

#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <GLES2/gl2.h>

#include "ares/core/Primitive.hpp"

namespace ares
{

namespace core
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;

    /*!
     * @brief Mesh class implementing a drawable surface.
     * 
     * This class implements a drawable mesh as a collection of primitives
     */
    class Mesh
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates an empty Mesh object with the provided name.
         * 
         * @param[in] name - Mesh name
         * @param[in] primitives - Vecor of primitives for the mesh
         */
        Mesh(const std::string& name, const std::vector<PrimitivePtr>& primitives = std::vector<PrimitivePtr>());

        /*!
         * @brief Class destructor
         */
        virtual ~Mesh() = default;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        /*!
         * @brief Method to add a primitive to the mesh
         * 
         * @param[in] primitive - Primitive to add
         */
        void addPrimitive(PrimitivePtr primitive) { m_primitives.push_back(primitive); }

        /*!
         * @brief Primitives getter
         * 
         * @return Primitives vector
         */
        const std::vector<PrimitivePtr>& primitives() const { return m_primitives; }

        /*!
         * @brief Method to draw the mesh
         *
         * @param[in] mvMatrix - Model-View matrix for the drawing
         * @param[in] projectionMatrix - Projection matrix for the drawing
         * @param[in] normalMatrix - Normal matrix for the drawing
         * @param[in] lightVec - Vector of light for the drawing
         */
        void draw(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec);

    protected:
        /*! Mesh name */
        std::string m_name;

        /*! Primitives vector */
        std::vector<PrimitivePtr> m_primitives;
    };
}

}

#endif
