#include <glad/glad.h>
#include <cassert>
#include <iostream>

#include "texture.h"
#include "stb_image.h"

bool load_texture_make_id(const char *filename, uint32_t *id){
    assert(id != NULL);
    stbi_set_flip_vertically_on_load(1);

    int w, h, channels;
    uint8_t * data = stbi_load(filename, &w, &h, &channels, 0);
    if(!data) return false;

    // generate texture
    uint32_t _id;
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        std::cerr << "Gl error: " << err << "\n";
        ok = false;
    }

    *id = _id;
    return ok;
};
