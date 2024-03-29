#include "observer.hpp"


namespace ui {

KeyEvent::KeyEvent(int k, int s, int a, int m)
    : key(k)
    , scancode(s)
    , action(a)
    , mode(m)
{}

std::ostream& operator << (std::ostream& os, const KeyEvent& event) {
    os << "<KeyEvent. key: " << event.key << ", scancode: " << event.scancode
       << ", action: " << event.action << ", mode: " << event.mode << ">";
    return os;
}

MouseEvent::MouseEvent(double x, double y)
    : xpos(x)
    , ypos(y)
{}

std::ostream& operator << (std::ostream& os, const MouseEvent& event) {
    os << "<MouseEvent. xpos: " << event.xpos << ", ypos: " << event.ypos
       << ">";
    return os;
}

bool operator == (const MouseEvent& lhs, const MouseEvent& rhs) {
    static constexpr double E = 1e-7;
    double dx = std::abs(lhs.xpos - rhs.xpos);
    double dy = std::abs(lhs.ypos - rhs.ypos);
    return dx <= E || dy <= E;
}

bool operator != (const MouseEvent& lhs, const MouseEvent& rhs) {
    return !(lhs == rhs);
}

MouseButtonEvent::MouseButtonEvent(int b, int a, int m)
    : button(b)
    , action(a)
    , mode(m)
{}

std::ostream& operator << (std::ostream& os, const MouseButtonEvent& e) {
    os << "<MouseButtonEvent. button: " << e.button << ", action: " << e.action
       << ", mode: " << e.mode << ">";
    return os;
}

ScrollEvent::ScrollEvent(double x, double y)
    : xoffset(x)
    , yoffset(y)
{}

std::ostream& operator << (std::ostream& os, const ScrollEvent& e) {
    os << "<ScrollEvent. dx: " << e.xoffset << ", dy: " << e.yoffset << ">";
    return os;
}

DropEvent::DropEvent(int c, const char** p)
    : paths(c) {
    for (ptrdiff_t i = 0; i < c; ++i) {
        paths[i] = std::filesystem::path(p[i]);
    }
}

DropEvent::DropEvent(std::vector<std::filesystem::path>&& p)
    : paths(std::move(p))
{}


std::ostream& operator << (std::ostream& os, const DropEvent& e) {
    os << "<DropEvent ";
    for (const auto& path : e.paths) {
        os << path.string() << "; ";
    }
    return os << ">";
}

FramebufferEvent::FramebufferEvent(GLFWwindow* win, int w, int h)
    : window(win)
    , height(h)
    , width(w)
{}

std::ostream& operator << (std::ostream& os, const FramebufferEvent& e) {
    return os << "<FramebufferEvent width: "
              << e.width << ", height: "
              << e.height;
}


Publisher::Publisher()
    : _listeners(std::vector<Listener*>{})
{}

Publisher::~Publisher() {
    _listeners.clear();
}

void Publisher::subscribe(Listener* listener) {
    _listeners.push_back(listener);
}

void Publisher::unsubscribe(Listener* listener) {
    auto candidate = std::find(_listeners.begin(), _listeners.end(),
        listener);
    if (candidate == _listeners.end()) { return; }

    _listeners.erase(candidate);
}

void Publisher::emit(const KeyEvent& event) const {
    templated_emit<KeyEvent>(event);
}

void Publisher::emit(const MouseEvent& event) const {
    templated_emit<MouseEvent>(event);
}

void Publisher::emit(const MouseButtonEvent& event) const {
    templated_emit<MouseButtonEvent>(event);
}

void Publisher::emit(const ScrollEvent& event) const {
    templated_emit<ScrollEvent>(event);
}

void Publisher::emit(const DropEvent& event) const {
    templated_emit<DropEvent>(event);
}

void Publisher::emit(const FramebufferEvent& event) const {
    templated_emit<FramebufferEvent>(event);
}

void Publisher::predicate(std::function<bool(void)> p) {
    predicate_ = p;
}

template <typename Event>
inline void Publisher::templated_emit(const Event& e) const {
    for (Listener* l : _listeners) {
        if (bool(predicate_) && predicate_()) {
            l->consume(e);
        }
    }
}

}
