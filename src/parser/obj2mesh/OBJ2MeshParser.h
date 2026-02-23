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
        struct Vertex {
            float x, y, z;
        };
        struct VertexTexture {
            float u,v;
        };
        struct Normal {
            float x, y, z;
        };
        struct Vertice {
            Vertex vertex;
            VertexTexture texture;
            Normal normal;
        };
        std::string m_source;
        std::vector<Vertex> m_vertices;
        std::vector<VertexTexture> m_vertexTextures;
        std::vector<unsigned int> m_indices;

        void build(Mesh::Mesh& mesh);
    };
}
