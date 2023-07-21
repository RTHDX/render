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
    style.WindowPadding = ImVec2(0.0, 0.0);
    style.ItemSpacing = ImVec2(0.0, 0.0);
    style.ItemInnerSpacing = ImVec2(0.0, 0.0);
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

void Widget::append(widget_list_t&& list) {
    components_ = std::move(list);
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

std::shared_ptr<Window> Window::create(const glm::vec2& top_left,
                                       const glm::vec2& size,
                                       const std::string& title,
                                       int flags,
                                       widget_list_t&& comps) {
    auto self = Window::create(top_left, size, title, flags);
    self->append(std::move(comps));
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


const opengl::RenderData& CanvasEntity::render_data() const {
    return render_data_;
}

void CanvasEntity::model(const glm::mat4& v) {
    model_ = v;
}

const glm::mat4& CanvasEntity::model() const {
    return model_;
}

opengl::DrawElementsCommand CanvasEntity::draw_command() const {
    return {
        .vao = render_data_.vao,
        .count = count_,
    };
}


std::shared_ptr<Canvas> Canvas::create(const glm::vec2& size,
                                       const std::string& title) {
    std::shared_ptr<Canvas> out(new Canvas(size, title));
    out->fbuff_data_ = {
        .fbo = opengl::gen_framebuffer(),
        .attachment_point = GL_COLOR_ATTACHMENT0,
        .texture = {
            .id = opengl::gen_texture(),
            .target = GL_TEXTURE_2D,
            .w = 0,
            .h = 0,
            .format = GL_RGBA,
            .type = GL_UNSIGNED_BYTE,
            .wrap_s = GL_CLAMP_TO_EDGE,
            .wrap_t = GL_CLAMP_TO_EDGE,
            .min_filter = GL_LINEAR,
            .mag_filter = GL_LINEAR
        }
    };
    return out;
}

std::shared_ptr<Canvas> Canvas::create(const glm::vec2& size,
                                       const std::string& title,
                                       entities_list_t&& entities) {
    auto self = create(size, title);
    self->entities_ = entities;
    return self;
}

Canvas::~Canvas() {
    //opengl::free_framebuffer(&fbuff_data_.fbo);
}

void Canvas::accept(Visitor& v) {
    v.visit(*this);
}

void Canvas::update(GLuint w, GLuint h) {
    fbuff_data_.texture.w = w;
    fbuff_data_.texture.h = h;
    opengl::set_texture_meta(nullptr, fbuff_data_.texture);
    opengl::attach_texture(fbuff_data_, fbuff_data_.texture);
}

void Canvas::append(entity_sptr_t&& entity) {
    entities_.push_back(std::move(entity));
}

const opengl::FramebufferData& Canvas::fbuff() const {
    return fbuff_data_;
}

const entities_list_t& Canvas::entities() const {
    return entities_;
}

const glm::mat4& Canvas::projection() const {
    return projection_;
}

const glm::mat4& Canvas::view() const {
    return view_;
}

void Canvas::background(const glm::vec4& b) {
    background_ = b;
}

const glm::vec4& Canvas::background() const {
    return background_;
}

}
