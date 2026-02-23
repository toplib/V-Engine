#include "OBJParser.h"

namespace Parser {

    OBJParser::OBJParser() = default;
    OBJParser::~OBJParser() = default;

    void OBJParser::source(std::string* source) {
        m_source = *source;
    }

    Mesh::Mesh OBJParser::parse() {
        std::vector<std::string> lines;
        std::istringstream stream(m_source);
        std::string line;
        
        Mesh::Mesh mesh;
        return mesh;
    }

}