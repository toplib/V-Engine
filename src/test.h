#pragma once

#include <cstring>
#include <fstream>

char* loadShaderFromPath(const char* path){
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        return nullptr;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];

    if (!file.read(buffer, size)) {
        delete[] buffer;
        return nullptr;
    }

    buffer[size] = '\0';   // to C str
    return buffer;
}

char* loadVertexShader(){
    return loadShaderFromPath("/home/toplib/V-Engine/vertex.glsl");
}

char* loadFragmentShader(){
    return loadShaderFromPath("/home/toplib/V-Engine/fragment.glsl");
}
