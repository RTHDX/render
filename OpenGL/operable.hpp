#pragma once


namespace opengl {

class Item;

class Operator;
class Operable {
public:
    Operable() = default;
    virtual ~Operable() = default;

    void visit(Operator* op);
};


class Operator {
public:
    Operator() = default;
    virtual ~Operator() = default;

    void accept(Item& item);
};

}
