#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include <iostream>

namespace Texture {

Texture::Texture() = default;

Texture::~Texture() {
    cleanup();
}

bool Texture::load(const std::string& path) {
    if (m_textureID != 0) {
        cleanup();
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (data) {
        GLenum format = (m_channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(format), m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        m_loaded = true;
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        m_loaded = false;
    }
    
    stbi_image_free(data);
    return m_loaded;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrapMode(GLenum wrapS, GLenum wrapT) {
    m_wrapS = wrapS;
    m_wrapT = wrapT;
    
    if (m_textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    }
}

void Texture::setFilterMode(GLenum minFilter, GLenum magFilter) {
    m_minFilter = minFilter;
    m_magFilter = magFilter;
    
    if (m_textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
    }
}

void Texture::cleanup() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
        m_loaded = false;
    }
}

}
