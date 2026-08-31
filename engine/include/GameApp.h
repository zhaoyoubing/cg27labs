// engine/include/GameApp.h

#pragma once

#include "Clock.h"
#include "ui/Window.h"
#include "entity/EcsWorldRegistry.h" 
#include "scene/Scene.h"
#include "renderpasses/RenderPipeline.h"
#include "renderpasses/ForwardPass.h"
#include "render/CameraView.h"
#include "resources/MaterialManager.h"
#include "resources/TextureManager.h"

#include <memory>
//#include <chrono>

class GameApp {
public:
    GameApp(std::string name = "My Game App");
    ~GameApp();

    // Prevent copying
    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;

    void init();   // initialisation

    // Main evemt/rendering loop
    void run();

protected:

    //virtual bool initResources();
    virtual void initRenderPipeline() = 0;
    virtual void initScene() = 0;

    virtual void update(float dt) = 0;
    virtual void render() = 0;

    std::string name_;

    // Core engine modules
    std::unique_ptr<Window> mainWin_;
    
    ECSWorldRegistry ecsWorld_; // ECS is scene
    
    CameraView camera_;

    RenderPipeline renderPipe_;
    
    // The currently active scene instance
    std::shared_ptr<Scene> activeScene;

    MaterialManager matMgr_;
    TextureManager texMgr_;
    

private:
    void initWindow(std::string title);   // init window and OpenGL setup
    void processEvents();     // poll events
    void shutdown();          // terminate

    bool bInitialised_{false};
    bool bRunning_{false};
    
    // Timing tracking
    // std::chrono::high_resolution_clock::time_point lastFrameTime_;
    Clock clock_;
};
