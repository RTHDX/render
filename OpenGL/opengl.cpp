#include <iostream>
#include <bitset>
#include <format>

#include <glm/gtc/type_ptr.hpp>

#include "opengl_utils.hpp"
#include "opengl.hpp"


namespace opengl {

Context& Context::instance() {
    static Context self;
    return self;
}

void Context::initialize() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::terminate();
    }
    glDebugMessageCallback(utils::gl_debug_output, nullptr);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
}

void Context::dump() const {
    #define PRINT_OPENGL_FEATURE_INTV(FEATURE) \
        {\
            int value;\
            glGetIntegerv(FEATURE, &value);\
            std::cout << " - " << #FEATURE << ": " << value << std::endl;\
        }

    std::cout << "[Context] Opengl ----------------------------- " << std::endl;
    PRINT_OPENGL_FEATURE_INTV(GL_MAJOR_VERSION);
    PRINT_OPENGL_FEATURE_INTV(GL_MINOR_VERSION);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_DRAW_BUFFERS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_TEXTURE_IMAGE_UNITS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_TEXTURE_SIZE);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_VARYING_FLOATS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_VERTEX_ATTRIBS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    PRINT_OPENGL_FEATURE_INTV(GL_MAX_VERTEX_UNIFORM_COMPONENTS);

    std::cout << std::endl;
    #undef PRINT_OPENGL_FEATURE_INTV
}

void Context::viewport(int width, int height) const {
    glViewport(0, 0, width, height);
}

void Context::background(const glm::vec4& color) {
    _background = color;
}

void Context::draw_background() const {
    glClearColor(_background.r, _background.g, _background.b, _background.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


Buffer::Buffer() : Object(), _is_bound(false) {}


VertexArrayBuffer::VertexArrayBuffer() : Buffer() {}

VertexArrayBuffer::~VertexArrayBuffer() {
    glDeleteVertexArrays(1, id_ptr());
}

void VertexArrayBuffer::initialize() {
    glGenVertexArrays(1, id_ptr());
}

void VertexArrayBuffer::bind() {
    glBindVertexArray(id());
    is_bound(true);
}

void VertexArrayBuffer::unbind() {
    glBindVertexArray(0);
    is_bound(false);
}


VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, id_ptr());
}

void VertexBuffer::initialize() {
    glGenBuffers(1, id_ptr());
}

void VertexBuffer::bind(const float* data, size_t len) {
    glBindBuffer(GL_ARRAY_BUFFER, id());
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_STATIC_DRAW);
    is_bound(true);
}

void VertexBuffer::set_layout(GLuint index, GLuint width, GLsizei stride) {
    glVertexAttribPointer(index, width, GL_FLOAT, GL_FALSE,
                          stride * sizeof(float), nullptr);
    glEnableVertexAttribArray(index);
}


ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, id_ptr());
}

void ElementBuffer::initialize() {
    glGenBuffers(1, id_ptr());
}

void ElementBuffer::bind(const uint32_t* data, size_t len) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, data, GL_STATIC_DRAW);
}


Shader::Shader(GLuint shader_type)
    : Object()
    , _is_compiled(false)
    , _shader_type(shader_type)
{}

Shader::~Shader() {
    if (id() != 0) {
        glDeleteShader(id());
    }
}

void Shader::initialize() {
    id(glCreateShader(_shader_type));
    utils::gl_check_error(__FILE__, __LINE__);
}

void Shader::compile(const std::string_view source) {
    const GLchar* content = source.data();
    glShaderSource(id(), 1, &content, nullptr);
    glCompileShader(id());
    check_shader();
    is_compiled(true);
}

void Shader::check_shader() const {
    static constexpr size_t BUFF_SIZE = 512;

    int success;
    char infoLog[BUFF_SIZE];
    glGetShaderiv(id(), GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id(), BUFF_SIZE, nullptr, infoLog);
        std::cout << "[OpenGL] Compilation failed with: \n"
                  << infoLog << std::endl;
    }
}


Program::Program(const std::string_view label) : Object()
    , _label(label)
    , _vertex_shader(GL_VERTEX_SHADER)
    , _fragment_shader(GL_FRAGMENT_SHADER)
    , _current_state(NONE)
{
    initialize();
}

Program::~Program() {
    glDeleteProgram(id());
}

void Program::initialize() {
    if (_current_state < State::INITIALIZED) {
        id(glCreateProgram());
        update_state(State::INITIALIZED);
    }
}

void Program::attach_shader(const Shader& shader) {
    glAttachShader(id(), shader.id());

    switch (shader.shader_type()) {
    case GL_VERTEX_SHADER:
        update_state(State::VERTEX_SHADER_CREATED);
        break;
    case GL_FRAGMENT_SHADER:
        update_state(State::FRAGEMENT_SHADER_CREATED);
        break;
    }
}

void Program::attach_shader(GLuint shader_type, const std::string_view src) {
    if (_current_state < State::INITIALIZED) {
        static constexpr char MSG[] =
            "[OpenGL][{}] Initialize program before attaching shaders.";
        std::cerr << std::format(MSG, _label) << std::endl;
        return;
    }

    switch (shader_type) {
    case GL_VERTEX_SHADER:
        _vertex_shader.initialize();
        _vertex_shader.compile(src);
        attach_shader(_vertex_shader);
        break;
    case GL_FRAGMENT_SHADER:
        _fragment_shader.initialize();
        _fragment_shader.compile(src);
        attach_shader(_fragment_shader);
        break;
    }
}

void Program::link_program() {
    if (_current_state < State::FRAGEMENT_SHADER_CREATED) {
        static constexpr char MSG[] =
            "[OpenGL][{}] Attach shaders before linking the program";
        std::cerr << std::format(MSG, _label) << std::endl;
        return;
    }

    glLinkProgram(id());
    check_program();

    update_state(State::LINKED);
}

void Program::check_program() const {
    static constexpr char MSG[] = "[OpenGL][{}] Linking failed: %s";
    static constexpr size_t BUFF_SIZE = 512;

    int success;
    char info_log[BUFF_SIZE];
    glGetProgramiv(id(), GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id(), BUFF_SIZE, nullptr, info_log);
        std::cout << std::format(MSG, _label, info_log) << std::endl;
    }
}

void Program::use() {
    if (_current_state < State::LINKED) {
        static constexpr char MSG[] =
            "[OpenGL][{}] Create buffers before activate the program";
        std::cerr << std::format(MSG, _label) << std::endl;
        return;
    }

    glUseProgram(id());
    update_state(State::ACTIVE);
}

void Program::set_vec4(const std::string_view name, const glm::vec4& val) const {
    auto loc = glGetUniformLocation(id(), name.data());
    if (loc < 0) {
        std::cerr << "Could not find uniform: " << name << std::endl;
        return;
    }

    glUniform4f(loc, val.r, val.g, val.b, val.a);
}

void Program::set_vec3(const std::string_view name, const glm::vec3& val) const {
    auto loc = glGetUniformLocation(id(), name.data());
    if (loc < 0) {
        std::cerr << "Could not find uniform: " << name << std::endl;
        return;
    }

    glUniform3f(loc, val.x, val.y, val.z);
}

void Program::set_mat4(const std::string_view name, const glm::mat4& value) const {
    auto location = glGetUniformLocation(id(), name.data());
    if (location < 0) {
        std::cerr << "Could not find uniform " << name << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::update_state(State new_state) {
    static constexpr size_t BITS = sizeof (State) * 8;

    State state = (State)(_current_state | new_state);
    if (state != _current_state) {
        std::cout << std::format("[OpenGL][{}] old state: ", _label)
                  << std::bitset<BITS>(_current_state);
        std::cout << ", new state: " << std::bitset<BITS>(state)
                  << std::endl;
        _current_state = state;
    }
}


void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    Context::instance().viewport(width, height);
}


GLuint create_program(const std::string& vertex_shader_src,
                      const std::string& fragment_shader_src) {
    GLuint program         = glCreateProgram(),
           vertex_shader   = glCreateShader(GL_VERTEX_SHADER),
           fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const auto* src = vertex_shader_src.data();
    glShaderSource(vertex_shader, 1, &src, nullptr);
    glCompileShader(vertex_shader);

    src = fragment_shader_src.data();
    glShaderSource(fragment_shader, 1, &src, nullptr);
    glCompileShader(fragment_shader);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    return program;
}

GLuint create_program(const std::filesystem::path& vertex_path,
                      const std::filesystem::path& fragment_path) {
    auto vertex_src = opengl::utils::read_shader(vertex_path);
    auto fragment_src = opengl::utils::read_shader(fragment_path);
    return create_program(vertex_src, fragment_src);
}

static int find_location(GLuint id, const std::string_view name) {
    auto location = glGetUniformLocation(id, name.data());
    if (location < 0) {
        std::cerr << "Could not find uniform " << name << std::endl;
    }
    return location;
}

bool set_vec3(GLuint id, const std::string_view name, const glm::vec3& val) {
    auto loc = find_location(id, name);
    if (loc < 0) return false;
    glUniform3f(loc, val.x, val.y, val.z);
    return true;

}

bool set_vec4(GLuint id, const std::string_view name, const glm::vec4& val) {
    auto loc = find_location(id, name);
    if (loc < 0) return false;
    glUniform4f(loc, val.r, val.g, val.b, val.a);
    return true;
}

bool set_mat4(GLuint id, const std::string_view name, const glm::mat4& val) {
    auto loc = find_location(id, name);
    if (loc < 0) return false;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
    return true;
}

GLuint create_vao() {
    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}

void bind_vao(GLuint id) {
    glBindVertexArray(id);
}

GLuint create_vbo(GLuint vao, VertexBufferAttrib attribs) {
    GLuint id;
    glGenBuffers(1, &id);

    bind_vao(vao);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, attribs.total_width, attribs.data,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(attribs.index);
    glVertexAttribPointer(attribs.index, attribs.stride, GL_FLOAT, GL_FALSE,
                          attribs.width, attribs.offset);
    bind_vao(0);
    return id;
}

}
