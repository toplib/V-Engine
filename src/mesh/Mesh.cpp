#include "Mesh.h"

namespace Mesh {

Mesh::Mesh() = default;

Mesh::~Mesh() {
    cleanup();
}

Mesh::Mesh(const Mesh& other)
    : m_vertices(other.m_vertices), m_indices(other.m_indices) {
    // GPU handles are not copied; the new instance starts unbuilt
}

Mesh& Mesh::operator=(const Mesh& other) {
    if (this == &other) return *this;
    cleanup();
    m_vertices = other.m_vertices;
    m_indices  = other.m_indices;
    // GPU handles reset to defaults by cleanup(); new instance starts unbuilt
    return *this;
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
    m_vertices = vertices;
}

std::vector<Vertex> Mesh::getVertices() {
    return m_vertices;
}


void Mesh::setIndices(const std::vector<unsigned int>& indices) {
    m_indices = indices;
}


void Mesh::build() {
    if (m_built) {
        cleanup();
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);


    // Texture coord attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    // Texture coord attribute (location 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    m_built = true;
}

void Mesh::bind() const {
    glBindVertexArray(VAO);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}


void Mesh::cleanup() {
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    m_built = false;
}

}
