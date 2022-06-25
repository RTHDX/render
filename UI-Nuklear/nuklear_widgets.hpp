#pragma once

#include <string_view>
#include <vector>
#include <memory>

#include <nuklear.h>

#include <RTX/rtx.hpp>


struct nk_context;
struct GLFWwindow;
namespace ui::nuklear {

nk_context* init(GLFWwindow* window);
void init_fonts();

class Widget;
using uWidget = std::unique_ptr<Widget>;
class Widget {
public:
    Widget(const std::string_view name);
    virtual ~Widget() = default;

    virtual void show(struct nk_context* ctx) = 0;

    void append(uWidget&& child);

    const std::string& name() const { return _name; }
    const std::vector<uWidget>& children() const { return _children; }

private:
    std::string _name;
    std::vector<uWidget> _children;
};


class Window : public Widget {
public:
    Window(const std::string_view name,
           const struct nk_rect rectangle, nk_flags flags);

    void show(struct nk_context* ctx) override;

private:
    struct nk_rect _dim;
    nk_flags _flags;
};

}
