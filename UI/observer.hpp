#pragma once

#include <vector>


namespace ui {


struct KeyEvent {
    int key;
    int scancode;
    int action;
    int mode;

public:
    KeyEvent(int k, int s, int a, int m);
};


struct MouseEvent {
    double xpos;
    double ypos;

public:
    MouseEvent(double x, double y);
};


struct MouseButtonEvent {
    int button;
    int action;
    int mode;

public:
    MouseButtonEvent(int b, int a, int m);
};


struct ScrollEvent {
    double xoffset;
    double yoffset;

public:
    ScrollEvent(double x, double y);
};


struct DropEvent {
    int count;
    const char** paths;

public:
    DropEvent(int c, const char** p);
};


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

private:
    template <typename Event> void templated_emit(const Event&) const;

private:
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

private:
    Publisher* _parent;
};

}
