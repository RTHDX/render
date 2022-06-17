#pragma once

#include <string_view>
#include <vector>
#include <memory>

struct nk_context;
struct GLFWwindow;
namespace ui::nuklear {

nk_context* init(GLFWwindow* window);
void init_fonts();

class Widget;
using uWidget = std::unique_ptr<Widget>;
class Widget {
public:
    Widget(const std::string_view name, struct nk_context* ctx);

    virtual void show() = 0;

    void append(uWidget&& child);

    nk_context* ctx() { return _ctx; }
    const std::string& name() const { return _name; }
    const std::vector<uWidget>& children() const { return _children; }

private:
    std::string _name;
    nk_context* _ctx;
    std::vector<uWidget> _children;
};


class Window : public Widget {
public:
    Window(const std::string_view name, struct nk_context* ctx,
           const struct nk_rect rectangle, nk_flags flags);

    void show() override;

private:
    struct nk_rect _dim;
    nk_flags _flags;
};


class LabelButton final : public Widget {
public:
    LabelButton(const std::string_view name, struct nk_context* ctx);

    void show() override;
};

}
