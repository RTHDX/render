#pragma once

#include <regex>
#include <string>
#include <format>
#include <sstream>
#include <filesystem>

#include <glm/glm.hpp>
#include <parselib/parselib.hpp>

#include "material.hpp"


namespace loader {

struct LexemType {
    enum Tag {
        INTEGER = 1,
        FLOAT,
        STRING,

        NEW_MTL = 10,
        KA,
        KD,
        KS,
        KE,
        NS,
        NI,
        D,
        ILLUM,
        MAP_KD,

        SPACE = 50,
        COMMENT,
        EOL
    };
};

parselib::Rules create_rules();

class Int_AST final : public parselib::AST {
public:
    Int_AST(const std::string& c);
    NO_POP
    NO_APPEND
    virtual void accept(parselib::Visitor*) const override;

    int value() const { return _value; }

private:
    int _value;
};

class Float_AST final : public parselib::AST {
public:
    Float_AST(const std::string& c);
    NO_POP
    NO_APPEND
    virtual void accept(parselib::Visitor*) const override;

    float value() const { return _value; }

private:
    float _value;
};

class Vector3f_AST final : public parselib::AST {
public:
    Vector3f_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& x() const { return _x; }
    const parselib::uAST& y() const { return _y; }
    const parselib::uAST& z() const { return _z; }

private:
    parselib::uAST _x, _y, _z;
};

class String_AST final : public parselib::AST {
public:
    String_AST(const std::string c);
    NO_POP
    NO_APPEND
    void accept(parselib::Visitor*) const override;

    const std::string& value() const { return _value; }

private:
    std::string _value;
};

class NewMtl_AST final : public parselib::AST {
public:
    NewMtl_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& name() const { return _name; }

private:
    parselib::uAST _name;
};

class Ka_AST final : public parselib::AST {
public:
    Ka_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* ast) const override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Ke_AST final : public parselib::AST {
public:
    Ke_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* ast) const override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Kd_AST final : public parselib::AST {
public:
    Kd_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Ks_AST final : public parselib::AST {
public:
    Ks_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Ns_AST final : public parselib::AST {
public:
    Ns_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class Ni_AST final : public parselib::AST {
public:
    Ni_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class D_AST final : public parselib::AST {
public:
    D_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class Illm_AST final : public parselib::AST {
public:
    Illm_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class MapKd_AST final : public parselib::AST {
public:
    MapKd_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class Mtl_AST final : public parselib::AST {
public:
    Mtl_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const std::vector<parselib::uAST>& components() const { return _components; }

private:
    std::vector<parselib::uAST> _components;
};

class Wrapper_AST final : public parselib::AST {
public:
    Wrapper_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) const override;

    const std::vector<parselib::uAST>& components() const { return _components; }

private:
    std::vector<parselib::uAST> _components;
};

parselib::Parser integer_parser();
parselib::Parser float_parser();
parselib::Parser string_parser();
// newmtl = 'newmtl' + string
parselib::Parser new_mtl_parser();
// Ka = 'Ka' + float + float + float
parselib::Parser ka_parser();
// Kd = 'Kd' + float + float + float
parselib::Parser kd_parser();
// Ks = 'Ks' + float + float + float
parselib::Parser ks_parser();
// Ke = 'Ke' + flaot + float + float
parselib::Parser ke_parser();
// Ns = 'Ns' + float
parselib::Parser ns_parser();
// Ni = 'Ni' + float
parselib::Parser ni_parser();
// d = 'd' + float
parselib::Parser d_parser();
// illum = illum + integer
parselib::Parser illum_parser();
// map_Kd = 'map_Kd' + string
parselib::Parser map_kd_parser();
// mtl_doc = any+
parselib::Parser mtl_parser();
}

namespace parselib {

class Visitor {
public:
    Visitor() = default;
    virtual ~Visitor() = default;

    virtual void visit(const loader::Int_AST& ast) = 0;
    virtual void visit(const loader::Float_AST& ast) = 0;
    virtual void visit(const loader::Vector3f_AST& ast) = 0;
    virtual void visit(const loader::String_AST& ast) = 0;
    virtual void visit(const loader::NewMtl_AST& ast) = 0;
    virtual void visit(const loader::Ka_AST& ast) = 0;
    virtual void visit(const loader::Kd_AST& ast) = 0;
    virtual void visit(const loader::Ks_AST& ast) = 0;
    virtual void visit(const loader::Ke_AST& ast) = 0;
    virtual void visit(const loader::Ns_AST& ast) = 0;
    virtual void visit(const loader::Ni_AST& ast) = 0;
    virtual void visit(const loader::D_AST& ast) = 0;
    virtual void visit(const loader::Illm_AST& ast) = 0;
    virtual void visit(const loader::MapKd_AST& ast) = 0;
    virtual void visit(const loader::Mtl_AST& ast) = 0;

    virtual void visit(const loader::Wrapper_AST& ast) = 0;
};

}

namespace loader {

Material read_material(const std::filesystem::path& path);

class MtlBuilder final : public parselib::Visitor {
public:
    void take(Material* material);

    void visit(const loader::Int_AST& ast) override;
    void visit(const loader::Float_AST& ast) override;
    void visit(const loader::Vector3f_AST& ast) override;
    void visit(const loader::String_AST& ast) override;
    void visit(const loader::NewMtl_AST& ast) override;
    void visit(const loader::Ka_AST& ast) override;
    void visit(const loader::Kd_AST& ast) override;
    void visit(const loader::Ks_AST& ast) override;
    void visit(const loader::Ke_AST& ast) override;
    void visit(const loader::Ns_AST& ast) override;
    void visit(const loader::Ni_AST& ast) override;
    void visit(const loader::D_AST& ast) override;
    void visit(const loader::Illm_AST& ast) override;
    void visit(const loader::MapKd_AST& ast) override;
    void visit(const loader::Mtl_AST& ast) override;

    void visit(const loader::Wrapper_AST& ast) override;

    void release();

private:
    Material* _material = nullptr;

    glm::vec3 _vec3_temp = {};
    int _int_temp = 0;
    float _float_temp = 0.0;
    std::string _str_temp = {};
};

}
