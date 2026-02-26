#pragma once

#include <glad/glad.h>
#include <vector>
#include "glm/gtc/quaternion.hpp"

namespace Mesh {

struct Vertex {
    float position[3];
    float texCoord[2];
    float normal[3];
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    // Copy only CPU data; GPU handles are not shared between copies
    Mesh(const Mesh& other);
    Mesh& operator=(const Mesh& other);

    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);

    std::vector<Vertex> getVertices();
    
    void build();
    void bind() const;
    void unbind() const;

    bool isBuilt() const { return m_built; }

    void cleanup();

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    
    bool m_built = false;
};

} // namespace Mesh
