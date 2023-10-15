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

#include "ares/core/PhongColorMaterial.hpp"
#include "ares/core/LightNode.hpp"
#include "ares/glutils/ShaderManager.hpp"

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
    constexpr char MVMX_UNIF_NAME[]          = "u_mvMx";
    constexpr char PMX_UNIF_NAME[]           = "u_pMx";
    constexpr char NORMMX_UNIF_NAME[]        = "u_normMx";
    constexpr char KA_UNIF_NAME[]            = "u_ka";
    constexpr char KD_UNIF_NAME[]            = "u_kd";
    constexpr char KS_UNIF_NAME[]            = "u_ks";
    constexpr char SHININESS_UNIF_NAME[]     = "u_shininess";
    constexpr char AMBIENTCOLOR_UNIF_NAME[]  = "u_ambientColor";
    constexpr char DIFFUSECOLOR_UNIF_NAME[]  = "u_diffuseColor";
    constexpr char SPECULARCOLOR_UNIF_NAME[] = "u_specularColor";
    constexpr char LIGHTPOS_UNIF_NAME[]      = "u_lightPos";

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
        "varying vec3 v_norm;\n"
        "varying vec3 v_pos;\n"
        "void main(void)\n"
        "{\n"
        "  vec4 vertPos4 = u_mvMx * vec4(POSITION, 1.0);\n"
        "  v_pos = vec3(vertPos4) / vertPos4.w;\n"
        "  v_norm = vec3(u_normMx * vec4(NORMAL, 0.0));\n"
        "  gl_Position = u_pMx * vertPos4;\n"
        "}";

    /* Fragment shader code */
    constexpr char FRAG_SHADER_SOURCE[] =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec3 v_norm;\n"
        "varying vec3 v_pos;\n"
        "uniform float u_ka;\n"
        "uniform float u_kd;\n"
        "uniform float u_ks;\n"
        "uniform float u_shininess;\n"
        "// Material color\n"
        "uniform vec3 u_ambientColor;\n"
        "uniform vec3 u_diffuseColor;\n"
        "uniform vec3 u_specularColor;\n"
        "uniform vec3 u_lightPos;\n"
        "\n"
        "void main() {\n"
        "  vec3 N = normalize(v_norm);\n"
        "  vec3 L = normalize(u_lightPos - v_pos);\n"
        "  // Lambert's cosine law\n"
        "  float diff = max(dot(N, L), 0.0);\n"
        "  vec3 R = reflect(-L, N);\n"
        "  vec3 V = normalize(-v_pos);\n"
        "  // Compute the specular term\n"
        "  float spec = pow(max(dot(V, R), 0.0), u_shininess);\n"
        "  gl_FragColor = vec4(u_ka * u_ambientColor +\n"
        "                      u_kd * diff * u_diffuseColor +\n"
        "                      u_ks * spec * u_specularColor,\n"
        "                      1.0);\n"
        "}";

    PhongColorMaterial::PhongColorMaterial(const glutils::RGBAColor& ambientColor,
                                           const glutils::RGBAColor& diffuseColor,
                                           const glutils::RGBAColor& specularColor,
                                           float ambientCoeff,
                                           float diffuseCoeff,
                                           float specularCoeff,
                                           float shininess
                                          )
        : Material()
        , m_ambientColor(ambientColor)
        , m_diffuseColor(diffuseColor)
        , m_specularColor(specularColor)
        , m_ambientCoeff(ambientCoeff)
        , m_diffuseCoeff(diffuseCoeff)
        , m_specularCoeff(specularCoeff)
        , m_shininess(shininess)
    {
        /* Get/compile shader */
        m_shader = glutils::ShaderManager::getShader(VERT_SHADER_SOURCE, FRAG_SHADER_SOURCE);
        if (nullptr != m_shader)
        {
            /* Add uniforms */
            m_shader->addUniform<glutils::UniformMat4>(MVMX_UNIF_NAME);
            m_shader->addUniform<glutils::UniformMat4>(PMX_UNIF_NAME);
            m_shader->addUniform<glutils::UniformMat4>(NORMMX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1f>(KA_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1f>(KD_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1f>(KS_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1f>(SHININESS_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(AMBIENTCOLOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(DIFFUSECOLOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(SPECULARCOLOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(LIGHTPOS_UNIF_NAME);
        }
    }

    void PhongColorMaterial::onSetup(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec)
    {
        /* Get uniforms */
        glutils::UniformMat4Ptr mvmxUnif          = m_shader->addUniform<glutils::UniformMat4>(MVMX_UNIF_NAME);
        glutils::UniformMat4Ptr pmxUnif           = m_shader->addUniform<glutils::UniformMat4>(PMX_UNIF_NAME);
        glutils::UniformMat4Ptr normmxUnif        = m_shader->addUniform<glutils::UniformMat4>(NORMMX_UNIF_NAME);
        glutils::Uniform1fPtr   kaUnif            = m_shader->addUniform<glutils::Uniform1f>(KA_UNIF_NAME);
        glutils::Uniform1fPtr   kdUnif            = m_shader->addUniform<glutils::Uniform1f>(KD_UNIF_NAME);
        glutils::Uniform1fPtr   ksUnif            = m_shader->addUniform<glutils::Uniform1f>(KS_UNIF_NAME);
        glutils::Uniform1fPtr   shininessUnif     = m_shader->addUniform<glutils::Uniform1f>(SHININESS_UNIF_NAME);
        glutils::Uniform3fPtr   ambientColorUnif  = m_shader->addUniform<glutils::Uniform3f>(AMBIENTCOLOR_UNIF_NAME);
        glutils::Uniform3fPtr   diffuseColorUnif  = m_shader->addUniform<glutils::Uniform3f>(DIFFUSECOLOR_UNIF_NAME);
        glutils::Uniform3fPtr   specularColorUnif = m_shader->addUniform<glutils::Uniform3f>(SPECULARCOLOR_UNIF_NAME);
        glutils::Uniform3fPtr   lightPosUnif      = m_shader->addUniform<glutils::Uniform3f>(LIGHTPOS_UNIF_NAME);

        /* Make sure uniforms are valid */
        if (
            (nullptr != mvmxUnif)          &&
            (nullptr != pmxUnif)           &&
            (nullptr != normmxUnif)        &&
            (nullptr != kaUnif)            &&
            (nullptr != kdUnif)            &&
            (nullptr != ksUnif)            &&
            (nullptr != shininessUnif)     &&
            (nullptr != ambientColorUnif)  &&
            (nullptr != diffuseColorUnif)  &&
            (nullptr != specularColorUnif) &&
            (nullptr != lightPosUnif)
           )
        {
            /* Set uniforms */
            mvmxUnif->setAndCommit(mvMatrix);
            pmxUnif->setAndCommit(projectionMatrix);
            normmxUnif->setAndCommit(normalMatrix);
            kaUnif->setAndCommit(m_ambientCoeff);
            kdUnif->setAndCommit(m_diffuseCoeff);
            ksUnif->setAndCommit(m_specularCoeff);
            shininessUnif->setAndCommit(m_shininess);
            ambientColorUnif->setAndCommit(m_ambientColor.toVec3());
            diffuseColorUnif->setAndCommit(m_diffuseColor.toVec3());
            specularColorUnif->setAndCommit(m_specularColor.toVec3());

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
