# 5CM507 Graphics Labs University of Derby 26/27

## Example Folder Structure
```
cg27Labs/
├── CMakeLists.txt
└── apps/                    # our tutorial labs
│   ├── CMakeLists.txt
│   └── TA00/                # Demo lab folder
│   │    ├── CMakeLists.txt
│   │    └── main.cpp
│   └── ... 
├── assets/                  # 3D models, textures
├── engine/                  # a mini game engine for teaching
│   ├── CMakeLists.txt
│   ├── GameApp.cpp          # The top-level Game app class
│   └── device/              # low-leve render-hardware interface (rhi)
│   │   ├── GPUPipeline.cpp  # GLSL Pipeline/program
│   │   └── Shader.cpp       # GLSL shader file loader
│   └── include/             # game engine header files
│       ├── device/
│       │  ├── GPUPipeline.h
│       │  └── Shader.h  
│       ├── entity/          # Combined Entity-Component-System (ECS) architecture and component data structs
│       │  ├── ECSWorldRegistry.h
│       │  └── TransformComp.h
│       ├── render/          # high-level rendering related classes
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