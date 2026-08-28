#pragma once

#include "GameApp.h"

class MyGameApp : GameApp {

public:

    MyGameApp(std:string name):GameApp(name) {

    }

    void init() {
        std::cout << "Hello, Graphics!" << std::endl; 

        spdlog::set_level(spdlog::level::debug);

        
    }
};