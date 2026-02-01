# V-Engine

A 3D game engine built with OpenGL 4.6, C++17, and modern graphics libraries.

## Project Structure

```
V-Engine/
├── src/                    # All engine source code (C/CPP/H/HPP)
│   └── main.cpp           # Main entry point
├── include/               # External libraries
│   ├── glad/             # OpenGL loader (GLAD)
│   │   ├── include/
│   │   │   ├── glad/
│   │   │   └── KHR/
│   │   └── src/
│   ├── glfw/             # GLFW - Window and input management
│   └── glm/              # GLM - OpenGL Mathematics library
├── build/                # Build output directory
└── CMakeLists.txt        # CMake build configuration
```

## Dependencies

- **GLAD**: OpenGL 4.6 Core Profile loader
- **GLFW**: Cross-platform window and input library
- **GLM**: Header-only C++ mathematics library for graphics

## Requirements

- CMake 3.23 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- OpenGL 4.6 compatible GPU and drivers

## Building the Project

### Linux/macOS

```bash
# Navigate to build directory
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .

# Run the engine
./bin/V-Engine
```

### Windows

```bash
# Navigate to build directory
cd build

# Generate build files (Visual Studio)
cmake ..

# Build the project
cmake --build . --config Release

# Run the engine
bin\Release\V-Engine.exe
```

## Features

- OpenGL 4.6 Core Profile rendering
- GLFW window management
- GLM mathematics library integration
- Basic render loop with depth testing
- Input handling (ESC to close)

## Development

The engine uses modern CMake practices:
- Separate include and source directories
- Static linking of GLAD
- Proper library organization
- Platform-specific OpenGL linking

## License

See individual library licenses:
- GLAD: Public Domain / MIT
- GLFW: zlib/libpng
- GLM: MIT License
