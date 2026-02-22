#pragma once
#include <sstream>
#include <string>
#include <vector>
#include "mesh/Mesh.h"

namespace Parser {
    class OBJParser {
    public:
        OBJParser();
        ~OBJParser();

        void source(std::string* source);
        Mesh::Mesh parse();
    private:
        std::string m_source;
        std::vector<Mesh::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
    };
}
