#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "opengl_utils.hpp"
#include "opengl_proc.hpp"
#include "opengl_functions.hpp"


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
    switch (e) {
    case GL_DEPTH_COMPONENT:
        return "GL_DEPTH_COMPONENT";
    case GL_DEPTH_STENCIL:
        return "GL_DEPTH_STENCIL";
    case GL_RED:
        return "GL_RED";
    case GL_RG:
        return "GL_RG";
    case GL_RGB:
        return "GL_RGB";
    case GL_RGBA:
        return "GL_RGBA";
    case GL_REPEAT:
        return "GL_REPEAT";
    case GL_LINEAR:
        return "GL_LINEAR";
        // ... add cases for other GLenum values as needed
    default:
        return "Unknown GLenum value";
    }
}


GLenum FramebufferData::status() const {
    GLenum res = glCheckNamedFramebufferStatus(fbo, target);
    return res;
}

std::ostream& operator << (std::ostream& os, const FramebufferData& f) {
    auto status = f.status();
    os << "Framebuffer status: ";
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
        os << "GL_FRAMEBUFFER_COMPLETE";
        break;
    case GL_FRAMEBUFFER_UNDEFINED:
        os << "GL_FRAMEBUFFER_UNDEFINED";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        os << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        os << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        os << "GL_FRAMEBUFFER_UNSUPPORTED";
        break;
        // Add cases for other status codes as needed.
    default:
        os << "Unknown framebuffer status: " << status;
        break;
    }
    return os;
}

void attach_texture(const FramebufferData& fbuff, const TextureData& tex) {
    SAFE_CALL(glBindFramebuffer(fbuff.target, fbuff.fbo))
    SAFE_CALL(glFramebufferTexture2D(fbuff.target, fbuff.attachment_point,
                                     tex.target, tex.id, 0));
    SAFE_CALL(glBindFramebuffer(fbuff.target, 0));
}


void TextureData::free() {
    if (id != 0 && Context::instance().is_context_active()) {
        SAFE_CALL(glBindTexture(target, 0));
        SAFE_CALL(glDeleteTextures(1, &id));
        id = 0;
    }
}

bool TextureData::is_valid() const {
    return id != 0;
}


GLsizei TextureDataArray2D::tile_w() const {
    return tex_data.w / tile_count_w;
}

GLsizei TextureDataArray2D::tile_h() const {
    return tex_data.h / tile_count_h;
}

GLsizei TextureDataArray2D::total_tiles() const {
    return tile_count_h * tile_count_w;
}

bool TextureDataArray2D::is_valid() const {
    return tex_data.is_valid() && tile_count_h != 0 && tile_count_w != 0;
}


std::ostream& operator<<(std::ostream& os, const TextureData& tex) {
    os << "| Property     | Value                       |\n"
       << "|--------------|-----------------------------|\n"
       << std::format("| Target       | {}                           |\n",
          gl_enum_to_string(tex.target))
       << std::format("| Width        | {:<28}|\n", tex.w)
       << std::format("| Height       | {:<28}|\n", tex.h)
       << std::format("| Format       | {}                           |\n",
           gl_enum_to_string(tex.format))
       << std::format("| Type         | {}                           |\n",
           gl_enum_to_string(tex.type))
       << std::format("| Wrap S       | {}                           |\n",
           gl_enum_to_string(tex.wrap_s))
       << std::format("| Wrap T       | {}                           |\n",
           gl_enum_to_string(tex.wrap_t))
       << std::format("| Min Filter   | {}                           |\n",
           gl_enum_to_string(tex.min_filter))
       << std::format("| Mag Filter   | {}                           |\n",
           gl_enum_to_string(tex.mag_filter));
    return os;
}


void RenderData::free() {
    if (Context::instance().is_context_active()) {
        SAFE_CALL(glDeleteBuffers(1, &ebo));
        SAFE_CALL(glDeleteBuffers(vertex_buffers.size(),
                                  vertex_buffers.data()));
        SAFE_CALL(glDeleteVertexArrays(1, &vao));
        SAFE_CALL(glDeleteProgram(program));
    }
    ebo = 0;
    vertex_buffers.clear();
    vao = 0;
    stencil_ref = -1;
    ebo_count = 0;
}

GLuint gen_texture(GLenum target) {
    GLuint tex;
    SAFE_CALL(glGenTextures(1, &tex));
    SAFE_CALL(glBindTexture(target, tex));
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
    SAFE_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims.x, dims.y, 0,
                           GL_RGBA, GL_UNSIGNED_BYTE, texture));
    SAFE_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void bind_texture(GLenum target, GLuint id) {
    SAFE_CALL(glBindTexture(target, id));
}

void set_texture_meta(byte_t* raw_data, const TextureData& params) {
    SAFE_CALL(glBindTexture(params.target, params.id));
    SAFE_CALL(glTexImage2D(params.target, 0, params.format, params.w, params.h,
                           0, params.format, params.type, raw_data));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_WRAP_S, params.wrap_s));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_WRAP_T, params.wrap_t));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_MIN_FILTER,
                              params.min_filter));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_MAG_FILTER,
                              params.mag_filter));
    SAFE_CALL(glBindTexture(params.target, 0));
}

void set_texture_2d_array_meta(byte_t* raw_data,
                               const TextureDataArray2D& data) {
    const auto t = data.tex_data.target;
    SAFE_CALL(glBindTexture(t, data.tex_data.id));

    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_BASE_LEVEL, 0));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_MAX_LEVEL, 1));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_MAG_FILTER,
              data.tex_data.mag_filter));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_MIN_FILTER,
              data.tex_data.min_filter));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_WRAP_S, data.tex_data.wrap_s));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_WRAP_T, data.tex_data.wrap_t));

    SAFE_CALL(glTexStorage3D(t, 1, GL_RGBA8, data.tile_w(), data.tile_h(),
              data.total_tiles()));

    SAFE_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, data.tex_data.w));
    SAFE_CALL(glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, data.tex_data.h));

    for (GLsizei i = 0; i < data.total_tiles(); ++i) {
        int ix = i % data.tile_count_h;
        int iy = i / data.tile_count_w;
        int x = ix * data.tile_w();
        int y = iy * data.tile_h();
        SAFE_CALL(glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, i,
            data.tile_w(), data.tile_h(), 1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            raw_data + ((y * data.tex_data.w + x) * 4)
        ));
    }
    SAFE_CALL(glGenerateMipmap(t));

    SAFE_CALL(glBindTexture(t, 0));
}

void activate_texture(const TextureActivationCommand& cmd) {
    SAFE_CALL(glActiveTexture(cmd.tex_type));
    SAFE_CALL(glBindTexture(cmd.sampler_type, cmd.id));
    set_int(cmd.program, cmd.sampler_name, 0);
}

void activate_texture(GLuint id) {
    SAFE_CALL(glBindTexture(GL_TEXTURE_2D, id));
    SAFE_CALL(glActiveTexture(GL_TEXTURE0));
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

}
