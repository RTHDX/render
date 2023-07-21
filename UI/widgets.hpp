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
        std::shared_ptr<CanvasEntity> self(new CanvasEntity());
        self->render_data_.program = opengl::create_program(vertex, fragment);
        self->render_data_.vao = opengl::gen_vertex_array();
        self->render_data_.ebo = opengl::gen_element_buffer();
        self->render_data_.vertex_buffers = informat_t::gen_buffers(
            self->render_data_.vao, vertex_input,
            self->render_data_.ebo, elements_input
        );
        self->count_ = elements_input.size();
        std::cout << opengl::get_program_interface(self->render_data_.program) << std::endl;
        return self;
    }

    const opengl::RenderData& render_data() const;
    void model(const glm::mat4& v);
    const glm::mat4& model() const;

    opengl::DrawElementsCommand draw_command() const;

private:
    opengl::RenderData render_data_;
    glm::mat4 model_ {glm::mat4(1.0)};
    size_t count_    {0};
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
    void update(GLuint w, GLuint h);
    void append(entity_sptr_t&& entity);

    const opengl::FramebufferData& fbuff() const;
    const entities_list_t& entities() const;
    const glm::mat4& projection() const;
    const glm::mat4& view() const;

    void background(const glm::vec4& b);
    const glm::vec4& background() const;

private:
    using Widget::Widget;

private:
    opengl::FramebufferData fbuff_data_ {};
    entities_list_t entities_           {};
    glm::mat4 projection_               {glm::mat4(1.0)};
    glm::mat4 view_                     {glm::mat4(1.0)};
    glm::vec4 background_               {glm::vec4(1.0, 1.0, 0.0, 1.0)};
};


class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Window& w) = 0;
    virtual void visit(Canvas& w) = 0;
};

}
