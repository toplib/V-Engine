- [ ] Implement assets system and make paths local
  - [ ] Create resource path resolver (relative to executable)
  - [ ] Replace hardcoded paths in main.cpp (lines 31, 50, 59-60)
  - [ ] Update parser and texture loaders to use asset system

- [x] Implement OBJ2Mesh parser
  - [ ] Fix normals parsing bug - normals not loaded from OBJ vn tokens
  - [ ] Add proper polygon triangulation for n-gons (>4 vertices)

- [ ] Implement simple lighting
  - [ ] Update vertex shader to include normal attributes
  - [ ] Update fragment shader with basic diffuse/specular lighting
  - [ ] Pass light uniforms (position, color, intensity)
  - [ ] Calculate lighting in fragment shader

- [ ] Separate object drawing from Mesh to Renderer
  - [ ] Move manual draw calls from main.cpp to Renderer class
  - [ ] Use Scene::getGameObjects() in render loop
  - [ ] Apply transform matrices from GameObject

- [ ] Make logger *(in progress)*
  - [ ] Replace all std::cerr/cout with Logger in main.cpp
  - [ ] Add Logger to Window class for initialization errors
  - [ ] Add Logger to Shader compilation/linking
  - [ ] Add Logger to OBJ2MeshParser for parsing errors

- [ ] Add polygon triangulating for OBJ2Mesh parser
  - [ ] Handle n-gons (5+ vertices) with proper ear clipping
  - [ ] Currently only quads (4 vertices) are triangulated

- [ ] Refactor camera controls
  - [ ] Move WASD movement from main.cpp to Camera class
  - [ ] Add update(float deltaTime) method to Camera
  - [ ] Add mouse look support

- [ ] Fix RAII issues
  - [ ] GameObject::getMeshRenderer() - return by const reference
  - [ ] MeshRenderer::getMesh() - return by const reference
  - [ ] Scene::getGameObjects() - return by const reference

- [ ] Cleanup Scene class
  - [ ] Remove duplicate getGameObjects() method
  - [ ] Keep only reference-returning version

- [ ] Add component lifecycle system
  - [ ] Add virtual Update() method to components
  - [ ] Add Start() lifecycle method
  - [ ] Create main game loop with deltaTime
