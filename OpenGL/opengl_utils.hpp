#pragma once

#include <filesystem>

#include <glad/glad.h>


namespace opengl::utils {

void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char* message, const void* userParam);
GLenum gl_check_error(const char* file, int line);
GLenum convert_to_shader_type(const std::filesystem::path& path);
std::string read_shader(const std::filesystem::path& path);

}
