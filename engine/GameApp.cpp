
#include "GameApp.h"

#include <spdlog/spdlog.h>


GameApp::GameApp(std::string name):name_(name) { }

GameApp::~GameApp() {
    shutdown();
}

// never call init() in the constructor as it involves the virtual function
// it will crash the programme
void GameApp::init() {

    if (bInitialised_) {
         spdlog::error("GameApp already initialised.");
         return;
    }

    spdlog::info("Initialising GameApp and core subsystems ...");

    initWindow(name_);

    initResources();

    bInitialised_ = true;

    spdlog::info("GameApp initialisation complete.");
}

void GameApp::initWindow(std::string title) {

    spdlog::info("Initialising the main window ...");
    
    // 1. Initialize primary window (GLFW context, user pointers, callbacks)
    mainWin_ = std::make_unique<Window>(800, 600, title);
    
}

void GameApp::run() {

    if (! bInitialised_) {
        spdlog::error("GameApp not initialised, please call GameApp's init() before run().");
        return;
    }

    bRunning_ = true;

    spdlog::info("Entering main engine loop.");

    while (bRunning_ && ! mainWin_->shouldClose()) {
        
        // auto currentTime = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<float> elapsed = currentTime - lastFrameTime_;
        // lastFrameTime_ = currentTime;
        // float deltaTime = elapsed.count();

        float dt = clock_.tick();;

        // --- Event Handling ---
        processEvents();

        // --- Engine Systems Update (ECS Logic) ---
        update(dt);

        // --- 4. Mesh Rendering Pass ---
        render();

        // set mouse x and y offsets to 0
        mainWin_->clearInputState();
    }
}

void GameApp::processEvents() {
    
    // Poll window events (triggers GLFW input callbacks bound to your per-window InputState)
    mainWin_->pollEvents();
}


void GameApp::shutdown() {
    spdlog::info("Shutting down GameApp...");
    // Cleanup order handles smart pointers automatically
}