#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>


struct GLFWwindow;
namespace ui {
namespace fs = std::filesystem;

bool init_imgui_opengl3_glfw(GLFWwindow* win);

class Visitor;
class Widget;
using widget_sptr_t = std::shared_ptr<Widget>;
using widget_list_t = std::vector<widget_sptr_t>;
class Widget {
public:
    Widget(const glm::vec2& size, const std::string& title);
    virtual ~Widget() = default;

    virtual void accept(Visitor& v) = 0;

    const glm::vec2& size() const;
    const std::string& title() const;

    void append(widget_sptr_t&& item);
    void append(widget_list_t&& list);
    const widget_list_t& components() const;

private:
    glm::vec2 size_           {};
    std::string title_        {};
    widget_list_t components_ {};
};


class Window final : public Widget {
public:
    static std::shared_ptr<Window> create(
        const glm::vec2& top_left,
        const glm::vec2& size,
        const std::string& title,
        int flags
    );
    static std::shared_ptr<Window> create(
        const glm::vec2& top_left,
        const glm::vec2& size,
        const std::string& title,
        int flags,
        widget_list_t&& comps
    );

    void accept(Visitor& v) override;

    const glm::vec2& top_left() const;
    int flags() const;

    bool is_visible() const;
    void is_visible(bool v);

private:
    using Widget::Widget;

private:
    glm::vec2 top_left_;
    int flags_;
    bool is_visible_ = true;
};

class CanvasEntity final {
public:
    template <opengl::vertex_input_c informat_t>
    static std::shared_ptr<CanvasEntity> create(
           const fs::path& vertex,
           const fs::path& fragment,
           const informat_t::vertex_input_t& vertex_input,
           const opengl::elements_input_t& elements_input) {
        render_data_.program = opengl::create_program(vertex, fragment);
        render_data_.vao = opengl::gen_vertex_array();
        render_data_.ebo = opengl::gen_element_buffer();
        render_data_.vertex_buffers = informat_t::gen_buffers(
            render_data_.vao, vertex_input,
            render_data_.ebo, elements_input
        );
    }

    const opengl::RenderData& render_data() const;

private:
    opengl::RenderData render_data_;
};
using entity_sptr_t = std::shared_ptr<CanvasEntity>;
using entities_list_t = std::vector<entity_sptr_t>;


class Canvas final : public Widget {
public:
    static std::shared_ptr<Canvas> create(
        const glm::vec2& size,
        const std::string& title
    );
    static std::shared_ptr<Canvas> create(
        const glm::vec2& size,
        const std::string& title,
        entities_list_t&& enities
    );

    ~Canvas() override;

    void accept(Visitor& v) override;

private:
    using Widget::Widget;

private:
    GLuint fbuff_                 {0};
    opengl::TextureData tex_data_ {};
    entities_list_t entities_     {};
};


class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Window& w) = 0;
    virtual void visit(Canvas& w) = 0;
};

}
