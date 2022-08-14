#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "opengl_utils.hpp"
#include "opengl_proc.hpp"


namespace opengl {

bool check_shader(GLuint id) {
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        static constexpr size_t BUFF_SIZE = 256;
        char msg[BUFF_SIZE];
        glGetShaderInfoLog(id, BUFF_SIZE, nullptr, msg);
        std::cout << "Shader compiled with error: " << msg << std::endl;
        return false;
    }
    return true;
}

bool check_program(GLuint id) {
    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        constexpr size_t BUFF_SIZE = 512;
        char msg[BUFF_SIZE];
        glGetProgramInfoLog(id, BUFF_SIZE, nullptr, msg);
        std::cerr << "Program linking failed with: " << msg << std::endl;
        return false;
    }
    return true;
}


GLuint create_program(const std::string& vertex_shader_src,
                      const std::string& fragment_shader_src) {
    GLuint program = glCreateProgram(),
           vertex_shader = glCreateShader(GL_VERTEX_SHADER),
           fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const auto* src = vertex_shader_src.data();
    SAFE_CALL(glShaderSource(vertex_shader, 1, &src, nullptr));
    SAFE_CALL(glCompileShader(vertex_shader));
    if (!check_shader(vertex_shader)) {
        SAFE_CALL(glDeleteShader(vertex_shader));
    }

    src = fragment_shader_src.data();
    SAFE_CALL(glShaderSource(fragment_shader, 1, &src, nullptr));
    SAFE_CALL(glCompileShader(fragment_shader));
    if (!check_shader(fragment_shader)) {
        SAFE_CALL(glDeleteShader(fragment_shader));
    }

    SAFE_CALL(glAttachShader(program, vertex_shader));
    SAFE_CALL(glAttachShader(program, fragment_shader));
    SAFE_CALL(glLinkProgram(program));
    if (!check_program(program)) {
        SAFE_CALL(glDeleteProgram(program));
    }
    SAFE_CALL(glDetachShader(program, vertex_shader));
    SAFE_CALL(glDetachShader(program, fragment_shader));
    return program;
}

GLuint create_program(const std::filesystem::path& vertex_path,
                      const std::filesystem::path& fragment_path) {
    auto vertex_src = opengl::utils::read_shader(vertex_path);
    auto fragment_src = opengl::utils::read_shader(fragment_path);
    return create_program(vertex_src, fragment_src);
}

void free_program(GLuint id) {
    SAFE_CALL(glDeleteProgram(id));
}

void on_gl_error(GLenum error_code, const char* call, const char* file,
                 int line) {
    if (error_code == GL_NO_ERROR) return;

    std::cerr << call << " -> ";
    switch (error_code) {
    case GL_INVALID_ENUM:
        std::cerr << "INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        std::cerr << "INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        std::cerr << "INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cerr << "INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        std::cerr << "OUT_OF_MEMORY";
        break;
    case GL_STACK_UNDERFLOW:
        std::cerr << "STACK_UNDERFLOW";
        break;
    case GL_STACK_OVERFLOW:
        std::cerr << "STACK_OVERFLOW";
        break;
    }
    std::cerr << "[" << file << ":" << line << "]" << std::endl;
}


std::vector<GLuint> gen_vertex_array(size_t count) {
    std::vector<GLuint> out(count);
    SAFE_CALL(glGenVertexArrays(count, out.data()));
    return out;
}

void free_vertex_array(const std::vector<GLuint>& in) {
    SAFE_CALL(glDeleteVertexArrays(in.size(), in.data()));
}

GLuint gen_vertex_array() {
    GLuint id;
    SAFE_CALL(glGenVertexArrays(1, &id));
    return id;
}

void free_vertex_array(GLuint id) {
    SAFE_CALL(glDeleteVertexArrays(1, &id));
}


std::vector<GLuint> gen_element_buffers(size_t count) {
    std::vector<GLuint> out(count);
    SAFE_CALL(glGenBuffers(count, out.data()));
    return out;
}

GLuint gen_element_buffer() {
    GLuint out;
    SAFE_CALL(glGenBuffers(1, &out));
    return out;
}

void free_element_buffers(std::vector<GLuint>& in) {
    SAFE_CALL(glDeleteBuffers(in.size(), in.data()));
    in.clear();
}

void free_element_buffer(GLuint id) {
    SAFE_CALL(glDeleteBuffers(1, &id));
}


std::vector<GLuint> gen_vertex_buffers(size_t count) {
    std::vector<GLuint> out(count);
    SAFE_CALL(glGenBuffers(count, out.data()));
    return out;
}

GLuint gen_vertex_buffers() {
    GLuint out;
    SAFE_CALL(glGenBuffers(1, &out));
    return out;
}

void free_vertex_buffers(const std::vector<GLuint>& in) {
    SAFE_CALL(glDeleteBuffers(in.size(), in.data()));
}

void free_vertex_buffer(GLuint id) {
    SAFE_CALL(glDeleteBuffers(1, &id));
}


std::vector<GLuint> gen_pixel_buffers(size_t count) {
    std::vector<GLuint> out(count);
    SAFE_CALL(glGenBuffers(count, out.data()));
    return out;
}

GLuint gen_pixel_buffers() {
    GLuint id;
    SAFE_CALL(glGenBuffers(1, &id));
    return id;
}

void free_pixel_buffers(const std::vector<GLuint>& in) {
    SAFE_CALL(glDeleteVertexArrays(in.size(), in.data()));
}

void free_pixel_buffer(GLuint id) {
    SAFE_CALL(glDeleteVertexArrays(1, &id));
}


void bind_vao(GLuint id) {
    SAFE_CALL(glBindVertexArray(id));
}


GLuint gen_texture() {
    GLuint tex;
    SAFE_CALL(glGenTextures(1, &tex));
    SAFE_CALL(glBindTexture(GL_TEXTURE_2D, tex));
    //SAFE_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    //SAFE_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    //SAFE_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //                          GL_LINEAR_MIPMAP_LINEAR));
    //SAFE_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    return tex;
}

void bind_texture(const glm::ivec2& dims, // x - width, y - height
                  std::vector<glm::vec3>&& texture) {
    assert(Context::instance().bound_texture_2d() > 0);
    SAFE_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dims.x, dims.y, 0, GL_RGB,
                           GL_FLOAT, glm::value_ptr(*texture.data())));
    SAFE_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void bind_texture(const glm::ivec2& dims, byte_t* texture) {
    assert(Context::instance().bound_texture_2d() > 0);
    SAFE_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dims.x, dims.y, 0, GL_RGB,
                           GL_UNSIGNED_BYTE, texture));
    SAFE_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void activate_texture(GLuint id) {
    SAFE_CALL(glBindTexture(GL_TEXTURE_2D, id));
    SAFE_CALL(glActiveTexture(GL_TEXTURE0));
}

void free_texture(GLuint id) {
    SAFE_CALL(glDeleteTextures(1, &id));
}

void use(GLuint id) {
    SAFE_CALL(glUseProgram(id));
}


void draw(DrawArrayCommand&& cmd) {
    assert(cmd.vao != 0);
    assert(opengl::Context::instance().active_program() != 0);
    bind_vao(cmd.vao);
    SAFE_CALL(glDrawArrays(cmd.mode, cmd.first, cmd.count));
    bind_vao(0);
}

void draw(DrawElementsCommand&& cmd) {
    assert(cmd.vao != 0);
    assert(opengl::Context::instance().active_program() != 0);

    bind_vao(cmd.vao);
    SAFE_CALL(glDrawElements(cmd.mode, cmd.count, cmd.type, cmd.indices));
    bind_vao(0);
}

static int find_location(GLuint id, const std::string_view name) {
    auto location = glGetUniformLocation(id, name.data());
    if (location < 0) {
        std::cerr << "Could not find uniform " << name << std::endl;
    }
    return location;
}

bool set_vec3(GLuint id, const std::string_view name, const glm::vec3& val) {
    assert(Context::instance().active_program() > 0);

    auto loc = find_location(id, name);
    if (loc < 0) return false;
    glUniform3f(loc, val.x, val.y, val.z);
    return true;

}

bool set_vec4(GLuint id, const std::string_view name, const glm::vec4& val) {
    assert(Context::instance().active_program() > 0);

    auto loc = find_location(id, name);
    if (loc < 0) return false;
    glUniform4f(loc, val.r, val.g, val.b, val.a);
    return true;
}

bool set_mat4(GLuint id, const std::string_view name, const glm::mat4& val) {
    assert(Context::instance().active_program() > 0);

    auto loc = find_location(id, name);
    if (loc < 0) return false;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
    return true;
}

}
