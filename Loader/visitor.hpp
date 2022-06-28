#pragma once


namespace loader {
struct Face;
struct Mesh;

class Visitor {
public:
    Visitor() = default;
    virtual ~Visitor() = default;

    virtual void visit(const Face&) = 0;
    virtual void visit(const Mesh&) = 0;
};


struct Visitable {
public:
    Visitable() = default;
    virtual ~Visitable() = default;

    virtual void accept(Visitor& visitor) = 0;
};

}
