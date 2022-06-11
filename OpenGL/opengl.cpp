#include <iostream>
#include <bitset>
#include <format>

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
}

void Context::dump() const {
    GLenum enums[] = {
        GL_MAJOR_VERSION,
        GL_MINOR_VERSION,
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO,
    };
    const char* names[] = {
        "GL_MAJOR_VERSION",
        "GL_MINOR_VERSION",
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO",
    };

    std::cout << "[Context] Opengl ----------------------------- " << std::endl;
    for (size_t i = 0; i < (sizeof(enums) / sizeof(GLenum)) - 1; ++i) {
        int int_value;
        glGetIntegerv (enums[i], &int_value);
        std::cout << names[i] << ": " << int_value << std::endl;
    }
    int values[2];
    values[0] = values[1] = 0;
    glGetIntegerv(enums[13], values);
    std::cout << names[13] << ": " << values[0] << " " << values[1] << std::endl;
    std::cout << "VENDOR: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "RENDERER: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "SHADING LANGUAGE VERSION: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Context::viewport(int width, int height) const {
    glViewport(0, 0, width, height);
}

void Context::background(const glm::vec4& color) {
    _background = color;
}

void Context::draw_background() const {
    glClearColor(_background.r, _background.g, _background.b, _background.a);
    glClear(GL_COLOR_BUFFER_BIT);
}


Buffer::Buffer() : Object(), _is_bound(false) {}


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
{}

Program::~Program() {
    glDeleteProgram(id());
}

void Program::initialize() {
    id(glCreateProgram());
    update_state(State::INITIALIZED);
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


GLuint create_program(const std::string_view vertex_shader_src,
                      const std::string_view fragment_shader_src) {
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

}
