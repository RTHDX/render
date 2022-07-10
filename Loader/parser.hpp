#pragma once

#include <regex>
#include <string>
#include <format>
#include <sstream>

#include <glm/glm.hpp>
#include <parselib/parselib.hpp>

#include "material.hpp"


namespace loader {

const std::string& float_template() {
    static std::string float_pattern(R"(\d+\.\d+)");
    return float_pattern;
}

const std::string& vec3_template() {
    static std::string pattern(std::format("{} {} {}", float_template(),
                                                       float_template(),
                                                       float_template()));
    return pattern;
}

const std::regex& Ka_template() {
    static std::regex pattern(std::format("Ka {}", vec3_template()));
    return pattern;
}

const std::regex& Kd_template() {
    static std::regex pattern(std::format("Kd {}", vec3_template()));
    return pattern;
}

const std::regex& Ks_template() {
    static std::regex pattern(std::format("Ks {}", vec3_template()));
    return pattern;
}

const std::regex& Ni_template() {
    static std::regex pattern(std::format("Ni {}", float_template()));
    return pattern;
}

const std::regex& Ns_template() {
    static std::regex pattern(std::format("Ns {}", float_template()));
    return pattern;
}

const std::regex& d_template() {
    static std::regex pattern(std::format("d {}", float_template()));
    return pattern;
}

const std::regex& illum_pattern() {
    static std::regex pattern("illum \d");
    return pattern;
}

const std::regex& newmtl_pattern() {
    static std::regex pattern("newmtl .+");
    return pattern;
}

glm::vec3 vec3_converter(const std::string& in) {
    glm::vec3 out;
    std::stringstream ss(in);
    std::string temp;
    ss >> temp >> out.x >> out.y >> out.z;
    return out;
}

float float_converter(const std::string& in) {
    float out;
    std::stringstream ss(in);
    std::string temp;
    ss >> temp >> out;
    return out;
}

std::string string_converter(const std::string& in) {
    std::string out;
    std::stringstream ss(in);
    std::string temp;
    ss >> temp >> out;
    return out;
}

IluminationModel illumination_converter(const std::string& in) {
    uint32_t out;
    std::stringstream ss(in);
    std::string temp;
    ss >> temp >> out;
    return (IluminationModel)out;
}


struct LexemType {
    enum Tag {
        INTEGER = 0,
        FLOAT,
        STRING,

        NEW_MTL = 10,
        KA,
        KD,
        KS,
        NS,
        NI,
        D,
        ILLUM,
        MAP_KD
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
    Vector3f_AST();

    void pop(parselib::AST* ast) const override;
    void append(parselib::AST* ast) const override;
    void accept(parselib::Visitor* v) const override;

    const parselib::uAST& x() const { return _x; }
    const parselib::uAST& y() const { return _y; }
    const parselib::uAST& z() const { return _z; }

private:
    parselib::uAST _x, _y, _z;
};

class String_AST final : public parselib::AST {
public:
    String_AST(const std::string& c);
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
    void accept(parselib::AST* ast) override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Kd_AST final : public parselib::AST {
public:
    Kd_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Ks_AST final : public parselib::AST {
public:
    Ks_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) override;

    const parselib::uAST& vector() const { return _vector; }

private:
    parselib::uAST _vector;
};

class Ns_AST final : public parselib::AST {
public:
    Ns_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class Ni_AST final : public parselib::AST {
public:
    Ni_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class D_AST final : public parselib::AST {
public:
    D_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    const accept(parselib::Visitor* v) override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class Illm_AST final : public parselib::AST {
public:
    Illm_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

class MapKd_AST final : public parselib::AST {
public:
    MapKd_AST() = default;

    void pop(parselib::AST* ast) override;
    void append(parselib::AST* ast) override;
    void accept(parselib::Visitor* v) override;

    const parselib::uAST& value() const { return _value; }

private:
    parselib::uAST _value;
};

parselib::Parser& integer_parser();
parselib::Parser& float_parser();
parselib::Parser& string_parser();
// newmtl = 'newmtl' + string
parselib::Parser& new_mtl_parser();
// Ka = 'Ka' + float + float + float
parselib::Parser& ka_parser();
// Kd = 'Kd' + float + float + float
parselib::Parser& kd_parser();
// Ks = 'Ks' + float + float + float
parselib::Parser& ks_parser();
// Ns = 'Ns' + float
parselib::Parser& ns_parser();
// Ni = 'Ni' + float
parselib::Parser& ni_parser();
// d = 'd' + float
parselib::Parser& d_parser();
// illum = illum + integer
parselib::Parser& illum_parser();
// map_Kd = 'map_Kd' + string
parselib::Parser& map_kd_parser();

}

namespace parselib {

class Visitor {
public:
    Visitor() = default;

    void take(loader::Material* material);
    void release();

private:
    loader::Material* _material;
};

}
