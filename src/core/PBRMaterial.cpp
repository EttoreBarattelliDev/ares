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

#include "ares/core/PBRMaterial.hpp"
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
    constexpr char MVMX_UNIF_NAME[]                    = "u_mvMx";
    constexpr char PMX_UNIF_NAME[]                     = "u_pMx";
    constexpr char NORMMX_UNIF_NAME[]                  = "u_normMx";
    constexpr char LIGHTPOS_UNIF_NAME[]                = "u_lightPos";
    constexpr char BASE_COLOR_FACTOR_UNIF_NAME[]       = "u_baseColorFactor";
    constexpr char EMISSIVE_FACTOR_UNIF_NAME[]         = "u_emissiveFactor";
    constexpr char METALLIC_FACTOR_UNIF_NAME[]         = "u_metallicFactor";
    constexpr char ROUGHNESS_FACTOR_UNIF_NAME[]        = "u_roughnessFactor";
    constexpr char BASE_COLOR_TEX_UNIF_NAME[]          = "u_baseColorTex";
    constexpr char EMISSIVE_TEX_UNIF_NAME[]            = "u_emissiveTex";
    constexpr char NORMAL_TEX_UNIF_NAME[]              = "u_normalTex";
    constexpr char OCCLUSION_TEX_UNIF_NAME[]           = "u_occlusionTex";
    constexpr char METAL_ROUGHNESS_TEX_UNIF_NAME[]     = "u_metalRoughnessTex";
    constexpr char HAS_BASE_COLOR_TEX_UNIF_NAME[]      = "u_hasBaseColorTex";
    constexpr char HAS_EMISSIVE_TEX_UNIF_NAME[]        = "u_hasEmissiveTex";
    constexpr char HAS_NORMAL_TEX_UNIF_NAME[]          = "u_hasNormalTex";
    constexpr char HAS_OCCLUSION_TEX_UNIF_NAME[]       = "u_hasOcclusionTex";
    constexpr char HAS_METAL_ROUGHNESS_TEX_UNIF_NAME[] = "u_hasMetalRoughnessTex";

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
        "uniform vec3 u_baseColorFactor;\n"
        "uniform vec3 u_emissiveFactor;\n"
        "uniform float u_metallicFactor;\n"
        "uniform float u_roughnessFactor;\n"
        "uniform sampler2D u_baseColorTex;\n"
        "uniform sampler2D u_emissiveTex;\n"
        "uniform sampler2D u_normalTex;\n"
        "uniform sampler2D u_occlusionTex;\n"
        "uniform sampler2D u_metalRoughnessTex;\n"
        "uniform int u_hasBaseColorTex;\n"
        "uniform int u_hasEmissiveTex;\n"
        "uniform int u_hasNormalTex;\n"
        "uniform int u_hasOcclusionTex;\n"
        "uniform int u_hasMetalRoughnessTex;\n"
        "\n"
        "#define PI 3.14159265359"
        "\n"
        "// ----------------------------------------------------------------------------\n"
        "// Easy trick to get tangent-normals to world-space to keep PBR code simplified.\n"
        "// Don't worry if you don't get what's going on; you generally want to do normal \n"
        "// mapping the usual way for performance anyways; I do plan make a note of this \n"
        "// technique somewhere later in the normal mapping tutorial.\n"
        "vec3 getNormalFromMap()\n"
        "{\n"
        "    vec3 tangentNormal = mix(vec3(0.0, 0.0, 1.0), texture2D(u_normalTex, v_uv).xyz * 2.0 - 1.0, float(u_hasNormalTex));\n"
        "\n"
        "    vec3 N   = v_norm;\n"
        //TODO add configuration for tangents, with attributes or computed here
        "    vec3 T  = normalize(cross(vec3(1.0), N));\n"
        //"    vec3 T  = v_tang;\n"
        "    vec3 B  = -normalize(cross(N, T));\n"
        "    mat3 TBN = mat3(T, B, N);\n"
        "\n"
        "    return normalize(TBN * tangentNormal);\n"
        "}\n"
        "// ----------------------------------------------------------------------------\n"
        "float DistributionGGX(vec3 N, vec3 H, float roughness)\n"
        "{\n"
        "    float a = roughness*roughness;\n"
        "    float a2 = a*a;\n"
        "    float NdotH = max(dot(N, H), 0.0);\n"
        "    float NdotH2 = NdotH*NdotH;\n"
        "\n"
        "    float nom   = a2;\n"
        "    float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
        "    denom = PI * denom * denom;\n"
        "\n"
        "    return nom / denom;\n"
        "}\n"
        "// ----------------------------------------------------------------------------\n"
        "float GeometrySchlickGGX(float NdotV, float roughness)\n"
        "{\n"
        "    float r = (roughness + 1.0);\n"
        "    float k = (r*r) / 8.0;\n"
        "\n"
        "    float nom   = NdotV;\n"
        "    float denom = NdotV * (1.0 - k) + k;\n"
        "\n"
        "    return nom / denom;\n"
        "}\n"
        "// ----------------------------------------------------------------------------\n"
        "float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)\n"
        "{\n"
        "    float NdotV = max(dot(N, V), 0.0);\n"
        "    float NdotL = max(dot(N, L), 0.0);\n"
        "    float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
        "    float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"
        "\n"
        "    return ggx1 * ggx2;\n"
        "}\n"
        "// ----------------------------------------------------------------------------\n"
        "vec3 fresnelSchlick(float cosTheta, vec3 F0)\n"
        "{\n"
        "    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
        "}\n"
        "// ----------------------------------------------------------------------------\n"
        "void main()\n"
        "{		\n"
        //TODO add factors and options to not have textures
        //"    vec3 albedo     = mix(vec3(1.0), pow(texture2D(u_baseColorTex, v_uv).rgb, vec3(2.2)), float(u_hasBaseColorTex)) * u_baseColorFactor;\n"
        "    vec3 albedo     = mix(vec3(1.0), texture2D(u_baseColorTex, v_uv).rgb, float(u_hasBaseColorTex)) * u_baseColorFactor;\n"
        "    vec3 metalRough = mix(vec3(1.0), texture2D(u_metalRoughnessTex, v_uv).rgb, float(u_hasMetalRoughnessTex));\n"
        "    float metallic  = metalRough.g * u_metallicFactor;\n"
        "    float roughness = metalRough.b * u_roughnessFactor;\n"
        "    float ao        = mix(1.0, texture2D(u_occlusionTex, v_uv).r, float(u_hasOcclusionTex));\n"
        "    vec3 emissive   = mix(vec3(1.0), texture2D(u_emissiveTex, v_uv).rgb, float(u_hasEmissiveTex)) * u_emissiveFactor;\n"
        "\n"
        "    vec3 N = getNormalFromMap();\n"
        "    vec3 V = normalize(-v_pos);\n"
        "\n"
        "    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 \n"
        "    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    \n"
        "    vec3 F0 = vec3(0.04); \n"
        "    F0 = mix(F0, albedo, metallic);\n"
        "\n"
        "    // reflectance equation\n"
        "    vec3 Lo = vec3(0.0);\n"
        "\n"
        "    // calculate per-light radiance\n"
        "    vec3 L = normalize(u_lightPos - v_pos);\n"
        "    vec3 H = normalize(V + L);\n"
        "    float distance = length(L);\n"
        "    float attenuation = 1.0 / (distance * distance);\n"
        "    vec3 radiance = vec3(1.0) * attenuation;\n"
        "\n"
        "    // Cook-Torrance BRDF\n"
        "    float NDF = DistributionGGX(N, H, roughness);   \n"
        "    float G   = GeometrySmith(N, V, L, roughness);     \n" 
        "    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);\n"
        "       \n"
        "    vec3 numerator    = NDF * G * F; \n"
        "    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero\n"
        "    vec3 specular = numerator / denominator;\n"
        "    \n"
        "    // kS is equal to Fresnel\n"
        "    vec3 kS = F;\n"
        "    // for energy conservation, the diffuse and specular light can't\n"
        "    // be above 1.0 (unless the surface emits light); to preserve this\n"
        "    // relationship the diffuse component (kD) should equal 1.0 - kS.\n"
        "    vec3 kD = vec3(1.0) - kS;\n"
        "    // multiply kD by the inverse metalness such that only non-metals \n"
        "    // have diffuse lighting, or a linear blend if partly metal (pure metals\n"
        "    // have no diffuse light).\n"
        "    kD *= 1.0 - metallic;	  \n"
        "\n"
        "    // scale light by NdotL\n"
        "    float NdotL = max(dot(N, L), 0.0);        \n"
        "\n"
        "    // add to outgoing radiance Lo\n"
        "    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again\n"
        "    \n"
        "    // ambient lighting (note that the next IBL tutorial will replace \n"
        "    // this ambient lighting with environment lighting).\n"
        "    vec3 ambient = vec3(0.09) * albedo * ao;\n"
        "    \n"
        "    vec3 color = ambient + Lo;\n"
        "\n"
        "    // HDR tonemapping\n"
        "    color = color / (color + vec3(1.0));\n"
        "    // gamma correct\n"
        "    color = pow(color, vec3(1.0/2.2)); \n"
        "\n"
        "    gl_FragColor = vec4(color + emissive, 1.0);\n"
        "}\n";

    PBRMaterial::PBRMaterial(
        const glutils::Vec3& baseColorFactor,
        const glutils::Vec3& emissiveFactor,
        float metallicFactor,
        float roughnessFactor,
        const glutils::TexturePtr& baseColorTex,
        const glutils::TexturePtr& emissiveTex,
        const glutils::TexturePtr& normalTex,
        const glutils::TexturePtr& occlusionTex,
        const glutils::TexturePtr& metallicRoughnessTex
    )
        : Material()
        , m_baseColorFactor(baseColorFactor)
        , m_emissiveFactor(emissiveFactor)
        , m_metallicFactor(metallicFactor)
        , m_roughnessFactor(roughnessFactor)
        , m_baseColorTex(baseColorTex)
        , m_emissiveTex(emissiveTex)
        , m_normalTex(normalTex)
        , m_occlusionTex(occlusionTex)
        , m_metallicRoughnessTex(metallicRoughnessTex)
    {
        /* Get/compile shader */
        m_shader = glutils::ShaderManager::getShader(VERT_SHADER_SOURCE, FRAG_SHADER_SOURCE);
        if (nullptr != m_shader)
        {
            /* Add uniforms */
            m_shader->addUniform<glutils::UniformMat4>(MVMX_UNIF_NAME);
            m_shader->addUniform<glutils::UniformMat4>(PMX_UNIF_NAME);
            m_shader->addUniform<glutils::UniformMat4>(NORMMX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(LIGHTPOS_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(BASE_COLOR_FACTOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform3f>(EMISSIVE_FACTOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1f>(METALLIC_FACTOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1f>(ROUGHNESS_FACTOR_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(BASE_COLOR_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(EMISSIVE_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(NORMAL_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(OCCLUSION_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(METAL_ROUGHNESS_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(HAS_BASE_COLOR_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(HAS_EMISSIVE_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(HAS_NORMAL_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(HAS_OCCLUSION_TEX_UNIF_NAME);
            m_shader->addUniform<glutils::Uniform1i>(HAS_METAL_ROUGHNESS_TEX_UNIF_NAME);
        }
    }

    void PBRMaterial::onSetup(const glutils::Mat4& mvMatrix, const glutils::Mat4& projectionMatrix, const glutils::Mat4& normalMatrix, const std::vector<LightNodePtr>& lightVec)
    {
        /* Get uniforms */
        glutils::UniformMat4Ptr mvmxUnif                 = m_shader->getUniform<glutils::UniformMat4>(MVMX_UNIF_NAME);
        glutils::UniformMat4Ptr pmxUnif                  = m_shader->getUniform<glutils::UniformMat4>(PMX_UNIF_NAME);
        glutils::UniformMat4Ptr normmxUnif               = m_shader->getUniform<glutils::UniformMat4>(NORMMX_UNIF_NAME);
        glutils::Uniform3fPtr   lightPosUnif             = m_shader->getUniform<glutils::Uniform3f>(LIGHTPOS_UNIF_NAME);
        glutils::Uniform3fPtr   baseColorFactorUnif      = m_shader->getUniform<glutils::Uniform3f>(BASE_COLOR_FACTOR_UNIF_NAME);
        glutils::Uniform3fPtr   emissiveFactorUnif       = m_shader->getUniform<glutils::Uniform3f>(EMISSIVE_FACTOR_UNIF_NAME);
        glutils::Uniform1fPtr   metallicFactorUnif       = m_shader->getUniform<glutils::Uniform1f>(METALLIC_FACTOR_UNIF_NAME);
        glutils::Uniform1fPtr   roughnessFactorUnif      = m_shader->getUniform<glutils::Uniform1f>(ROUGHNESS_FACTOR_UNIF_NAME);
        glutils::Uniform1iPtr   baseColorTexUnif         = m_shader->getUniform<glutils::Uniform1i>(BASE_COLOR_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   emissiveTexUnif          = m_shader->getUniform<glutils::Uniform1i>(EMISSIVE_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   normalTexUnif            = m_shader->getUniform<glutils::Uniform1i>(NORMAL_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   occlusionTexUnif         = m_shader->getUniform<glutils::Uniform1i>(OCCLUSION_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   metalRoughnessTexUnif    = m_shader->getUniform<glutils::Uniform1i>(METAL_ROUGHNESS_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   hasBaseColorTexUnif      = m_shader->getUniform<glutils::Uniform1i>(HAS_BASE_COLOR_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   hasEmissiveTexUnif       = m_shader->getUniform<glutils::Uniform1i>(HAS_EMISSIVE_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   hasNormalTexUnif         = m_shader->getUniform<glutils::Uniform1i>(HAS_NORMAL_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   hasOcclusionTexUnif      = m_shader->getUniform<glutils::Uniform1i>(HAS_OCCLUSION_TEX_UNIF_NAME);
        glutils::Uniform1iPtr   hasMetalRoughnessTexUnif = m_shader->getUniform<glutils::Uniform1i>(HAS_METAL_ROUGHNESS_TEX_UNIF_NAME);

        /* Make sure uniforms are valid */
        if (
            (nullptr != mvmxUnif)              &&
            (nullptr != pmxUnif)               &&
            (nullptr != normmxUnif)            &&
            (nullptr != baseColorFactorUnif)   &&
            (nullptr != emissiveFactorUnif)    &&
            (nullptr != roughnessFactorUnif)   &&
            (nullptr != lightPosUnif)          &&
            (nullptr != lightPosUnif)          &&
            (nullptr != baseColorTexUnif)      &&
            (nullptr != emissiveTexUnif)       &&
            (nullptr != normalTexUnif)         &&
            (nullptr != occlusionTexUnif)      &&
            (nullptr != metalRoughnessTexUnif) &&
            (nullptr != hasBaseColorTexUnif)   &&
            (nullptr != hasEmissiveTexUnif)    &&
            (nullptr != hasNormalTexUnif)      &&
            (nullptr != hasOcclusionTexUnif)   &&
            (nullptr != hasMetalRoughnessTexUnif)
           )
        {
            /* Set uniforms */
            mvmxUnif->setAndCommit(mvMatrix);
            pmxUnif->setAndCommit(projectionMatrix);
            normmxUnif->setAndCommit(normalMatrix);
            baseColorFactorUnif->setAndCommit(m_baseColorFactor);
            emissiveFactorUnif->setAndCommit(m_emissiveFactor);
            metallicFactorUnif->setAndCommit(m_metallicFactor);
            roughnessFactorUnif->setAndCommit(m_roughnessFactor);
            baseColorTexUnif->setAndCommit(0);
            emissiveTexUnif->setAndCommit(1);
            normalTexUnif->setAndCommit(2);
            occlusionTexUnif->setAndCommit(3);
            metalRoughnessTexUnif->setAndCommit(4);

            /* Activate texture */
            hasBaseColorTexUnif->setAndCommit((nullptr != m_baseColorTex) ? (1) : (0));
            if (nullptr != m_baseColorTex)
            {
                m_baseColorTex->activate(0);
            }
            
            hasEmissiveTexUnif->setAndCommit((nullptr != m_emissiveTex) ? (1) : (0));
            if (nullptr != m_emissiveTex)
            {
                m_emissiveTex->activate(1);
            }

            hasNormalTexUnif->setAndCommit((nullptr != m_normalTex) ? (1) : (0));
            if (nullptr != m_normalTex)
            {
                m_normalTex->activate(2);
            }

            hasOcclusionTexUnif->setAndCommit((nullptr != m_occlusionTex) ? (1) : (0));
            if (nullptr != m_occlusionTex)
            {
                m_occlusionTex->activate(3);
            }

            hasMetalRoughnessTexUnif->setAndCommit((nullptr != m_metallicRoughnessTex) ? (1) : (0));
            if (nullptr != m_metallicRoughnessTex)
            {
                m_metallicRoughnessTex->activate(4);
            }

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
