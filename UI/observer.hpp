#pragma once

#include <vector>
#include <ostream>
#include <filesystem>
#include <functional>


struct GLFWwindow;
namespace ui {

struct KeyEvent {
    int key;
    int scancode;
    int action;
    int mode;

public:
    KeyEvent() = default;
    KeyEvent(int k, int s, int a, int m);
};
std::ostream& operator << (std::ostream& os, const KeyEvent& event);


struct MouseEvent {
    double xpos;
    double ypos;

public:
    MouseEvent() = default;
    MouseEvent(double x, double y);
};
std::ostream& operator << (std::ostream& os, const MouseEvent& event);
bool operator == (const MouseEvent& lhs, const MouseEvent& rhs);
bool operator != (const MouseEvent& lhs, const MouseEvent& rhs);


struct MouseButtonEvent {
    int button;
    int action;
    int mode;

public:
    MouseButtonEvent() = default;
    MouseButtonEvent(int b, int a, int m);
};
std::ostream& operator << (std::ostream& os, const MouseButtonEvent& event);


struct ScrollEvent {
    double xoffset;
    double yoffset;

public:
    ScrollEvent() = default;
    ScrollEvent(double x, double y);
};
std::ostream& operator << (std::ostream& os, const ScrollEvent& event);


struct DropEvent {
    std::vector<std::filesystem::path> paths;

public:
    DropEvent() = default;
    DropEvent(int c, const char** p);
    DropEvent(std::vector<std::filesystem::path>&& p);
};
std::ostream& operator << (std::ostream& os, const DropEvent& event);

struct FramebufferEvent {
    GLFWwindow* window;
    int width;
    int height;

public:
    FramebufferEvent() = default;
    FramebufferEvent(GLFWwindow* win, int w, int h);
};
std::ostream& operator << (std::ostream& os, const FramebufferEvent& e);


class Listener;
class Publisher {
public:
    Publisher();
    virtual ~Publisher();

    void subscribe(Listener* listener);
    void unsubscribe(Listener* listener);

    void emit(const KeyEvent& event) const;
    void emit(const MouseEvent& event) const;
    void emit(const MouseButtonEvent& event) const;
    void emit(const ScrollEvent& event) const;
    void emit(const DropEvent& event) const;
    void emit(const FramebufferEvent& event) const;

    void predicate(std::function<bool(void)> predicate);

private:
    template <typename Event> void templated_emit(const Event&) const;

private:
    std::function<bool(void)> predicate_ {[]() { return true; }};
    std::vector<Listener*> _listeners;
};

class Listener {
public:
    Listener(Publisher* parent = nullptr)
        : _parent(parent)
    {
        if (_parent) { _parent->subscribe(this); }
    }

    virtual ~Listener()
    {
        if (_parent) { _parent->unsubscribe(this); }
    }

    void bind(Publisher* parent) { _parent = parent; }

    virtual void consume(const KeyEvent& event) = 0;
    virtual void consume(const MouseEvent& event) = 0;
    virtual void consume(const MouseButtonEvent& event) = 0;
    virtual void consume(const ScrollEvent& event) = 0;
    virtual void consume(const DropEvent& event) = 0;
    virtual void consume(const FramebufferEvent& event) = 0;

private:
    Publisher* _parent;
};

}
