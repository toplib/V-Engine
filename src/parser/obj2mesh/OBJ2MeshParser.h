#pragma once
#include <sstream>
#include <string>
#include <vector>
#include "mesh/Mesh.h"

namespace Parser {
    class OBJ2MeshParser {
    public:
        OBJ2MeshParser();
        ~OBJ2MeshParser();

        void source(std::string* source);
        Mesh::Mesh parse();
    private:
        std::string m_source;
        std::vector<Mesh::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
    };
}
