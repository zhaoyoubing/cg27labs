#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PlayerMoveSystem.h"
#include "MyGameApp.h"

#include <glm/glm.hpp>

#include "device/GPUPipeline.h"

#include "entity/EcsTypes.h"
#include "entity/EcsWorldRegistry.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"
#include "systems/CameraSystem.h"

#include "ui/Window.h"
#include "ui/InputState.h"

#include "scene/GltfMeshLoader.h"

#include <spdlog/spdlog.h>
#include <iostream>


int main() {

    std::cout << "Hello, Graphics!" << std::endl; 

    spdlog::set_level(spdlog::level::debug);


    // Instantiate the game-specific application derived from GameApp
    MyGameApp app("TA02");
    
    // calls init(), initializes the window/context
    app.init();

    // starts the engine main event/rendering loop, 
    app.run();
    
    return 0;

}