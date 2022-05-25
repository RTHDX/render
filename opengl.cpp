#include <iostream>

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
}

void Context::dump() const {
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "[Context] Opengl " << major << "." << minor << std::endl;
    std::cout << " - Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << " - Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << " - Shading language version: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glDebugMessageCallback(utils::gl_debug_output, nullptr);
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


VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, id_ptr());
}

void VertexBuffer::initialize() {
    glGenBuffers(1, id_ptr());
}

void VertexBuffer::bind(float* data, size_t len) {
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

void ElementBuffer::bind(uint32_t* data, size_t len) {
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
}

void Shader::compile(const char* source) {
    glShaderSource(id(), 1, &source, nullptr);
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


Program::~Program() {
    glDeleteProgram(id());
}

void Program::initialize() {
    id(glCreateProgram());
}

void Program::attach_shader(const Shader& shader) {
    glAttachShader(id(), shader.id());
}

void Program::link_program() {
    glLinkProgram(id());
}

void Program::check_program() const {
    int success;
    char infoLog[512];
    glGetProgramiv(id(), GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id(), 512, NULL, infoLog);
        std::cout << "[OpenGL] Linking failed: \n" << infoLog << std::endl;
    }
}

void Program::use() {
    glUseProgram(id());
}


Pipeline& Pipeline::create_vertex_shader(const std::string_view src) {
    _vrt_shader = Shader(GL_VERTEX_SHADER);
    _vrt_shader.initialize();
    _vrt_shader.compile(src.data());
    return *this;
}

Pipeline& Pipeline::create_fragment_shader(const std::string_view src) {
    _frg_shader = Shader(GL_FRAGMENT_SHADER);
    _frg_shader.initialize();
    _frg_shader.compile(src.data());
    return *this;
}

Pipeline& Pipeline::create_program() {
    _program = Program();
    _program.initialize();
    _program.attach_shader(_vrt_shader);
    _program.attach_shader(_frg_shader);
    _program.link_program();
    return *this;
}

Pipeline& Pipeline::create_vao() {
    _vertex_array_buffer = VertexArrayBuffer();
    _vertex_array_buffer.initialize();
    _vertex_array_buffer.bind();
    return *this;
}

Pipeline& Pipeline::create_vbo(float* data, size_t len) {
    _vertex_buffer_object = VertexBuffer();
    _vertex_buffer_object.initialize();
    _vertex_buffer_object.bind(data, len);
    return *this;
}

Pipeline& Pipeline::create_ebo(uint32_t* data, size_t len) {
    _element_buffer = ElementBuffer();
    _element_buffer.initialize();
    _element_buffer.bind(data, len);
    _vertex_count = len;
    return *this;
}

Pipeline& Pipeline::set_layot(GLuint index, GLuint width, GLuint stride) {
    _vertex_buffer_object.set_layout(index, width, stride);
    return *this;
}

void Pipeline::draw() {
    Context::instance().draw_background();

    _program.use();
    glDrawElements(GL_TRIANGLES, _vertex_count, GL_UNSIGNED_INT, nullptr);
}


void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    Context::instance().viewport(width, height);
}

}
