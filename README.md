# 5CM507 Graphics Labs University of Derby 26/27

## Functionalities (to be implemented)
### Forward Rendering (TA01-TA05)
### Deferred Rendering (TA06)
### Environmental Lighting (TA07)
### Screen-space Lighting (TA08)
### Proedural Generation (TA09)
### Particle Systems (TA10)
### Atomospheric Effects (TA11)
### Post-processing (TA12)
### Ray Tracing (TB01-TB04)
### Animation (TB05-TB06)
### Physical Simulation (TB07-TB18)
### Advanced Rendering (TB09-TB12)



## Project Folder Structure
```
cg27Labs/
├── CMakeLists.txt
└── apps/                    # our tutorial labs
│   ├── CMakeLists.txt
│   └── README.md            # Lab tutorial topics  
│   └── TA00/                # Demo lab folder
│   │    ├── CMakeLists.txt
│   │    └── main.cpp
│   └── ... 
├── assets/                  # 3D models, textures
├── engine/                  # a mini game engine for teaching
│   ├── CMakeLists.txt
│   ├── GameApp.cpp          # The top-level Game app class
│   └── device/              # low-level render-hardware interface (rhi)
│   │   ├── GPUPipeline.cpp  # GLSL Pipeline/program
│   │   └── Shader.cpp       # GLSL shader file loader
│   └── include/             # game engine header files
│       ├── device/
│       │  ├── GPUPipeline.h
│       │  └── Shader.h  
│       ├── entity/          # Combined Entity-Component-System (ECS) architecture and component data structs
│       │  ├── ECSWorldRegistry.h
│       │  └── TransformComp.h
│       ├── render/          # high-level rendering classes
│       │  ├── Material.h
│       │  └── Texture.h
│       ├── renderpasses/    # middle-level render pipelines and render passes
│       │  ├── RenderPipeline.h
│       │  ├── RenderPass.h
│       │  ├── ForwardPass.h
│       │  └── GBufferPass.h
│       └── scene/           # scene and mesh related classess
│       │   ├── MeshModel.h
│       │   ├── MeshNode.h
│       │   └── Mesh.h       # mesh geometry consisting vertices and indices
│       └── systems/         # classes to update (tick()) the game and scenestates
│       │   ├── CameraSystem.h
│       │   └── PhysicalSystem.h  
│       ├── ui/              # Window management, inputs, and engine context
│       │   ├── InputState.h
│       │   └── Window.h
│       └── GameApp.h
└── shaders/                 # GLSL shader source files   
```