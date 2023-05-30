#pragma once
#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

struct Shader{
	void use() const { glUseProgram(id); }
	inline void set_int(const std::string& name, int value) const{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
	inline void set_float(const std::string& name, float value) const{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}
	inline void set_bool(const std::string& name, bool value) const{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	inline void set_mat4(const std::string& name, const glm::mat4& m){
		uint32_t loc = glGetUniformLocation(id, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
	}

  uint32_t id;
};

std::optional<Shader> load_shader(const std::string& vshader_path,
                                  const std::string& fshader_path);
