#pragma once


namespace opengl {

class Context {
public:
    static Context& instance();

    void initialize(bool = false);
    void initialize_light(bool = false);
    void dump() const;

    void viewport(int width, int height) const;
    glm::vec4& background() { return _background; }
    void background(const glm::vec4& color);
    void background(glm::u8vec4 color);

    bool is_initialized() const { return initialized_; }
    bool is_context_active() const;

    void draw_background() const;

    GLint active_program() const;
    GLint bound_vao() const;
    GLint bound_texture_2d() const;
    GLint bound_texture_2d_array() const;
    GLint bound_framebuffer() const;

private:
    Context() = default;

private:
    glm::vec4 _background;
    bool initialized_ = false;
};

}
