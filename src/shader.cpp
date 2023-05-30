#include <fstream>
#include <optional>
#include <string>
#include <glad/glad.h>
#include "shader.h"
#include "utils.h"

static bool compile_shader(uint32_t shader_id, const char * shader_src){
    glShaderSource(shader_id, 1, &shader_src, NULL);
    glCompileShader(shader_id);

    int ok;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &ok);

    if(!ok){
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
    }

    return ok;
};

std::optional<std::string> read_file_to_string(const std::string& filename){
    std::ifstream f(filename);

    if(!f.is_open()){
        return std::nullopt;
    }

    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    f.seekg(0, std::ios::beg);

    std::string buffer(size, 0);
    f.read(&buffer[0], size);

    return buffer;
};

std::optional<Shader> load_shader(const std::string& vertex_shader_path,
                                  const std::string& frag_shader_path){

    auto vertex_src = read_file_to_string(vertex_shader_path);
    auto frag_src = read_file_to_string(frag_shader_path);

    if((!vertex_src.has_value() || !frag_src.has_value())) return std::nullopt;

    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if(!compile_shader(vertex_shader, vertex_src->c_str()) ||
       !compile_shader(frag_shader, frag_src->c_str())){
        return std::nullopt;
    }

    uint32_t shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);  

    char info_log[512];
    int ok;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &ok);

    if(!ok) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
        return std::nullopt;
    }

    return Shader{shader_program};
};

