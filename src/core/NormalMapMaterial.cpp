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

#include "ares/core/NormalMapMaterial.hpp"
#include "ares/core/LightNode.hpp"
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
    constexpr char MVMX_UNIF_NAME[]       = "u_mvMx";
    constexpr char PMX_UNIF_NAME[]        = "u_pMx";
    constexpr char NORMMX_UNIF_NAME[]     = "u_normMx";
    constexpr char DIFFUSETEX_UNIF_NAME[] = "u_diffuseTex";
    constexpr char NORMALTEX_UNIF_NAME[]  = "u_normalTex";
    constexpr char LIGHTPOS_UNIF_NAME[]   = "u_lightPos";

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
        "uniform mat4 u_mvMx;\n"
        "uniform mat4 u_pMx;\n"
        "uniform mat4 u_normMx;\n"
        "varying vec3 v_pos;\n"
        "varying vec3 v_norm;\n"
        "varying vec3 v_tang;\n"
        "varying vec3 v_bita;\n"
        "varying vec2 v_uv;\n"
        "void main(void)\n"
        "{\n"
        "  v_pos = vec3(u_mvMx * vec4(POSITION, 1.0));\n"
        "  v_norm = normalize(mat3(u_normMx) * NORMAL);\n"
        "  v_tang = normalize(mat3(u_normMx) * vec3(TANGENT));\n"
        "  v_bita = normalize(mat3(u_normMx) * cross(NORMAL, vec3(TANGENT)));\n"
        "  v_uv = TEXCOORD_0;\n"
        "  gl_Position = u_pMx * vec4(v_pos, 1.0);\n"
        "}";

    /* Fragment shader code */
    constexpr char FRAG_SHADER_SOURCE[] =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec3 v_pos;\n"
        "varying vec3 v_norm;\n"
        "varying vec3 v_tang;\n"
        "varying vec3 v_bita;\n"
        "varying vec2 v_uv;\n"
        "uniform vec3 u_lightPos;\n"
        "uniform sampler2D u_diffuseTex;\n"
        "uniform sampler2D u_normalTex;\n"
        "\n"
        "void main() {\n"
        "  // Construct Tangent Space Basis\n"
        "  mat3 TBN = mat3(v_tang, v_bita, v_norm);\n"
        "  vec3 N = normalize(texture2D(u_normalTex, v_uv).rgb * 2.0 - 1.0);\n"
        "  N = normalize(TBN * N);\n"
        "  vec3 L = normalize(u_lightPos - v_pos);\n"
        "  vec3 R = reflect(-L, N);\n"
        "  vec3 V = normalize(-v_pos);\n"
        "  float diff = max(dot(N, L), 0.0);\n"
        "  // Compute the specular term\n"
        "  float spec = max(dot(V, R), 0.0);\n"
        "  vec4 diffuseColor = texture2D(u_diffuseTex, v_uv);\n"
        "  gl_FragColor = vec4(diff * diffuseColor.rgb +"
        "                      0.3 * vec3(spec),"  //TODO add configuration for specular factor and color?
        "                      diffuseColor.a);\n"
        "}";

    NormalMapMaterial::NormalMapMaterial(glutils::TexturePtr diffuseTex, glutils::TexturePtr normalTex)
        : Material()
        , m_diffuseTex(diffuseTex)
        , m_normalTex(normalTex)
    {
        if ((nullptr == m_diffuseTex) || (nullptr == m_normalTex))
        {
            throw std::runtime_error("Invalid texture");
        }

        /* Get/compile shader */
        m_shader = glutils::ShaderManager::getShader(VERT_SHADER_SOURCE, FRAG_SHADER_SOURCE);
        if (nullptr != m_shader)
        {
            /* Add uniforms */
            m_shader->addUniform<glutils::UniformMat4>(MVMX_UNIF_NAME);
            m_shader->addUniform<glutils::UniformMat4>(PMX_UNIF_NAME);
            m_shader->addUniform<glutils::UniformMat4>(NORMMX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(DIFFUSETEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(NORMALTEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(LIGHTPOS_UNIF_NAME);
        }
    }

    void NormalMapMaterial::onSetup(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec)
    {
        if ((nullptr == m_diffuseTex) || (nullptr == m_normalTex))
        {
            throw std::runtime_error("Invalid texture");
        }

        /* Get uniforms */
        glutils::UniformMat4Ptr mvmxUnif          = m_shader->getUniform<glutils::UniformMat4>(MVMX_UNIF_NAME);
        glutils::UniformMat4Ptr pmxUnif           = m_shader->getUniform<glutils::UniformMat4>(PMX_UNIF_NAME);
        glutils::UniformMat4Ptr normmxUnif        = m_shader->getUniform<glutils::UniformMat4>(NORMMX_UNIF_NAME);
        glutils::Uniform1iPtr   diffuseTexUnif    = m_shader->getUniform<glutils::Uniform1i>(DIFFUSETEX_UNIF_NAME);
        glutils::Uniform1iPtr   normalTexUnif     = m_shader->getUniform<glutils::Uniform1i>(NORMALTEX_UNIF_NAME);
        glutils::Uniform3fPtr   lightPosUnif      = m_shader->getUniform<glutils::Uniform3f>(LIGHTPOS_UNIF_NAME);

        /* Make sure uniforms are valid */
        if (
            (nullptr != mvmxUnif)          &&
            (nullptr != pmxUnif)           &&
            (nullptr != normmxUnif)        &&
            (nullptr != diffuseTexUnif)    &&
            (nullptr != normalTexUnif)     &&
            (nullptr != lightPosUnif)      &&
            (nullptr != m_diffuseTex)      &&
            (nullptr != m_normalTex)
           )
        {
            /* Set uniforms */
            mvmxUnif->setAndCommit(mvMatrix);
            pmxUnif->setAndCommit(projectionMatrix);
            normmxUnif->setAndCommit(normalMatrix);
            diffuseTexUnif->setAndCommit(0);
            normalTexUnif->setAndCommit(1);

            /* Activate texture */
            m_diffuseTex->activate(0);
            m_normalTex->activate(1);

            /* Set lights */
            if (!lightVec.empty())
            {
                //TODO Add support for multiple lights
                LightNodePtr lightNode = lightVec[0];
                glutils::Vec3 lightPos = lightNode->lightPosition();
                lightPosUnif->setAndCommit(lightPos);
            }
        }
    }

}

}
