// ROOT/include/GameApp.h

#pragma once

#include <memory>
#include <chrono>
#include "ui/Window.h"
#include "ecs/EcsWorldRegistry.h" 

class GameApp {
public:
    GameApp(std::string name = "My Game App");
    ~GameApp();

    // Prevent copying
    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;

    // Main evemt/rendering loop
    void run();

private:
    void init(std::string title = "My Game App");   // init window and OpenGL setup
    void processEvents();           // poll events
    void update(float dt);          // update data of various system
    void render();            // core rendering implementation
    void shutdown();          // terminate

    bool bRunning_{true};
    
    // Core engine modules (supporting your multi-window architecture)
    std::unique_ptr<Window> mainWin_;
    ECSWorldRegistry ecsWorld_;

    std::string appName_;

    // Timing tracking
    std::chrono::high_resolution_clock::time_point lastFrameTime_;
};
