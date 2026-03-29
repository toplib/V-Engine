#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include <iostream>
#include <vector>

#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfRgba.h>

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
        throw std::runtime_error("Failed to load texture: " + path);
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
        m_loaded = false;
    }

    stbi_image_free(data);
    return m_loaded;
}

bool Texture::loadEXR(const std::string& path) {
    if (m_textureID != 0) {
        cleanup();
    }

    try {
        Imf::RgbaInputFile file(path.c_str());
        Imath::Box2i dw = file.dataWindow();

        m_width    = dw.max.x - dw.min.x + 1;
        m_height   = dw.max.y - dw.min.y + 1;
        m_channels = 4;

        Imf::Array2D<Imf::Rgba> pixels(m_height, m_width);
        file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * m_width, 1, m_width);
        file.readPixels(dw.min.y, dw.max.y);

        std::vector<float> data(m_width * m_height * 4);
        for (int y = 0; y < m_height; ++y) {
            int flippedY = m_height - 1 - y;
            for (int x = 0; x < m_width; ++x) {
                const Imf::Rgba& px = pixels[y][x];
                int idx = (flippedY * m_width + x) * 4;
                data[idx + 0] = px.r;
                data[idx + 1] = px.g;
                data[idx + 2] = px.b;
                data[idx + 3] = px.a;
            }
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        m_hdr    = true;
        m_loaded = true;

    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load texture: " + path + "(" + e.what() + ")");

        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
        }
        m_loaded = false;
    }

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
        m_hdr = false;
    }
}

} // namespace Texture