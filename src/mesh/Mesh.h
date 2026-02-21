#pragma once

#include <glad/glad.h>
#include <vector>

namespace vengine {

struct Vertex {
    float position[3];
    float color[3];
    float texCoord[2];
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    
    void build();
    void bind() const;
    void unbind() const;
    void draw() const;
    
    void cleanup();

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    
    bool m_built = false;
};

}
