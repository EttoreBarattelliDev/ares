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

#include "ares/core/FlatTexMaterial.hpp"
#include "ares/glutils/ShaderManager.hpp"

#include <stdexcept>

namespace ares
{

namespace core
{

    /* Attribute names */
    constexpr char POS_ATTRIB_NAME[]   = "POSITION";
    constexpr char NORM_ATTRIB_NAME[]  = "NORMAL";
    constexpr char TANG_ATTRIB_NAME[]  = "TANGENT";
    constexpr char UV_ATTRIB_NAME[]    = "TEXCOORD_0";
    constexpr char COLOR_ATTRIB_NAME[] = "COLOR_0";

    /* Uniform names */
    constexpr char MVP_UNIF_NAME[]     = "u_mvp";
    constexpr char TEX_UNIF_NAME[]     = "u_tex";

    /* Vertex shader code */
    //TODO the vertex shader will likely be the same for all materials, move somewhere common
    constexpr char VERT_SHADER_SOURCE[] =
        "#version 100\n"
        "precision mediump float;\n"
        "attribute vec3 POSITION;\n"
        "attribute vec3 NORMAL;\n"
        "attribute vec4 TANGENT;\n"
        "attribute vec2 TEXCOORD_0;\n"
        "attribute vec3 COLOR_0;\n"
        "uniform mat4 u_mvp;\n"
        "varying vec2 v_uv;\n"
        "void main(void)\n"
        "{\n"
        "  v_uv = TEXCOORD_0;\n"
        "  gl_Position = u_mvp * vec4(POSITION, 1.0);\n"
        "}";

    /* Fragment shader code */
    constexpr char FRAG_SHADER_SOURCE[] =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec2 v_uv;\n"
        "uniform sampler2D u_tex;\n"
        "void main(void)\n"
        "{\n"
        "  gl_FragColor = texture2D(u_tex, v_uv);\n"
        "}";

    FlatTexMaterial::FlatTexMaterial(glutils::TexturePtr tex)
        : Material()
        , m_texture(tex)
    {
        if (nullptr == m_texture)
        {
            throw std::runtime_error("Invalid texture");
        }

        /* Get/compile shader */
        m_shader = glutils::ShaderManager::getShader(VERT_SHADER_SOURCE, FRAG_SHADER_SOURCE);
        if (nullptr != m_shader)
        {
            /* Add uniforms */
            m_shader->addUniform<glutils::UniformMat4>(MVP_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(TEX_UNIF_NAME);
        }
    }

    void FlatTexMaterial::onSetup(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec)
    {
        /* Get uniforms */
        glutils::UniformMat4Ptr mvpUnif   = m_shader->getUniform<glutils::UniformMat4>(MVP_UNIF_NAME);
        glutils::Uniform1iPtr   texUnif   = m_shader->getUniform<glutils::Uniform1i>(TEX_UNIF_NAME);

        /* Make sure uniforms are valid */
        if ((nullptr != mvpUnif) && (nullptr != texUnif) && (nullptr != m_texture))
        {
            /* Calculate mvp */
            glutils::Mat4 mvp(projectionMatrix);
            mvp *= mvMatrix;

            /* Set uniforms */
            mvpUnif->setAndCommit(mvp);
            texUnif->setAndCommit(0);

            /* Activate texture */
            m_texture->activate(0);
        }
    }

}

}
