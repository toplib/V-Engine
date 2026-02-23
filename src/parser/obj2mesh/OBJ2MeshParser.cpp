#include "OBJ2MeshParser.h"

namespace Parser {

    OBJ2MeshParser::OBJ2MeshParser() = default;
    OBJ2MeshParser::~OBJ2MeshParser() = default;

    void OBJ2MeshParser::source(std::string* source) {
        m_source = *source;
    }

    Mesh::Mesh OBJ2MeshParser::parse() {
        std::vector<std::string> lines;
        std::istringstream stream(m_source);
        std::string line;
        
        Mesh::Mesh mesh;
        return mesh;
    }

}