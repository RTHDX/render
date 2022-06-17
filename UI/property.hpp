#pragma once


namespace ui {

template <typename T> class Property {
public:
    Property(T init) : _value(init) {}

    void set(const T& value) { _value = value; }
    const T& get() const { return _value; }

private:
    T _value;
};

}
