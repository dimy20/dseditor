#include "texture.h"
#include <glad/glad.h>
#include "stb_image.h"

std::optional<Texture> load_texture(const std::string& filename){
	stbi_set_flip_vertically_on_load(1);

	int w, h, channels;
	uint8_t * data = stbi_load(filename.c_str(), &w, &h, &channels, 0);
  if(!data) return std::nullopt;

  // generate texture
	uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

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
  bool gl_err = false;
	while((err = glGetError()) != GL_NO_ERROR){
		char errString[128] = {0};
		snprintf(errString, 128, "Error: %d\n", err);
    gl_err = true;
	}

  if(gl_err) return std::nullopt;

  return Texture{id};
};
