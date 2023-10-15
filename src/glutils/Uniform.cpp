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

#include "ares/glutils/Uniform.hpp"
#include "ares/glutils/GlUtils.hpp"

#include <stdexcept>

namespace ares
{

namespace glutils
{

    /***************** Uniform *****************/
    Uniform::Uniform(const std::string& name, int32_t loc)
        : m_name(name)
        , m_location(loc)
    {
        /* Check for valid location */
        if (loc < 0)
        {
            throw std::runtime_error("Invalid uniform " + name);
        }
    }

    /***************** Uniform1f *****************/
    Uniform1f::Uniform1f(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value(0.F)
    {
    }

    void Uniform1f::setValue(float v0)
    {
        m_value = v0;
    }

    void Uniform1f::commit()
    {
        glUniform1f(m_location, m_value);
        GlUtils::checkGLError("glUniform1f");
    }

    void Uniform1f::setAndCommit(float v0)
    {
        setValue(v0);
        commit();
    }

    /***************** Uniform2f *****************/
    Uniform2f::Uniform2f(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void Uniform2f::setValue(const Vec2& v0)
    {
        m_value = v0;
    }

    void Uniform2f::commit()
    {
        glUniform2f(m_location, m_value[0], m_value[1]);
        GlUtils::checkGLError("glUniform2f");
    }

    void Uniform2f::setAndCommit(const Vec2& v0)
    {
        setValue(v0);
        commit();
    }

    /***************** Uniform3f *****************/
    Uniform3f::Uniform3f(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void Uniform3f::setValue(const Vec3& v0)
    {
        m_value = v0;
    }

    void Uniform3f::commit()
    {
        glUniform3f(m_location, m_value[0], m_value[1], m_value[2]);
        GlUtils::checkGLError("glUniform3f");
    }

    void Uniform3f::setAndCommit(const Vec3& v0)
    {
        setValue(v0);
        commit();
    }

    /***************** Uniform4f *****************/
    Uniform4f::Uniform4f(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void Uniform4f::setValue(const Vec4& v0)
    {
        m_value = v0;
    }

    void Uniform4f::commit()
    {
        glUniform4f(m_location, m_value[0], m_value[1], m_value[2], m_value[3]);
        GlUtils::checkGLError("glUniform4f");
    }

    void Uniform4f::setAndCommit(const Vec4& v0)
    {
        setValue(v0);
        commit();
    }

    /***************** Uniformfv *****************/
    Uniformfv::Uniformfv(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void Uniformfv::setValue(const std::vector<float>& v0)
    {
        m_value = v0;
    }

    void Uniformfv::commit()
    {
        glUniform1fv(m_location, m_value.size(), m_value.data());
        GlUtils::checkGLError("glUniform1fv");
    }

    void Uniformfv::setAndCommit(const std::vector<float>& v0)
    {
        setValue(v0);
        commit();
    }

    /***************** Uniform1i *****************/
    Uniform1i::Uniform1i(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value(0)
    {
    }

    void Uniform1i::setValue(int32_t v0)
    {
        m_value = v0;
    }

    void Uniform1i::commit()
    {
        glUniform1i(m_location, m_value);
        GlUtils::checkGLError("glUniform1i");
    }

    void Uniform1i::setAndCommit(int32_t v0)
    {
        setValue(v0);
        commit();
    }

    /***************** UniformMat2 *****************/
    UniformMat2::UniformMat2(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void UniformMat2::setValue(const Mat2& v0)
    {
        m_value = v0;
    }

    void UniformMat2::commit()
    {
        glUniformMatrix2fv(m_location, 1, GL_FALSE, m_value.const_data());
        GlUtils::checkGLError("glUniformMatrix2fv");
    }

    void UniformMat2::setAndCommit(const Mat2& v0)
    {
        setValue(v0);
        commit();
    }

    /***************** UniformMat3 *****************/
    UniformMat3::UniformMat3(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void UniformMat3::setValue(const Mat3& v0)
    {
        m_value = v0;
    }

    void UniformMat3::commit()
    {
        glUniformMatrix3fv(m_location, 1, GL_FALSE, m_value.const_data());
        GlUtils::checkGLError("glUniformMatrix3fv");
    }

    void UniformMat3::setAndCommit(const Mat3& v0)
    {
        setValue(v0);
        commit();
    }

    /***************** UniformMat4 *****************/
    UniformMat4::UniformMat4(const std::string& name, int32_t loc)
        : Uniform(name, loc)
        , m_value()
    {
    }

    void UniformMat4::setValue(const Mat4& v0)
    {
        m_value = v0;
    }

    void UniformMat4::commit()
    {
        glUniformMatrix4fv(m_location, 1, GL_FALSE, m_value.const_data());
        GlUtils::checkGLError("glUniformMatrix4fv");
    }

    void UniformMat4::setAndCommit(const Mat4& v0)
    {
        setValue(v0);
        commit();
    }
}

}
