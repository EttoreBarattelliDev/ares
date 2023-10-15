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

#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <GLES2/gl2.h>

#include "ares/glutils/Attribute.hpp"
#include "ares/glutils/AttributeData.hpp"
#include "ares/glutils/Uniform.hpp"

namespace ares
{

namespace glutils
{
    class Shader;
    using ShaderPtr = std::shared_ptr<Shader>;

    /*!
     * @brief Shader class implementing OpenGL shader functionality.
     * 
     * This class implements a shader interface to handle shader operations,
     * such as activation/deactivation, attributes and uniforms.
     * The uniforms are handled via the Uniform class. Uniform objects can be
     * retrieved via the Shader class.
     * This class does not provide shader compilation functionality. Shaders
     * should be compiled via the ShaderManager class, which manages the shader
     * programs created in the application and re-uses already created shader
     * programs to improve performance and resource usage.
     */
    class Shader
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a shader object with a specified OpenGL shader program ID.
         * The OpenGL shader program must have been already compiled.
         * 
         * @param[in] prog - OpenGL shader program ID
         */
        Shader(GLuint prog);

        /*!
         * @brief Class destructor
         */
        virtual ~Shader() = default;

        Shader() = delete;
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        /*!
         * @brief Activates the shader
         *
         * @param[in] attributeData - Data for the attributes
         */
        void activate(const std::vector<glutils::AttributeDataPtr>& attributeData);

        /*!
         * @brief Deactivates the shader
         *
         * @param[in] attributeData - Data for the attributes
         */
        void deactivate(const std::vector<glutils::AttributeDataPtr>& attributeData);

        /*!
         * @brief OpenGL shader program ID getter
         */
        GLuint program() const { return m_program; }

        /*!
         * @brief Gets the attribute location within this shader program
         * 
         * @param[in] attribName - Requested attribute name
         * @return Requested attribute location
         */
        GLint getAttribLocation(const std::string& attribName) const;

        /*!
         * @brief Gets the uniform location within this shader program
         * 
         * @param[in] unifName - Requested uniform name
         * @return Requested uniform location
         */
        GLint getUniformLocation(const std::string& unifName) const;

        /*!
         * @brief Gets an attribute object
         * 
         * This method gets the object pointer for an attribute that was
         * previously added with addAttribute. If the attribute was not added,
         * the method returns nullptr.
         * 
         * @param[in] attribName - Requested attribute name
         * @return Requested attribute object pointer, or nullptr if not found
         */
        AttributePtr getAttribute(const std::string& attribName) const;

        /*!
         * @brief Adds an attribute
         * 
         * This method adds a new attribute object to the shader object.
         * If the attribute was already added the existing attribute object is returned.
         * If the attribute was not already added, a new attribute object is created.
         * 
         * @param[in] name - Attribute name
         * @return Requested attribute object pointer
         */
        AttributePtr addAttribute(const std::string& name);

        /*!
         * @brief Adds attributes
         * 
         * This method adds new attribute objects to the shader object.
         * The overall stride and offset for each attribute are calculated based on
         * the other attributes in the provided vector
         * 
         * @param[in] names - Attribute names
         */
        void addAttributes(const std::vector<std::string>& names);

        /*!
         * @brief Gets a uniform object
         * 
         * This method gets the object pointer for a uniform that was
         * previously added with addUniform. If the uniform was not added,
         * the method returns nullptr.
         * 
         * @param[in] unifName - Requested uniform name
         * @return Requested uniform object pointer, or nullptr if not found
         */
        UniformPtr getUniform(const std::string& unifName) const;

        /*!
         * @brief Gets a uniform object of a specific type
         * 
         * This method is the templated version of getUniform to retrieve
         * a uniform object of a specific type. If the requested object is
         * not found or it is of a different type, the method returns nullptr
         * 
         * @param[in] unifName - Requested uniform name
         * @return Requested uniform object pointer, or nullptr if not found
         */
        template<class T>
        std::shared_ptr<T> getUniform(const std::string& unifName)
        {
            return std::dynamic_pointer_cast<T>(getUniform(unifName));
        }

        /*!
         * @brief Adds a uniform
         * 
         * This method adds a new uniform object to the shader object.
         * If the uniform was already added and its type matches the input type,
         * the existing uniform object is returned.
         * If the uniform was already added but its type does not match the input type,
         * an expection is thrown.
         * If the uniform was not already added, a new uniform object is created.
         * 
         * @param[in] unifName - Requested uniform name
         * @return Requested uniform object pointer
         */
        template<class T>
        std::shared_ptr<T> addUniform(const std::string& unifName)
        {
            std::shared_ptr<T> retval;
            UniformPtr unifPtr = getUniform(unifName);
            if (nullptr == unifPtr)
            {
                retval = std::make_shared<T>(unifName, getUniformLocation(unifName));
                m_uniformMap.emplace(unifName, retval);
            }
            else
            {
                retval = std::dynamic_pointer_cast<T>(unifPtr);
                if (nullptr == retval)
                {
                    throw std::runtime_error("Uniform already added with different type");
                }
            }
            return retval;
        }

    private:
        /*! OpenGL shader program ID */
        GLuint m_program;

        /*! Map of attribute objects added to the shader */
        std::unordered_map<std::string, AttributePtr> m_attributeMap;

        /*! Map of uniform objects added to the shader */
        std::unordered_map<std::string, UniformPtr> m_uniformMap;
    };
}

}

#endif
