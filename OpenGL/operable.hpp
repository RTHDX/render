#pragma once


namespace opengl {

class Item;

class Operator;
class Operable {
public:
    Operable() = default;

    void visit(Operator* op);
};


class Operator {
public:
    Operator() = default;

    void accept(Item& item);
};

}
