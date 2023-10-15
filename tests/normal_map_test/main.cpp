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

#include <iostream>

/* Port includes, for display and input devices */
#include "ares/port/X11Display.hpp"
#include "ares/port/X11Input.hpp"

/* Core includes for ARES 3D objects */
#include "ares/core/DrawingContext.hpp"
#include "ares/core/EventDispatcher.hpp"
#include "ares/core/FPSCameraController.hpp"
#include "ares/core/NormalMapMaterial.hpp"
#include "ares/core/Mesh.hpp"
#include "ares/core/Scene.hpp"
#include "ares/core/LightNode.hpp"
#include "ares/core/MeshNode.hpp"
#include "ares/core/PointLight.hpp"
#include "ares/core/PerspectiveCamera.hpp"
#include "ares/core/Renderer.hpp"
#include "ares/glutils/PngLoader.hpp"
#include "ares/glutils/Texture.hpp"

/* Width and height of the window */
constexpr int32_t windowWidth  = 1920;
constexpr int32_t windowHeight = 1080;

/* PNG filenames for textures, assuming the application is run from a 'build' dir */
constexpr char DIFFUSE_TEX[] = "../tests/normal_map_test/grey.png";
constexpr char NORMAL_TEX[] = "../tests/normal_map_test/normal.png";

/* Cube vertex data */
constexpr GLfloat vertexData[] =
{
    /* Front face */
    /* Bottom-left */
    -1.f, -1.f, 1.f,          //POSITION
    0.0f, 0.0f, 1.0f,         //NORMAL
    1.0f, 0.0f, 0.0f, 1.0f,   //TANGENT
    0.0F, 0.0F,               //TEXCOORD_0
    1.0f, 0.0f, 0.0f,         //COLOR_0
    /* Bottom-right */
    1.f, -1.f, 1.f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    -1.f, 1.f, 1.f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    -1.f, 1.f, 1.f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    1.f, -1.f, 1.f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-right */
    1.f, 1.f, 1.f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    
    /* Right face */
    /* Bottom-left */
    1.f, -1.f, 1.f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    0.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    1.f, -1.f, -1.f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    1.f, 1.f, 1.f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    1.f, 1.f, 1.f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    1.f, -1.f, -1.f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-right */
    1.f, 1.f, -1.f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    1.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    
    /* Back face */
    /* Bottom-left */
    1.f, -1.f, -1.f,
    0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    0.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    -1.f, -1.f, -1.f,
    0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    1.f, 1.f, -1.f,
    0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    1.f, 1.f, -1.f,
    0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    -1.f, -1.f, -1.f,
    0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-right */
    -1.f, 1.f, -1.f,
    0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    
    /* Left face */
    /* Bottom-left */
    -1.f, -1.f, -1.f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    -1.f, -1.f, 1.f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    -1.f, 1.f, -1.f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    -1.f, 1.f, -1.f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    -1.f, -1.f, 1.f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-right */
    -1.f, 1.f, 1.f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    
    /* Top face */
    /* Bottom-left */
    -1.f, 1.f, 1.f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    1.f, 1.f, 1.f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    -1.f, 1.f, -1.f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-left */
    -1.f, 1.f, -1.f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
    /* Bottom-right */
    1.f, 1.f, 1.f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 1.0f,
    1.0F, 0.0F,
    1.0f, 0.0f, 0.0f,
    /* Top-right */
    1.f, 1.f, -1.f,
    0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    1.0F, 1.0F,
    1.0f, 0.0f, 0.0f,
}; 

int main(int /*argc*/, char** /*argv*/)
{
    /* Create display and input devices */
    ares::port::X11DisplayPtr displayDevice = std::make_shared<ares::port::X11Display>(windowWidth, windowHeight);
    if (nullptr == displayDevice)
    {
        std::cout << "Failed to create display device" << std::endl;
        return -1;
    }
    ares::port::X11InputPtr inputDevice = std::make_shared<ares::port::X11Input>(displayDevice);
    if (nullptr == inputDevice)
    {
        std::cout << "Failed to create input device" << std::endl;
        return -1;
    }

    /* Create drawing context and event dispatched with created devices */
    ares::core::DrawingContextPtr drawingContext = std::make_shared<ares::core::DrawingContext>(displayDevice);
    if (nullptr == drawingContext)
    {
        std::cout << "Failed to create drawing context" << std::endl;
        return -1;
    }
    ares::core::EventDispatcherPtr eventDispatcher = std::make_shared<ares::core::EventDispatcher>(inputDevice);
    if (nullptr == eventDispatcher)
    {
        std::cout << "Failed to create event dispatcher" << std::endl;
        return -1;
    }
    
    /* Create scene */
    ares::core::ScenePtr scene = std::make_shared<ares::core::Scene>("test_scene", drawingContext);
    if (nullptr == scene)
    {
        std::cout << "Failed to create scene" << std::endl;
        return -1;
    }
    scene->activate();

    /* Create material using diffuse color and normal map textures */
    ares::glutils::ImagePtr img1 = ares::glutils::PngLoader::loadPng(DIFFUSE_TEX, false);
    if (nullptr == img1)
    {
        std::cout << "Failed to load image file" << std::endl;
        return -1;
    }
    ares::glutils::TexturePtr tex1 = std::make_shared<ares::glutils::Texture>(img1);
    ares::glutils::ImagePtr img2 = ares::glutils::PngLoader::loadPng(NORMAL_TEX, false);
    if (nullptr == img2)
    {
        std::cout << "Failed to load image file" << std::endl;
        return -1;
    }
    ares::glutils::TexturePtr tex2 = std::make_shared<ares::glutils::Texture>(img2);
    ares::core::MaterialPtr material = std::make_shared<ares::core::NormalMapMaterial>(tex1, tex2);

    /* Create mesh using the vertex data attributes */
    ares::core::MeshNodePtr meshNode = scene->createNode<ares::core::MeshNode>("meshNode", scene->rootNode());
    ares::glutils::VboPtr vbo = std::make_shared<ares::glutils::Vbo>(vertexData, sizeof(vertexData), ares::glutils::Vbo::TargetType::ArrayBuffer);
    std::vector<ares::glutils::AttributeDataPtr> attribData;
    attribData.push_back(std::make_shared<ares::glutils::AttributeData>("POSITION", vbo, 3, ares::glutils::AttributeData::AttributeType::Float, false, 60, 0));
    attribData.push_back(std::make_shared<ares::glutils::AttributeData>("NORMAL", vbo, 3, ares::glutils::AttributeData::AttributeType::Float, false, 60, 12));
    attribData.push_back(std::make_shared<ares::glutils::AttributeData>("TANGENT", vbo, 4, ares::glutils::AttributeData::AttributeType::Float, false, 60, 24));
    attribData.push_back(std::make_shared<ares::glutils::AttributeData>("TEXCOORD_0", vbo, 2, ares::glutils::AttributeData::AttributeType::Float, false, 60, 40));
    attribData.push_back(std::make_shared<ares::glutils::AttributeData>("COLOR_0", vbo, 3, ares::glutils::AttributeData::AttributeType::Float, false, 60, 48));
    ares::core::PrimitivePtr primitive = std::make_shared<ares::core::Primitive>(attribData, ares::core::Primitive::PrimitiveType::Triangles, 30, material);
    ares::core::MeshPtr mesh = std::make_shared<ares::core::Mesh>("");
    mesh->addPrimitive(primitive);
    meshNode->setMesh(mesh);
    meshNode->setPosition(0.F, 0.F, -3.F);

    /* Create light */
    ares::core::LightNodePtr lightNode = scene->createNode<ares::core::LightNode>("lightNode", scene->rootNode());
    ares::core::PointLightPtr pointLight = std::make_shared<ares::core::PointLight>();
    lightNode->setLight(pointLight);
    lightNode->setPosition(1.F, 3.F, 2.F);

    /* Create camera and controller */
    ares::core::CameraNodePtr cameraNode = scene->createNode<ares::core::CameraNode>("cameraNode", scene->rootNode());
    ares::core::PerspectiveCameraPtr camera = std::make_shared<ares::core::PerspectiveCamera>(2.F, 1.F, 0.01F, 10000.F);
    cameraNode->setCamera(camera);
    ares::core::FPSCameraControllerPtr cameraController = std::make_shared<ares::core::FPSCameraController>(eventDispatcher, cameraNode);
    scene->setActiveCameraNode(cameraNode);

    /* Create renderer */
    ares::core::RendererPtr renderer = std::make_shared<ares::core::Renderer>();

    /* Main render loop */
    float x = -6.F;
    int32_t dir = 1;
    while (ares::port::DisplayDevice::State::Open == displayDevice->state())
    {
        /* Process events and camera controller */
        eventDispatcher->processEvents();
        cameraController->process();
        
        /* Set light position */
        lightNode->setPosition(x, 1.F, 1.F);

        /* Render scene */
        renderer->render(scene);
        
        /* Update light position for next iteration */
        x += .05F * dir;
        if (x < -6.F || x > 6.F)
        {
        	dir *= -1;
        }
    }
    
    return 0;
}
