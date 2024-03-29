#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "opengl_utils.hpp"


namespace opengl::utils {

void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char* message, const void* userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
}


GLenum gl_check_error(const char* file, int line) {
    GLenum errorCode = glGetError();
    while (errorCode != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
        throw std::runtime_error(error);
    }
    return errorCode;
}

GLenum convert_to_shader_type(const std::filesystem::path& path) {
    const auto extension = path.extension();
    if (extension == std::filesystem::path(".vert")) {
        return GL_VERTEX_SHADER;
    }
    if (extension == std::filesystem::path(".frag")) {
        return GL_FRAGMENT_SHADER;
    }
    return 0;
}

std::string read_shader(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        std::wcerr << "No path found" << path.c_str() << std::endl;
        return {};
    }

    std::ifstream file(path.c_str());
    if (!file) {
        std::cerr << "Could not open" << std::endl;
        return std::string();
    }
    std::string content(
        (std::istreambuf_iterator<char>(file)),
         std::istreambuf_iterator<char>()
    );
    return content;
}

}
