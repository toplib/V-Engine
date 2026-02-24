#include "OBJ2MeshParser.h"

#include <iostream>

#include "glm/vec3.hpp"
#include "utils/StringUtils.h"

namespace Parser {

    OBJ2MeshParser::OBJ2MeshParser() = default;
    OBJ2MeshParser::~OBJ2MeshParser() = default;

    void OBJ2MeshParser::source(std::string* source) {
        m_source = *source;
    }

    Mesh::Mesh OBJ2MeshParser::parse() {
        std::istringstream stream(m_source);
        std::string line;
        bool error = false;

        // Clear vector's
        m_vertices.clear();
        m_vertexTextures.clear();
        m_normals.clear();
        m_indices.clear();
        m_indexes.clear();

        while (std::getline(stream, line))
        {
            Utils::trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // Token parsing ["v/vt/vn/f", "-1.0", "-1.0", "-1.0", "-1.0"]
            std::istringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (ss >> token) {
                tokens.push_back(token);
            }

            // Vertex
            if (tokens.size() < 1) {
                std::cerr << "ERROR: Mesh parsing error, invalid tokens size!" << std::endl;
            }
            if (tokens[0] == "v") {
                if (tokens.size() < 4) {
                    std::cerr << "ERROR: Mesh parsing error, invalid vertex count!" << std::endl;
                    error = true;
                    break;
                }
                Vertex vertex;
                vertex.x = std::stof(tokens[1]);
                vertex.y = std::stof(tokens[2]);
                vertex.z = std::stof(tokens[3]);
                m_vertices.push_back(vertex);
            } else if (tokens[0] == "vt") {
                if (tokens.size() < 3) {
                    std::cerr << "ERROR: Mesh parsing error, invalid vertex texture count!" << std::endl;
                    error = true;
                    break;
                }
                VertexTexture vertexTexture;
                vertexTexture.u = std::stof(tokens[1]);
                vertexTexture.v = std::stof(tokens[2]);
                m_vertexTextures.push_back(vertexTexture);
            }  else if (tokens[0] == "vn") {
                if (tokens.size() < 4) {
                    std::cerr << "ERROR: Mesh parsing error, invalid vertex normal count!" << std::endl;
                    error = true;
                    break;
                }
                Normal normal;
                normal.x = std::stof(tokens[1]);
                normal.y = std::stof(tokens[2]);
                normal.z = std::stof(tokens[3]);
                m_normals.push_back(normal);
            } else if (tokens[0] == "f") {
                std::string v;
                std::string vt;
                std::string vn; // Optional
                std::vector<Index> indexes;

                // Token parsing
                for (int i = 1; i < tokens.size(); i++) {
                    std::istringstream fs(tokens[i]);
                    std::getline(fs, v, '/');
                    std::getline(fs, vt, '/');
                    std::getline(fs, vn, '/');

                    // Indexes
                    int iv = std::stoi(v) - 1;
                    int ivt = vt.empty() ? -1 : std::stoi(vt) -1;
                    int ivn = vn.empty() ? -1 : std::stoi(vn) -1;

                    Index index;
                    index.vertex = m_vertices[iv];
                    index.texture = m_vertexTextures[ivt];
                    Normal normal;
                    index.normal = normal;
                    indexes.push_back(index);
                }
                if (tokens.size() > 4) {
                    m_indexes.push_back(indexes[0]);
                    m_indexes.push_back(indexes[1]);
                    m_indexes.push_back(indexes[2]);

                    m_indexes.push_back(indexes[0]);
                    m_indexes.push_back(indexes[3]);
                    m_indexes.push_back(indexes[2]);
                    continue;
                }

                m_indexes.push_back(indexes[0]);
                m_indexes.push_back(indexes[1]);
                m_indexes.push_back(indexes[2<]);

            }
        }
        Mesh::Mesh mesh;
        build(mesh);
        return mesh;
    }

    void OBJ2MeshParser::build(Mesh::Mesh& mesh) {
        std::vector<Mesh::Vertex> vertices;
        std::vector<unsigned int> indices;
        for (int i = 0; i < m_indexes.size(); i++) {
            Mesh::Vertex vertex;
            vertex.position[0] = m_indexes[i].vertex.x;
            vertex.position[1] = m_indexes[i].vertex.y;
            vertex.position[2] = m_indexes[i].vertex.z;

            vertex.texCoord[0] = m_indexes[i].texture.u;
            vertex.texCoord[1] = m_indexes[i].texture.v;

            vertex.normal[0] = m_indexes[i].normal.x;
            vertex.normal[1] = m_indexes[i].normal.y;
            vertex.normal[2] = m_indexes[i].normal.z;
            vertices.push_back(vertex);
        }
        mesh.setVertices(vertices);
        mesh.setIndices(indices);
    }

}
