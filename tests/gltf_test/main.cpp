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
#include "ares/core/Scene.hpp"
#include "ares/core/LightNode.hpp"
#include "ares/core/PointLight.hpp"
#include "ares/core/Renderer.hpp"

/* Gltf include to load GLTF file */
#include "ares/gltf/Gltf.hpp"

/* Width and height of the window */
constexpr int32_t windowWidth  = 1920;
constexpr int32_t windowHeight = 1080;

/* GLTF filename, assuming the application is run from a 'build' dir */
constexpr char GLTF_FILENAME[] = "../third-party/glTF-Sample-Models/2.0/SciFiHelmet/glTF/SciFiHelmet.gltf";

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

    /* Load GLTF file */
    ares::gltf::GltfPtr gltf = std::make_shared<ares::gltf::Gltf>(drawingContext);
    if (nullptr == gltf)
    {
        std::cout << "Failed to create gltf parser" << std::endl;
        return -1;
    }
    bool gltfLoadStatus = gltf->loadFile(GLTF_FILENAME);
    if (!gltfLoadStatus)
    {
        std::cout << "Failed to load gltf file" << std::endl;
        return -1;
    }

    /* Parse scene */
    auto sceneVec = gltf->parse();
    if (sceneVec.empty())
    {
        std::cout << "Failed to parse gltf scene" << std::endl;
        return -1;
    }
    ares::core::ScenePtr scene = sceneVec[0];

    /* Set camera position and create controller */
	ares::core::FPSCameraControllerPtr cameraController;
    ares::core::CameraNodePtr cameraNode = scene->activeCameraNode();
    if (nullptr != cameraNode)
    {
        cameraNode->setPosition(0.f, 0.f, 3.f);
        cameraController = std::make_shared<ares::core::FPSCameraController>(eventDispatcher, cameraNode);
    }

    /* Create a point light and add it to the scene */
    ares::core::LightNodePtr lightNode = scene->createNode<ares::core::LightNode>("lightNode", scene->rootNode());
    if (nullptr == lightNode)
    {
        std::cout << "Failed to create light node" << std::endl;
        return -1;
    }
    ares::core::PointLightPtr pointLight = std::make_shared<ares::core::PointLight>();
    if (nullptr == pointLight)
    {
        std::cout << "Failed to create point light" << std::endl;
        return -1;
    }
    lightNode->setLight(pointLight);
    lightNode->setPosition(0.F, 2.F, 1.F);

    /* Create renderer */
    ares::core::RendererPtr renderer = std::make_shared<ares::core::Renderer>();
    if (nullptr == renderer)
    {
        std::cout << "Failed to create renderer" << std::endl;
        return -1;
    }
    renderer->setBgColor(ares::glutils::RGBAColor(1.F, 1.F, 1.F, 1.F));

    /* Main loop */
    while (ares::port::DisplayDevice::State::Open == displayDevice->state())
    {
        /* Process events and camera controller */
        eventDispatcher->processEvents();
        cameraController->process();

        /* Render scene */
        renderer->render(scene);
    }

    return 0;
}
