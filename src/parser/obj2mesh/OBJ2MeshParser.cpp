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
        while (std::getline(stream, line)) {
            Utils::trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (ss >> token) {
                tokens.push_back(token);
            }

            if (tokens.size() < 1) {
                std::cerr << "ERROR: Mesh parsing error, invalid tokens size!" << std::endl;
                continue;
            }

            // Vertex
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
            }
            // Vertex Texture
            else if (tokens[0] == "vt") {
                if (tokens.size() < 3) {
                    std::cerr << "ERROR: Mesh parsing error, invalid vertex texture count!" << std::endl;
                    error = true;
                    break;
                }
                VertexTexture vertexTexture;
                vertexTexture.u = std::stof(tokens[1]);
                vertexTexture.v = std::stof(tokens[2]);
                m_vertexTextures.push_back(vertexTexture);
            }
            // Face
            else if (tokens[0] == "f") {
                if (tokens.size() < 4) {
                    std::cerr << "ERROR: Mesh parsing error, invalid face count!" << std::endl;
                    error = true;
                    break;
                }
                // Parse faces
                for (size_t i = 1; i < tokens.size(); ++i) {
                    std::vector<std::string> indices = Utils::split(tokens[i], '/');
                    if (indices.size() < 1) {
                        std::cerr << "ERROR: Mesh parsing error, invalid face index format!" << std::endl;
                        error = true;
                        break;
                    }
                    unsigned int vertexIndex = std::stoul(indices[0]) - 1;
                    m_indices.push_back(vertexIndex);
                }
            }
        }

        Mesh::Mesh mesh;
        if (!error) {
            build(mesh);
        }
        return mesh;
    }


    void OBJ2MeshParser::build(Mesh::Mesh& mesh) {

    }

}
