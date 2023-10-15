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

#ifndef UNIFORM_HPP_INCLUDED
#define UNIFORM_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include "ares/glutils/LinearAlgebra.hpp"

namespace ares
{

namespace glutils
{
    class Uniform;
    using UniformPtr = std::shared_ptr<Uniform>;

    /*!
     * @brief Abstract class representing an OpenGL Uniform
     * 
     * This abstract class represents an interface for shader
     * uniforms. It contains the uniform name and location in the shader
     * and provides a method to set the uniform in the shader.
     * This class must be derived for the actual uniform types.
     */
    class Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniform(const std::string& name, int32_t loc);

        Uniform() = delete;
        Uniform(const Uniform&) = delete;
        Uniform& operator=(const Uniform&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniform() = default;

        /*!
         * @brief Method to set value in the shader
         * 
         * Pure virtual method to set the uniform value in the shader.
         * The shader MUST be activated first
         */
        virtual void commit() = 0;

        /*!
         * @brief Uniform name getter
         * 
         * @return Uniform name
         */
        const std::string& name() const { return m_name; }

        /*!
         * @brief Uniform location getter
         * 
         * @return Uniform location
         */
        int32_t location() const { return m_location; }

    protected:
        /*! Uniform name */
        std::string m_name;

        /*! Uniform location in the shader */
        int32_t m_location;
    };

    class Uniform1f;
    using Uniform1fPtr = std::shared_ptr<Uniform1f>;

    /*!
     * @brief Float Uniform class
     */
    class Uniform1f : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniform1f(const std::string& name, int32_t loc);

        Uniform1f() = delete;
        Uniform1f(const Uniform1f&) = delete;
        Uniform1f& operator=(const Uniform1f&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniform1f() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(float v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(float v0);

    private:
        /*! Uniform value */
        float m_value;
    };

    class Uniform2f;
    using Uniform2fPtr = std::shared_ptr<Uniform2f>;

    /*!
     * @brief Float 2 Uniform class
     */
    class Uniform2f : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniform2f(const std::string& name, int32_t loc);

        Uniform2f() = delete;
        Uniform2f(const Uniform2f&) = delete;
        Uniform2f& operator=(const Uniform2f&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniform2f() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const Vec2& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const Vec2& v0);

    private:
        /*! Uniform value */
        Vec2 m_value;
    };

    class Uniform3f;
    using Uniform3fPtr = std::shared_ptr<Uniform3f>;

    /*!
     * @brief Float 3 Uniform class
     */
    class Uniform3f : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniform3f(const std::string& name, int32_t loc);

        Uniform3f() = delete;
        Uniform3f(const Uniform3f&) = delete;
        Uniform3f& operator=(const Uniform3f&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniform3f() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const Vec3& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const Vec3& v0);

    private:
        /*! Uniform value */
        Vec3 m_value;
    };

    class Uniform4f;
    using Uniform4fPtr = std::shared_ptr<Uniform4f>;

    /*!
     * @brief Float 4 Uniform class
     */
    class Uniform4f : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniform4f(const std::string& name, int32_t loc);

        Uniform4f() = delete;
        Uniform4f(const Uniform4f&) = delete;
        Uniform4f& operator=(const Uniform4f&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniform4f() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const Vec4& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const Vec4& v0);

    private:
        /*! Uniform value */
        Vec4 m_value;
    };

    class Uniformfv;
    using UniformfvPtr = std::shared_ptr<Uniformfv>;

    /*!
     * @brief Float vector Uniform class
     */
    class Uniformfv : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniformfv(const std::string& name, int32_t loc);

        Uniformfv() = delete;
        Uniformfv(const Uniformfv&) = delete;
        Uniformfv& operator=(const Uniformfv&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniformfv() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const std::vector<float>& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const std::vector<float>& v0);

    private:
        /*! Uniform value */
        std::vector<float> m_value;
    };

    class Uniform1i;
    using Uniform1iPtr = std::shared_ptr<Uniform1i>;

    /*!
     * @brief Int 1 Uniform class
     */
    class Uniform1i : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        Uniform1i(const std::string& name, int32_t loc);

        Uniform1i() = delete;
        Uniform1i(const Uniform1i&) = delete;
        Uniform1i& operator=(const Uniform1i&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~Uniform1i() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(int32_t v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(int32_t v0);

    private:
        /*! Uniform value */
        int32_t m_value;
    };

    class UniformMat2;
    using UniformMat2Ptr = std::shared_ptr<UniformMat2>;

    /*!
     * @brief Matrix 2x2 Uniform class
     */
    class UniformMat2 : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        UniformMat2(const std::string& name, int32_t loc);

        UniformMat2() = delete;
        UniformMat2(const UniformMat2&) = delete;
        UniformMat2& operator=(const UniformMat2&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~UniformMat2() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const Mat2& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const Mat2& v0);

    private:
        /*! Uniform value */
        Mat2 m_value;
    };

    class UniformMat3;
    using UniformMat3Ptr = std::shared_ptr<UniformMat3>;

    /*!
     * @brief Matrix 3x3 Uniform class
     */
    class UniformMat3 : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        UniformMat3(const std::string& name, int32_t loc);

        UniformMat3() = delete;
        UniformMat3(const UniformMat3&) = delete;
        UniformMat3& operator=(const UniformMat3&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~UniformMat3() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const Mat3& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const Mat3& v0);

    private:
        /*! Uniform value */
        Mat3 m_value;
    };

    class UniformMat4;
    using UniformMat4Ptr = std::shared_ptr<UniformMat4>;

    /*!
     * @brief Matrix 4x4 Uniform class
     */
    class UniformMat4 : public Uniform
    {
    public:
        /*!
         * @brief Class constructor
         * 
         * Creates a uniform object with a given name and location within the shader
         * 
         * @param[in] name - Uniform name
         * @param[in] loc  - Uniform location
         */
        UniformMat4(const std::string& name, int32_t loc);

        UniformMat4() = delete;
        UniformMat4(const UniformMat4&) = delete;
        UniformMat4& operator=(const UniformMat4&) = delete;

        /*!
         * @brief Class destructor
         */
        virtual ~UniformMat4() = default;

        /*!
         * @brief Value setter
         * 
         * @param[in] v0 - Uniform value
         */
        void setValue(const Mat4& v0);

        /*!
         * @brief Method to set value in the shader
         */
        void commit() override;

        /*!
         * @brief Method to set and commit value
         * 
         * @param[in] v0 - Uniform value
         */
        void setAndCommit(const Mat4& v0);

    private:
        /*! Uniform value */
        Mat4 m_value;
    };
}

}

#endif
