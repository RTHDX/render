#include "observer.hpp"


namespace ui {

KeyEvent::KeyEvent(int k, int s, int a, int m)
    : key(k)
    , scancode(s)
    , action(a)
    , mode(m)
{}

MouseEvent::MouseEvent(double x, double y)
    : xpos(x)
    , ypos(y)
{}

MouseButtonEvent::MouseButtonEvent(int b, int a, int m)
    : button(b)
    , action(a)
    , mode(m)
{}

ScrollEvent::ScrollEvent(double x, double y)
    : xoffset(x)
    , yoffset(y)
{}

DropEvent::DropEvent(int c, const char** p)
    : count(c)
    , paths(p)
{}


Publisher::Publisher()
    : _listeners(std::vector<Listener*>{})
{
}

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

template <typename Event> inline void Publisher::templated_emit(const Event& e) const {
    for (Listener* l : _listeners) {
        l->consume(e);
    }
}

}
