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

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include <stdexcept>
#include <iostream>

#include "ares/gltf/Gltf.hpp"
#include "ares/glutils/Vbo.hpp"
#include "ares/core/Scene.hpp"
#include "ares/core/CameraNode.hpp"
#include "ares/core/LightNode.hpp"
#include "ares/core/MeshNode.hpp"
#include "ares/core/PerspectiveCamera.hpp"
#include "ares/core/PBRMaterial.hpp"
#include "ares/glutils/Texture.hpp"

namespace ares
{

namespace gltf
{
    constexpr char CAMERA_TYPE_PERSPECTIVE[] = "perspective";

    static int32_t accessorTypeToSize(int32_t accessorType)
    {
        int32_t retval = 0;

        switch (accessorType)
        {
            case TINYGLTF_TYPE_SCALAR:
                retval = 1;
                break;
            case TINYGLTF_TYPE_VEC2:
                retval = 2;
                break;
            case TINYGLTF_TYPE_VEC3:
                retval = 3;
                break;
            case TINYGLTF_TYPE_VEC4:
                retval = 4;
                break;
        }

        return retval;
    }

    static glutils::AttributeData::AttributeType componentTypeToAttrType(int32_t componentType)
    {
        glutils::AttributeData::AttributeType retval = glutils::AttributeData::AttributeType::Byte;

        switch (componentType)
        {
            case TINYGLTF_COMPONENT_TYPE_BYTE:
                retval = glutils::AttributeData::AttributeType::Byte;
                break;
            case TINYGLTF_COMPONENT_TYPE_DOUBLE:
                retval = glutils::AttributeData::AttributeType::Float;
                break;
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                retval = glutils::AttributeData::AttributeType::Float;
                break;
            case TINYGLTF_COMPONENT_TYPE_INT:
                retval = glutils::AttributeData::AttributeType::Int;
                break;
            case TINYGLTF_COMPONENT_TYPE_SHORT:
                retval = glutils::AttributeData::AttributeType::Short;
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                retval = glutils::AttributeData::AttributeType::UnsignedByte;
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                retval = glutils::AttributeData::AttributeType::UnsignedInt;
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                retval = glutils::AttributeData::AttributeType::UnsignedShort;
                break;
            default:
                //ERROR
                break;
        }

        return retval;
    }

    static core::Primitive::PrimitiveType primitiveModeToType(int32_t mode)
    {
        core::Primitive::PrimitiveType retval = core::Primitive::PrimitiveType::Triangles;

        if (mode >= 0)
        {
            switch (mode)
            {
                case TINYGLTF_MODE_TRIANGLE_FAN:
                    retval = core::Primitive::PrimitiveType::TriangleFan;
                    break;
                case TINYGLTF_MODE_TRIANGLE_STRIP:
                    retval = core::Primitive::PrimitiveType::TriangleStrip;
                    break;
                case TINYGLTF_MODE_TRIANGLES:
                    retval = core::Primitive::PrimitiveType::Triangles;
                    break;
                default:
                    //UNSUPPORTED
                    break;
            }
        }

        return retval;
    }

    static glutils::Vbo::TargetType targetType(int32_t targetType)
    {
        glutils::Vbo::TargetType retval = glutils::Vbo::TargetType::Invalid;

        switch (targetType)
        {
            case TINYGLTF_TARGET_ARRAY_BUFFER:
                retval = glutils::Vbo::TargetType::ArrayBuffer;
                break;
            case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER:
                retval = glutils::Vbo::TargetType::ElementArrayBuffer;
                break;
            default:
                break;
        }

        return retval;
    }

    static glutils::Texture::WrapType wrapType(int32_t type)
    {
        glutils::Texture::WrapType retval = glutils::Texture::WrapType::Repeat;

        switch (type)
        {
            case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
                retval = glutils::Texture::WrapType::ClampToEdge;
                break;
            case TINYGLTF_TEXTURE_WRAP_REPEAT:
                retval = glutils::Texture::WrapType::Repeat;
                break;
            case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
                retval = glutils::Texture::WrapType::MirroredRepeat;
                break;
        }

        return retval;
    }

    static glutils::Texture::FilterType filterType(int32_t type)
    {
        glutils::Texture::FilterType retval = glutils::Texture::FilterType::Nearest;

        switch (type)
        {
            case TINYGLTF_TEXTURE_FILTER_LINEAR:
                retval = glutils::Texture::FilterType::Linear;
                break;
            case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
                retval = glutils::Texture::FilterType::LinerMipmapLinear;
                break;
            case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
                retval = glutils::Texture::FilterType::LinearMipmapNearset;
                break;
            case TINYGLTF_TEXTURE_FILTER_NEAREST:
                retval = glutils::Texture::FilterType::Nearest;
                break;
            case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
                retval = glutils::Texture::FilterType::NearestMipmapLinear;
                break;
            case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
                retval = glutils::Texture::FilterType::NearestMipmapNearest;
                break;
        }

        return retval;
    }

    Gltf::Gltf(core::DrawingContextPtr drawingContext)
        : m_drawingContext(drawingContext)
        , m_loader(new tinygltf::TinyGLTF)
        , m_model(new tinygltf::Model)
    {
    }

    Gltf::~Gltf()
    {
        delete m_model;
        delete m_loader;
    }

    bool Gltf::loadFile(const std::string& filename, FileType fileType)
    {
        /* Assume failure */
        bool retval = false;

        /* Check for valid data */
        if ((nullptr != m_loader) && (nullptr != m_model))
        {
            std::string err;
            std::string warn;

            /* Load file depending on type */
            switch (fileType)
            {
            case FileType::ASCII:
                retval = m_loader->LoadASCIIFromFile(m_model, &err, &warn, filename);
                break;
            case FileType::BINARY:
                retval = m_loader->LoadBinaryFromFile(m_model, &err, &warn, filename);
                break;
            default:
                err = "Invalid file type";
                break;
            }
            
            /* Print warnings if any */
            if (!warn.empty())
            {
                std::cout << "WARN: " << warn << std::endl;
            }

            /* Print errors if any */
            if (!err.empty())
            {
                std::cout << "ERR: " << err << std::endl;
            }
        }

        if (!retval)
        {
            throw std::runtime_error("Failed to load gltf file");
        }

        return retval;
    }

    std::vector<core::ScenePtr> Gltf::parse()
    {
        std::vector<core::ScenePtr> sceneVec;

        if (nullptr == m_model)
        {
            throw std::runtime_error("Invalid gltf model");
        }

        /* Parse all data */
        parseBuffers();
        parseImages();
        parseTextures();
        parseMaterials();
        parseCameras();
        parseLights();
        parseMeshes();

        /* Parse all scenes */
        for (const auto& scene : m_model->scenes)
        {
            auto aresScene = parseScene(scene);
            sceneVec.push_back(aresScene);
        }

        /* Clear all temporary data */
        m_cameraVector.clear();
        m_imageVector.clear();
        m_lightVector.clear();
        m_materialVector.clear();
        m_meshVector.clear();
        m_textureVector.clear();
        m_vboVector.clear();

        return sceneVec;
    }

    void Gltf::parseBuffers()
    {
        /* Parse buffers */
        for (const auto& bufferView : m_model->bufferViews)
        {
            /* Only parse array or element array buffers for now */
            glutils::Vbo::TargetType targType = targetType(bufferView.target);
            if (glutils::Vbo::TargetType::Invalid != targType)
            {
                /* Get data and create Vbo */
                const auto& buffer = m_model->buffers[bufferView.buffer];
                const uint8_t* buffPtr = &(buffer.data.data()[bufferView.byteOffset]);
                auto vbo = std::make_shared<glutils::Vbo>(buffPtr, bufferView.byteLength, targType);
                m_vboVector.push_back(vbo);
            }
        }
    }

    void Gltf::parseImages()
    {
        /* Parse images */
        for (const auto& image : m_model->images)
        {
            /* Get image data */
            glutils::Image::Format imgFormat = (image.component == 4) ? (glutils::Image::Format::RGBA) : (glutils::Image::Format::RGB);
            auto aresImage = std::make_shared<glutils::Image>(image.image, imgFormat, image.width, image.height);
            m_imageVector.push_back(aresImage);
        }
    }

    void Gltf::parseTextures()
    {
        /* Parse textures */
        for (const auto& texture : m_model->textures)
        {
            auto wrapS = glutils::Texture::WrapType::Repeat;
            auto wrapT = glutils::Texture::WrapType::Repeat;
            auto minF = glutils::Texture::FilterType::Linear;
            auto magF = glutils::Texture::FilterType::Linear;

            /* Get texture configuration and create texture object */
            if (texture.sampler >= 0)
            {
                const auto& sampler = m_model->samplers[texture.sampler];
                wrapS = wrapType(sampler.wrapS);
                wrapT = wrapType(sampler.wrapT);
                minF = filterType(sampler.minFilter);
                magF = filterType(sampler.magFilter);
            }
            auto aresTex = std::make_shared<glutils::Texture>(m_imageVector[texture.source], wrapS, wrapT, minF, magF);
            m_textureVector.push_back(aresTex);
        }
    }

    void Gltf::parseMaterials()
    {
        /* Parse materials */
        for (const auto& material : m_model->materials)
        {
            /* Parse emissive info */
            glutils::Vec3 emissiveFactor(material.emissiveFactor[0], material.emissiveFactor[1], material.emissiveFactor[2]);
            glutils::TexturePtr emissiveTex;
            if (material.emissiveTexture.index >= 0)
            {
                emissiveTex = m_textureVector[material.emissiveTexture.index];
            }

            /* Parse normal info */
            glutils::TexturePtr normalTex;
            if (material.normalTexture.index >= 0)
            {
                normalTex = m_textureVector[material.normalTexture.index];
            }

            /* Parse occlusion info */
            glutils::TexturePtr occlusionTex;
            if (material.occlusionTexture.index >= 0)
            {
                occlusionTex = m_textureVector[material.occlusionTexture.index];
            }

            /* Parse base color info */
            glutils::Vec3 baseColorFactor(material.pbrMetallicRoughness.baseColorFactor[0], material.pbrMetallicRoughness.baseColorFactor[1], material.pbrMetallicRoughness.baseColorFactor[2]);
            glutils::TexturePtr baseColorTex;
            if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
            {
                baseColorTex = m_textureVector[material.pbrMetallicRoughness.baseColorTexture.index];
            }

            /* Parse metal roughness info */
            float metallicFactor = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
            float roughnessFactor = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
            glutils::TexturePtr metallicRoughnessTex;
            if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
            {
                metallicRoughnessTex = m_textureVector[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
            }

            /* Create material */
            auto aresMaterial = std::make_shared<core::PBRMaterial>(
                                    baseColorFactor,
                                    emissiveFactor,
                                    metallicFactor,
                                    roughnessFactor,
                                    baseColorTex,
                                    emissiveTex,
                                    normalTex,
                                    occlusionTex,
                                    metallicRoughnessTex);

            m_materialVector.push_back(aresMaterial);
        }
    }

    void Gltf::parseCameras()
    {
        /* Parse cameras */
        for (const auto& camera : m_model->cameras)
        {
            /* Check camera type */
            if (camera.type == CAMERA_TYPE_PERSPECTIVE)
            {
                /* Create camera */
                const auto& persp = camera.perspective;
                auto aresCamera = std::make_shared<core::PerspectiveCamera>(persp.aspectRatio, persp.yfov, persp.znear, persp.zfar);
                m_cameraVector.push_back(aresCamera);
            }
        }
    }

    void Gltf::parseLights()
    {
        /* Parse lights */
        for (const auto& light : m_model->lights)
        {
            //TODO
        }
    }

    core::ScenePtr Gltf::parseScene(const tinygltf::Scene& scene)
    {
        /* Create scene */
        core::ScenePtr aresScene = std::make_shared<core::Scene>(scene.name, m_drawingContext);

        /* Parse scenes */
        for (const auto& nodeIdx : scene.nodes)
        {
            /* Parse all scene nodes recursively */
            const auto& node = m_model->nodes[nodeIdx];
            parseNode(node, aresScene, aresScene->rootNode());
        }

        /* Create a default camera if none is defined */
        if (nullptr == aresScene->activeCameraNode())
        {
            ares::core::CameraNodePtr cameraNode = aresScene->createNode<ares::core::CameraNode>("cameraNode", aresScene->rootNode());
            ares::core::PerspectiveCameraPtr camera = std::make_shared<ares::core::PerspectiveCamera>(1.7778F, 1.05F, 0.01F, 1000.F);
            cameraNode->setCamera(camera);
            aresScene->setActiveCameraNode(cameraNode);
        }

        return aresScene;
    }

    void Gltf::parseNode(const tinygltf::Node& node, core::ScenePtr scene, core::NodePtr parentNode)
    {
        core::NodePtr aresNode;

        /* Check node type */
        if (node.camera >= 0)
        {
            /* Camera node */
            auto aresCameraNode = scene->createNode<core::CameraNode>(node.name, parentNode);
            aresCameraNode->setCamera(m_cameraVector[node.camera]);
            aresNode = aresCameraNode;
            scene->setActiveCameraNode(aresCameraNode);
        }
        else if (node.light >= 0)
        {
            /* Light node */
            auto aresLightNode = scene->createNode<core::LightNode>(node.name, parentNode);
            aresLightNode->setLight(m_lightVector[node.light]);
            aresNode = aresLightNode;
        }
        else if (node.mesh >= 0)
        {
            /* Mesh node */
            auto aresMeshNode = scene->createNode<core::MeshNode>(node.name, parentNode);
            aresMeshNode->setMesh(m_meshVector[node.mesh]);
            aresNode = aresMeshNode;
        }
        else
        {
            /* Empty node */
            aresNode = scene->createNode<core::Node>(node.name, parentNode);
        }
        
        if (!node.matrix.empty())
        {
            //TODO check row vs column major
            ares::glutils::Mat4 mat(node.matrix);
            aresNode->setTransformMatrix(mat);
        }
        else
        {
            if (!node.translation.empty())
            {
                /* Set position */
                aresNode->setPosition(node.translation[0], node.translation[1], node.translation[2]);
            }
            if (!node.rotation.empty())
            {
                /* Set rotation */
                aresNode->setRotationQuaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
            }
            if (!node.scale.empty())
            {
                /* Set scaling */
                aresNode->setScaling(node.scale[0], node.scale[1], node.scale[2]);
            }
        }

        /* Parse child nodes */
        for (const auto& childNodeIdx : node.children)
        {
            const auto& childNode = m_model->nodes[childNodeIdx];
            parseNode(childNode, scene, aresNode);
        }
    }

    void Gltf::parseMeshes()
    {
        /* Parse meshes */
        for (const auto& mesh : m_model->meshes)
        {
            std::vector<core::PrimitivePtr> primVec;

            /* Parse primitives for this mesh */
            for (const auto& primitive : mesh.primitives)
            {
                std::vector<glutils::AttributeDataPtr> attrDataVec;
                int32_t vertexCount = 0;

                /* Parse attributes for this primitive */
                for (const auto& attributePair : primitive.attributes)
                {
                    /* Get data for this attribute */
                    const auto& attribName = attributePair.first;
                    const auto& accessor = m_model->accessors[attributePair.second];
                    const auto& bufferView = m_model->bufferViews[accessor.bufferView];

                    int32_t itemSize = accessorTypeToSize(accessor.type);
                    glutils::AttributeData::AttributeType glType = componentTypeToAttrType(accessor.componentType);

                    /* Create attribute data */
                    auto attrData = std::make_shared<glutils::AttributeData>(
                                                            attribName,
                                                            m_vboVector[accessor.bufferView],
                                                            itemSize,
                                                            glType,
                                                            accessor.normalized,
                                                            bufferView.byteStride,
                                                            accessor.byteOffset);
                    attrDataVec.push_back(attrData);
                    vertexCount = accessor.count;
                }

                /* Check if primitive has indices */
                glutils::AttributeDataPtr indicesVbo;
                if (primitive.indices >= 0)
                {
                    /* Get indices data */
                    const auto& accessor = m_model->accessors[primitive.indices];
                    const auto& bufferView = m_model->bufferViews[accessor.bufferView];

                    int32_t itemSize = accessorTypeToSize(accessor.type);
                    glutils::AttributeData::AttributeType glType = componentTypeToAttrType(accessor.componentType);

                    /* Create attribute data for indices */
                    indicesVbo = std::make_shared<glutils::AttributeData>(
                                        "",
                                        m_vboVector[accessor.bufferView],
                                        itemSize,
                                        glType,
                                        accessor.normalized,
                                        bufferView.byteStride,
                                        accessor.byteOffset);
                    vertexCount = accessor.count;
                }

                /* Create primitive */
                auto aresPrim = std::make_shared<core::Primitive>(attrDataVec, primitiveModeToType(primitive.mode), vertexCount, m_materialVector[primitive.material], indicesVbo);
                primVec.push_back(aresPrim);
            }

            /* Create mesh */
            auto aresMesh = std::make_shared<core::Mesh>(mesh.name, primVec);
            m_meshVector.push_back(aresMesh);
        }
    }

}

}
