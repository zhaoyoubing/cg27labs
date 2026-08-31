
#include "GameApp.h"

#include <spdlog/spdlog.h>


GameApp::GameApp(std::string name):name_(name) {
    init();
}

GameApp::~GameApp() {
    shutdown();
}

void GameApp::init() {
    spdlog::info("Initializing GameApp and core subsystems...");

    initWindow(name_);

    initRenderPipeline();

    //initResources();

    initScene();

    bInitialised_ = true;

    spdlog::info("GameApp initialisation complete.");
}

void GameApp::initWindow(std::string title) {
    
    // 1. Initialize primary window (GLFW context, user pointers, callbacks)
    mainWin_ = std::make_unique<Window>(800, 600, title);

    // 2. Initialize timing baseline
    lastFrameTime_ = std::chrono::high_resolution_clock::now();
    
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

        float dt = mainWin_->updateDeltaTime();

        // --- Event Handling ---
        processEvents();

        // --- Engine Systems Update (ECS Logic) ---
        update(dt);

        // --- 4. Mesh Rendering Pass ---
        render();
    }
}

void GameApp::processEvents() {
    
    // Poll window events (triggers GLFW input callbacks bound to your per-window InputState)
    mainWin_->pollEvents();
}

/*
void GameApp::update(float deltaTime) {
    
    // Example: Update your ECS systems sequentially
    // CameraSystem::Update(m_ecsWorld, deltaTime);
    // PhysicsSystem::Update(m_ecsWorld, deltaTime);
}


void GameApp::render() {
    // Clear frame buffers
    mainWin_->clearScreen();

    {
        // Optional: GPU profiling zone for your mesh rendering pass
        // TracyGpuZone("Geometry Pass");
        
        // Example: Render loop over mesh components
        // auto view = m_ecsWorld.View<TransformComponent, MeshComponent>();
        // for (auto entity : view) { ... draw mesh ... }
    }

    // Swap front and back buffers
    mainWin_->swapBuffers();
}
*/

void GameApp::shutdown() {
    spdlog::info("Shutting down GameApp...");
    // Cleanup order handles smart pointers automatically
}