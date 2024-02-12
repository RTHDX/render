#include <iostream>
#include <iomanip>

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


void viewport(GLsizei w, GLsizei h) {
    SAFE_CALL(glViewport(0, 0, w, h));
}

void viewport(GLsizei x, GLsizei y, GLsizei w, GLsizei h) {
    SAFE_CALL(glViewport(x, y, w, h));
}

void background(const glm::vec4 color, GLbitfield clear_bits) {
    SAFE_CALL(glClearColor(color.r, color.g, color.b, color.a));
    SAFE_CALL(glClear(clear_bits));
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

std::vector<GLuint> gen_framebuffers(size_t count) {
    std::vector<GLuint> out(count);
    SAFE_CALL(glGenFramebuffers(count, out.data()));
    return out;
}

GLuint gen_framebuffer() {
    GLuint id;
    SAFE_CALL(glGenFramebuffers(1, &id));
    return id;
}

void free_framebuffers(const std::vector<GLuint>& ids) {
    SAFE_CALL(glDeleteFramebuffers(ids.size(), ids.data()));
}

void free_framebuffer(GLuint* id) {
    SAFE_CALL(glDeleteFramebuffers(1, id));
}

glm::u8vec4 read_pixel_color(GLint x, GLint y) {
    GLubyte pixel_data[4];
    SAFE_CALL(glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data));
    return glm::u8vec4{
        pixel_data[0],
        pixel_data[1],
        pixel_data[2],
        pixel_data[3]
    };
}

stencil_idx_t read_stencil(GLint x, GLint y) {
    stencil_idx_t ref = 0;
    SAFE_CALL(glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE,
                           &ref));
    return ref;
}

void bind_vao(GLuint id) {
    SAFE_CALL(glBindVertexArray(id));
}

void bind_fbo(GLuint id) {
    SAFE_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

static std::string gl_enum_to_string(GLenum e) {
#define ENUM_TO_STR(VAL) case VAL: return #VAL

    switch (e) {
    ENUM_TO_STR(GL_DEPTH_COMPONENT);
    ENUM_TO_STR(GL_DEPTH_STENCIL);
    ENUM_TO_STR(GL_RED);
    ENUM_TO_STR(GL_RG);
    ENUM_TO_STR(GL_RGB);
    ENUM_TO_STR(GL_RGBA);
    ENUM_TO_STR(GL_REPEAT);
    ENUM_TO_STR(GL_LINEAR);
    ENUM_TO_STR(GL_NEAREST);
    ENUM_TO_STR(GL_CLAMP_TO_EDGE);
    ENUM_TO_STR(GL_CLAMP_TO_BORDER);
    ENUM_TO_STR(GL_MIRRORED_REPEAT);
    ENUM_TO_STR(GL_TEXTURE_MAG_FILTER);
    ENUM_TO_STR(GL_TEXTURE_MIN_FILTER);
    ENUM_TO_STR(GL_TEXTURE_WRAP_S);
    ENUM_TO_STR(GL_TEXTURE_WRAP_T);
    ENUM_TO_STR(GL_TEXTURE_2D);
    ENUM_TO_STR(GL_TEXTURE_CUBE_MAP);
    ENUM_TO_STR(GL_TEXTURE_3D);
    ENUM_TO_STR(GL_TEXTURE_2D_ARRAY);
    ENUM_TO_STR(GL_FRAMEBUFFER);
    ENUM_TO_STR(GL_RENDERBUFFER);
    ENUM_TO_STR(GL_DEPTH_BUFFER_BIT);
    ENUM_TO_STR(GL_COLOR_BUFFER_BIT);
    ENUM_TO_STR(GL_STENCIL_BUFFER_BIT);
    ENUM_TO_STR(GL_UNSIGNED_BYTE);
    ENUM_TO_STR(GL_FLOAT);
    ENUM_TO_STR(GL_STATIC_DRAW);
    ENUM_TO_STR(GL_DYNAMIC_DRAW);
    ENUM_TO_STR(GL_STREAM_DRAW);
    ENUM_TO_STR(GL_FLOAT_VEC2);
    ENUM_TO_STR(GL_FLOAT_VEC3);
    ENUM_TO_STR(GL_FLOAT_VEC4);
    ENUM_TO_STR(GL_INT);
    ENUM_TO_STR(GL_INT_VEC2);
    ENUM_TO_STR(GL_INT_VEC3);
    ENUM_TO_STR(GL_INT_VEC4);
    ENUM_TO_STR(GL_UNSIGNED_INT);
    ENUM_TO_STR(GL_BOOL);
    ENUM_TO_STR(GL_BOOL_VEC2);
    ENUM_TO_STR(GL_BOOL_VEC3);
    ENUM_TO_STR(GL_BOOL_VEC4);
    ENUM_TO_STR(GL_FLOAT_MAT2);
    ENUM_TO_STR(GL_FLOAT_MAT3);
    ENUM_TO_STR(GL_FLOAT_MAT4);
    ENUM_TO_STR(GL_SAMPLER_2D);
    ENUM_TO_STR(GL_SAMPLER_2D_ARRAY);
    ENUM_TO_STR(GL_SAMPLER_CUBE);
    ENUM_TO_STR(GL_FRAMEBUFFER_COMPLETE);
    ENUM_TO_STR(GL_FRAMEBUFFER_UNDEFINED);
    ENUM_TO_STR(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
    ENUM_TO_STR(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
    ENUM_TO_STR(GL_FRAMEBUFFER_UNSUPPORTED);
    default:
        return "Unknown GLenum value";
    }

#undef ENUM_TO_STR
}

GLuint gen_texture(GLenum target) {
    GLuint tex;
    SAFE_CALL(glGenTextures(1, &tex));
    SAFE_CALL(glBindTexture(target, tex));
    return tex;
}


void dump_image_part(byte_t* data, GLsizei stride) {
    for (GLsizei i = 0; i < stride; ++i) {
        std::cout << std::setw(4) << int(*(data + i)) << ", ";
    }
}

void activate_texture(const TextureActivationCommand& cmd) {
    SAFE_CALL(glActiveTexture(cmd.tex_unit));
    SAFE_CALL(glBindTexture(cmd.sampler_type, cmd.id));
    set_int(cmd.program, cmd.sampler_name, 0);
}


void free_texture(GLuint id) {
    SAFE_CALL(glDeleteTextures(1, &id));
}

void apply_stencil(StencilCommand&& cmd) {
    SAFE_CALL(glStencilOp(cmd.s_fail, cmd.dp_fail, cmd.dp_pass));
    SAFE_CALL(glStencilFunc(cmd.function, cmd.ref, cmd.mask));
}

void use(GLuint id) {
    SAFE_CALL(glUseProgram(id));
}


void draw(const DrawArrayCommand& cmd) {
    assert(cmd.vao != 0);
    assert(opengl::Context::instance().active_program() != 0);
    bind_vao(cmd.vao);
    SAFE_CALL(glDrawArrays(cmd.mode, cmd.first, cmd.count));
    bind_vao(0);
}

void draw(const DrawElementsCommand& cmd) {
    assert(cmd.vao != 0);
    assert(opengl::Context::instance().active_program() != 0);

    bind_vao(cmd.vao);
    SAFE_CALL(glDrawElements(cmd.mode, cmd.count, cmd.type, cmd.indices));
    bind_vao(0);
}

void draw_array_framebuffer(const DrawArrayFramebuffer& cmd) {
    SAFE_CALL(glBindFramebuffer(GL_FRAMEBUFFER, cmd.fbo));
    SAFE_CALL(glClearColor(cmd.background.r, cmd.background.g,
                           cmd.background.b, cmd.background.a));
    SAFE_CALL(glClear(cmd.clear_bits));
    bind_vao(cmd.vao);
    const auto& fbuff_v = cmd.viewport;
    SAFE_CALL(glViewport(fbuff_v.x, fbuff_v.y, fbuff_v.z, fbuff_v.w));
    SAFE_CALL(glDrawArrays(cmd.mode, cmd.first, cmd.count));
    bind_vao(0);
    SAFE_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    const auto& screen_v = cmd.screen_viewport;
    SAFE_CALL(glViewport(screen_v.x, screen_v.y, screen_v.z, screen_v.w));
}

void draw_instance_array(const DrawArrayInstanced& cmd) {
    assert(cmd.vao != 0);
    assert(opengl::Context::instance().active_program() != 0);

    bind_vao(cmd.vao);
    SAFE_CALL(glDrawArraysInstanced(cmd.mode, cmd.first, cmd.count,
                                    cmd.instancecount));
    bind_vao(0);
}

void draw_instance_elements(const DrawElementInstanced& cmd) {
    assert(cmd.vao != 0);
    assert(opengl::Context::instance().active_program() != 0);

    bind_vao(cmd.vao);
    SAFE_CALL(glDrawElementsInstanced(cmd.mode, cmd.count, cmd.type,
                                      cmd.indices, cmd.instancecount));
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

bool set_vec2(GLuint id, const std::string_view name, glm::vec2& val) {
    assert(Context::instance().active_program() > 0);
    auto loc = find_location(id, name);
    if (loc < 0) return false;
    SAFE_CALL(glUniform2f(loc, val.x, val.y));
    return true;
}

bool set_int(GLuint id, const std::string_view name, GLint value) {
    assert(Context::instance().active_program() > 0);
    auto loc = find_location(id, name);
    if (loc < 0) return false;
    SAFE_CALL(glUniform1i(loc, value));
    return true;
}

bool set_float(GLuint id, const std::string_view name, GLfloat value) {
    assert(Context::instance().active_program() > 0);
    auto loc = find_location(id, name);
    if (loc < 0) return false;
    SAFE_CALL(glUniform1f(loc, value));
    return true;
}

static std::string type_to_string(GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return "float";
    case GL_FLOAT_VEC2:
        return "vec2";
    case GL_FLOAT_VEC3:
        return "vec3";
    case GL_FLOAT_VEC4:
        return "vec4";
    case GL_INT:
        return "int";
    case GL_INT_VEC2:
        return "ivec2";
    case GL_INT_VEC3:
        return "ivec3";
    case GL_INT_VEC4:
        return "ivec4";
    case GL_UNSIGNED_INT:
        return "unsigned int";
    case GL_BOOL:
        return "bool";
    case GL_BOOL_VEC2:
        return "bvec2";
    case GL_BOOL_VEC3:
        return "bvec3";
    case GL_BOOL_VEC4:
        return "bvec4";
    case GL_FLOAT_MAT2:
        return "mat2";
    case GL_FLOAT_MAT3:
        return "mat3";
    case GL_FLOAT_MAT4:
        return "mat4";
    case GL_SAMPLER_2D:
        return "sampler2D";
    case GL_SAMPLER_2D_ARRAY:
        return "sampler2DArray";
    case GL_SAMPLER_CUBE:
        return "samplerCube";
    default:
        return "unknown type";
    }
}

std::ostream& operator << (std::ostream& os, const ShaderProgramInterface& i) {
    constexpr std::string_view ROW_FORMAT = "|{:<20}|{:<20}|";
    constexpr std::string_view DELIM_FORMAT = "|{:-^20}|{:-^20}|";

    os << "Input attributes amount: " << i.input_count << std::endl;
    os << std::format(ROW_FORMAT, "Name", "Type") << std::endl;
    os << std::format(DELIM_FORMAT, "", "") << std::endl;
    for (const auto& [name, type] : i.input_block) {
        os << std::format(ROW_FORMAT, name, type_to_string(type)) << std::endl;
    } os << std::endl;

    os << "Uniform variables amount: " << i.uniforms_count << std::endl;
    os << std::format(ROW_FORMAT, "Name", "Type") << std::endl;
    os << std::format(DELIM_FORMAT, "", "") << std::endl;
    for (const auto& [name, type] : i.uniform_block) {
        os << std::format(ROW_FORMAT, name, type_to_string(type)) << std::endl;
    } os << std::endl;
    return os;
}

ShaderProgramInterface get_program_interface(GLuint program) {
    ShaderProgramInterface intf;
    // Get number of active uniforms in program
    SAFE_CALL(glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                                      &intf.uniforms_count));

    // Loop through all active uniforms and print their names and types
    for (GLint i = 0; i < intf.uniforms_count; i++) {
        GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE};
        GLint values[2];
        SAFE_CALL(glGetProgramResourceiv(program, GL_UNIFORM, i, 2, properties,
                                         2, NULL, values));

        GLint len = values[0];
        GLint type = values[1];

        char* name = new char[len];
        SAFE_CALL(glGetProgramResourceName(program, GL_UNIFORM, i, len, NULL,
                                           name));
        intf.uniform_block.insert({std::string(name), type});
        delete[] name;
    }

    // Get number of active attributes in program
    SAFE_CALL(glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT,
                                      GL_ACTIVE_RESOURCES, &intf.input_count));

    // Loop through all active attributes and print their names and types
    for (GLint i = 0; i < intf.input_count; i++) {
        GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE };
        GLint values[2];
        SAFE_CALL(glGetProgramResourceiv(program, GL_PROGRAM_INPUT, i, 2,
                                         properties, 2, NULL, values));

        GLint len = values[0];
        GLint type = values[1];

        char* name = new char[len];
        SAFE_CALL(glGetProgramResourceName(program, GL_PROGRAM_INPUT, i, len,
                                           NULL, name));
        intf.input_block.insert({std::string(name), type});
        delete[] name;
    }

    return intf;
}

std::optional<std::string>
ShaderProgramInterface::found_uname_by_value(GLenum v) const {
    for (const auto& [key, val] : uniform_block) {
        if (val == v) { return key; }
    }
    return std::nullopt;
}

std::optional<std::string>
ShaderProgramInterface::found_iname_by_balue(GLenum v) const {
    for (const auto& [key, val] : input_block) {
        if (val == v) { return key; }
    }
    return std::nullopt;
}

}
