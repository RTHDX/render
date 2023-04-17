#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "opengl_utils.hpp"
#include "opengl_proc.hpp"


namespace opengl {

static inline GLint get(GLenum feature) {
    GLint val = 0;
    SAFE_CALL(glGetIntegerv(feature, &val));
    return val;
}

Context& Context::instance() {
    static Context self;
    return self;
}

void Context::initialize(bool to_dump) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        initialized_ = false;
        std::terminate();
    }
    SAFE_CALL(glDebugMessageCallback(utils::gl_debug_output, nullptr));
    SAFE_CALL(glEnable(GL_MULTISAMPLE));
    SAFE_CALL(glEnable(GL_STENCIL_TEST));
    SAFE_CALL(glEnable(GL_DEPTH_TEST));
    SAFE_CALL(glEnable(GL_BLEND));
    SAFE_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    initialized_ = true;
    if (to_dump) { dump(); }
}

void Context::initialize_light(bool to_dump) {
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::terminate();
    }
    if (to_dump) { dump(); }
}

void Context::dump() const {
#define PRINT_OPENGL_FEATURE_INTV(FEATURE) \
    std::cout << " - " << #FEATURE << ": " << get(FEATURE) << std::endl;

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
    SAFE_CALL(glViewport(0, 0, width, height));
}

void Context::background(const glm::vec4& color) {
    _background = color;
}

void Context::draw_background() const {
    static constexpr GLbitfield CLEAR_MODE = GL_COLOR_BUFFER_BIT
                                             | GL_DEPTH_BUFFER_BIT
                                             | GL_STENCIL_BUFFER_BIT;

    SAFE_CALL(glClearColor(_background.r,
                           _background.g,
                           _background.b,
                           _background.a));
    SAFE_CALL(glClearStencil(0x0));
    SAFE_CALL(glClear(CLEAR_MODE));
}

GLint Context::active_program() const { return get(GL_CURRENT_PROGRAM); }
GLint Context::bound_vao() const { return get(GL_VERTEX_ARRAY_BINDING); }
GLint Context::bound_texture_2d() const { return get(GL_TEXTURE_BINDING_2D); }

GLint Context::bound_texture_2d_array() const {
    return get(GL_TEXTURE_BINDING_2D_ARRAY);
}

}
