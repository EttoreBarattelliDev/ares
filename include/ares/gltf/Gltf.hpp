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

#ifndef GLTF_HPP_INCLUDED
#define GLTF_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>

/* Forwards */
namespace tinygltf
{
    class TinyGLTF;
    class Model;
    class Scene;
    class Node;
    class Camera;
    class Light;
    class Mesh;
}

namespace ares
{

namespace core
{
    class DrawingContext;
    using DrawingContextPtr = std::shared_ptr<DrawingContext>;
    class Scene;
    using ScenePtr = std::shared_ptr<Scene>;
    class Node;
    using NodePtr = std::shared_ptr<Node>;
    class Camera;
    using CameraPtr = std::shared_ptr<Camera>;
    class Light;
    using LightPtr = std::shared_ptr<Light>;
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
    class Material;
    using MaterialPtr = std::shared_ptr<Material>;
}

namespace glutils
{
    class Vbo;
    using VboPtr = std::shared_ptr<Vbo>;
    class Image;
    using ImagePtr = std::shared_ptr<Image>;
    class Texture;
    using TexturePtr = std::shared_ptr<Texture>;
}

namespace gltf
{
    class Gltf;
    using GltfPtr = std::shared_ptr<Gltf>;

    /*!
     * @brief Gltf file loader and parser
     *
     * This class can be used to load gltf file, parse
     * their content and construct an ARES scene with nodes
     * and meshes defined in the gltf
     */
    class Gltf
    {
    public:
        /*!
         * @brief File type enumeration
         */
        enum class FileType
        {
            ASCII,
            BINARY
        };

        /*!
         * @brief Class constructor
         *
         * @param[in] drawingContext - Drawing context
         */
        Gltf(core::DrawingContextPtr drawingContext);

        /*!
         * @brief Class destructor
         */
        ~Gltf();

        Gltf(const Gltf&) = delete;
        Gltf& operator=(const Gltf&) = delete;

        /*!
         * @brief Method to load a gltf file
         *
         * @param[in] filename - Name of the file to load
         * @param[in] fileType - Type of the file to load
         * @return true if success, false otherwise
         */
        bool loadFile(const std::string& filename, FileType fileType = FileType::ASCII);

        /*!
         * @brief Method to parse a loaded gltf file
         *
         * @return Vector of Scene objects with the parsed scenes
         */
        std::vector<core::ScenePtr> parse();

    private:

        /*! Drawing context */
        core::DrawingContextPtr m_drawingContext;

        /*! TinyGLTF loader */
        tinygltf::TinyGLTF* m_loader;

        /*! TinyGLTF model */
        tinygltf::Model* m_model;

        /*! Vector of Vbo object */
        std::vector<glutils::VboPtr> m_vboVector;

        /*! Vector of Image object */
        std::vector<glutils::ImagePtr> m_imageVector;

        /*! Vector of Material object */
        std::vector<core::MaterialPtr> m_materialVector;

        /*! Vector of Camera object */
        std::vector<core::CameraPtr> m_cameraVector;

        /*! Vector of Light object */
        std::vector<core::LightPtr> m_lightVector;

        /*! Vector of Mesh object */
        std::vector<core::MeshPtr> m_meshVector;

        /*! Vector of Texture object */
        std::vector<glutils::TexturePtr> m_textureVector;

        /*! Method to parse buffers in the gltf */
        void parseBuffers();

        /*! Method to parse images in the gltf */
        void parseImages();

        /*! Method to parse textures in the gltf */
        void parseTextures();

        /*! Method to parse materials in the gltf */
        void parseMaterials();

        /*! Method to parse cameras in the gltf */
        void parseCameras();

        /*! Method to parse lights in the gltf */
        void parseLights();

        /*! Method to parse meshes in the gltf */
        void parseMeshes();

        /*! Method to parse a scene in the gltf */
        core::ScenePtr parseScene(const tinygltf::Scene& scene);

        /*! Method to parse a node in the gltf */
        void parseNode(const tinygltf::Node& node, core::ScenePtr scene, core::NodePtr parentNode);
    };
}

}

#endif
