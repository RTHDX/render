#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

struct GLFWwindow;
namespace ui {

bool init_imgui_opengl3_glfw(GLFWwindow* win);

class Visitor;
class Widget;
using widget_sptr_t = std::shared_ptr<Widget>;
class Widget {
public:
    Widget(const glm::vec2& size, const std::string& title);
    virtual ~Widget() = default;

    virtual void accept(Visitor& v) = 0;

    const glm::vec2& size() const;
    const std::string& title() const;

    void append(widget_sptr_t&& item);
    const std::vector<widget_sptr_t>& components() const;

private:
    glm::vec2 size_                        {};
    std::string title_                     {};
    std::vector<widget_sptr_t> components_ {};
};


class Window final : public Widget {
public:
    static std::shared_ptr<Window> create(
        const glm::vec2& top_left,
        const glm::vec2& size,
        const std::string& title,
        int flags
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


class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Window& w) = 0;
};

}
