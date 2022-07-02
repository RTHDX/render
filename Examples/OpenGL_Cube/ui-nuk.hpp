#pragma once

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static constexpr size_t MAX_VERTEX_BUFFER = 512 * 1024;
static constexpr size_t MAX_ELEMENT_BUFFER = 128 * 1024;

static void init_fonts() {
    struct nk_font_atlas* font;
    nk_glfw3_font_stash_begin(&font);
    nk_glfw3_font_stash_end();
}

nk_context* init_context(GLFWwindow* window) {
    auto* ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS,
                              MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    init_fonts();
    return ctx;
}
