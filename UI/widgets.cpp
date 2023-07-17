#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "widgets.hpp"


namespace ui {


bool init_imgui_opengl3_glfw(GLFWwindow* win) {
    IMGUI_CHECKVERSION();
    auto* ctx = ImGui::CreateContext();
    if (!ctx) {
        return false;
    }
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    if (!ImGui_ImplGlfw_InitForOpenGL(win, true)) {
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 460")) {
        return false;
    }
    return true;
}


Widget::Widget(const glm::vec2& size, const std::string& title)
    : size_(size)
    , title_(title)
{}

const glm::vec2& Widget::size() const { return size_; }
const std::string& Widget::title() const { return title_; }

void Widget::append(widget_sptr_t&& item) {
    components_.push_back(std::move(item));
}

const std::vector<widget_sptr_t>& Widget::components() const {
    return components_;
}


std::shared_ptr<Window> Window::create(const glm::vec2& top_left,
                                       const glm::vec2& size,
                                       const std::string& title,
                                       int flags) {
    std::shared_ptr<Window> self(new Window(size, title));
    self->top_left_ = top_left;
    self->flags_ = flags;
    return self;
}

void Window::accept(Visitor& v) {
    v.visit(*this);
}

const glm::vec2& Window::top_left() const { return top_left_; }
int Window::flags() const { return flags_; }

bool Window::is_visible() const {
    return is_visible_;
}

void Window::is_visible(bool v) {
    is_visible_ = v;
}

}
