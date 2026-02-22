#pragma once

#include <glad/glad.h>
#include <string>

namespace Texture {

class Texture {
public:
    Texture();
    ~Texture();

    bool load(const std::string& path);
    void bind(unsigned int slot = 0) const;
    void unbind() const;
    
    void setWrapMode(GLenum wrapS, GLenum wrapT);
    void setFilterMode(GLenum minFilter, GLenum magFilter);
    
    void cleanup();
    
    unsigned int getID() const { return m_textureID; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    bool isLoaded() const { return m_loaded; }

private:
    unsigned int m_textureID = 0;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
    bool m_loaded = false;
    
    GLenum m_wrapS = GL_MIRRORED_REPEAT;
    GLenum m_wrapT = GL_MIRRORED_REPEAT;
    GLenum m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum m_magFilter = GL_LINEAR;
};

} // namespace Texture
