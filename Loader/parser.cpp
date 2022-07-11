#include "parser.hpp"


namespace loader {

parselib::Rules create_rules() {
    return parselib::Rules {
        parselib::Rule("\\s+", LexemType::SPACE, true),
        parselib::Rule("#\.*\\n", LexemType::COMMENT, true),
        parselib::Rule("\\n", LexemType::EOL, true),

        parselib::Rule{R"(-?(\d+)?\.\d+)", LexemType::FLOAT},
        parselib::Rule{R"(\d+)", LexemType::INTEGER},
        parselib::Rule{R"(newmtl)", LexemType::NEW_MTL},
        parselib::Rule{R"(Ka)", LexemType::KA},
        parselib::Rule{R"(Kd)", LexemType::KD},
        parselib::Rule{R"(Ks)", LexemType::KS},
        parselib::Rule(R"(Ke)", LexemType::KE),
        parselib::Rule{R"(Ns)", LexemType::NS},
        parselib::Rule{R"(Ni)", LexemType::NI},
        parselib::Rule{R"(d)", LexemType::D},
        parselib::Rule{R"(illum)", LexemType::ILLUM},
        parselib::Rule{R"(map_Kd)", LexemType::MAP_KD},
        parselib::Rule{R"((\w|\.|\d)+)", LexemType::STRING},
    };
}

#define ACCEPT(CLASS) \
    void CLASS::accept(parselib::Visitor* v) const {\
        if (v) { v->visit(*this); }\
    }
#define POP_AST(MEMBER, AST) \
    if (MEMBER.get() == AST) { MEMBER.reset(); return; }
#define APPEND_AST(MEMBER, AST) \
    if (!MEMBER) { MEMBER.reset(AST); return; }


Int_AST::Int_AST(const std::string& c)
    : parselib::AST()
    , _value(std::atoi(c.c_str()))
{}
ACCEPT(Int_AST)

Float_AST::Float_AST(const std::string& c)
    : parselib::AST()
    , _value(std::atof(c.c_str()))
{}
ACCEPT(Float_AST)

String_AST::String_AST(const std::string& c)
    : parselib::AST()
    , _value(c)
{}
ACCEPT(String_AST)

void Vector3f_AST::pop(parselib::AST* ast) {
    if (_x.get() == ast) {
        _x.reset();
        return;
    }

    if (_y.get() == ast) {
        _y.reset();
        return;
    }

    if (_z.get() == ast) {
        _z.reset();
        return;
    }
}

void Vector3f_AST::append(parselib::AST* ast) {
    if (!_x) {
        _x.reset(ast);
        return;
    }

    if (!_y) {
        _y.reset(ast);
        return;
    }

    if (!_z) {
        _z.reset(ast);
        return;
    }
}
ACCEPT(Vector3f_AST)

void NewMtl_AST::pop(parselib::AST* ast) {
    if (_name.get() == ast) {
        _name.reset();
        return;
    }
}

void NewMtl_AST::append(parselib::AST* ast) {
    if (!_name) {
        _name.reset(ast);
        return;
    }
}
ACCEPT(NewMtl_AST)

void Ka_AST::pop(parselib::AST* ast) {
    if (_vector.get() == ast) {
        _vector.reset();
        return;
    }
}

void Ka_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
        return;
    }
}
ACCEPT(Ka_AST)

void Kd_AST::pop(parselib::AST* ast) {
    if (_vector.get() == ast) {
        _vector.reset();
        return;
    }
}

void Kd_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
        return;
    }
}
ACCEPT(Kd_AST)

void Ke_AST::pop(parselib::AST* ast) { POP_AST(_vector, ast); }
void Ke_AST::append(parselib::AST* ast) { APPEND_AST(_vector, ast); }
ACCEPT(Ke_AST)

void Ks_AST::pop(parselib::AST* ast) {
    if (_vector.get() == ast) {
        _vector.reset();
        return;
    }
}

void Ks_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
        return;
    }
}
ACCEPT(Ks_AST)

void Ns_AST::pop(parselib::AST* ast) {
    if (_value.get() == ast) {
        _value.reset();
        return;
    }
}

void Ns_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
        return;
    }
}
ACCEPT(Ns_AST)

void Ni_AST::pop(parselib::AST* ast) {
    if (_value.get() == ast) {
        _value.reset();
        return;
    }
}

void Ni_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
        return;
    }
}
ACCEPT(Ni_AST)

void D_AST::pop(parselib::AST* ast) {
    if (_value.get() == ast) {
        _value.reset();
        return;
    }
}

void D_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
        return;
    }
}
ACCEPT(D_AST)

void Illm_AST::pop(parselib::AST* ast) {
    if (_value.get() == ast) {
        _value.reset();
        return;
    }
}

void Illm_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
        return;
    }
}
ACCEPT(Illm_AST)

void MapKd_AST::pop(parselib::AST* ast) { POP_AST(_value, ast); }
void MapKd_AST::append(parselib::AST* ast) { APPEND_AST(_value, ast); }
ACCEPT(MapKd_AST)

static void common_remove(std::vector<parselib::uAST>& components,
                           parselib::AST* ast) {
    auto iter = std::find_if(components.begin(), components.end(),
        [ast](const parselib::uAST& item) {
            return item.get() == ast;
        }
    );

    if (iter != components.end()) {
        components.erase(components.begin(), iter);
    }
}

void Mtl_AST::pop(parselib::AST* ast) {
    common_remove(_components, ast);
}

void Mtl_AST::append(parselib::AST* ast) {
    _components.push_back(std::move(parselib::uAST(ast)));
}
ACCEPT(Mtl_AST)

void Wrapper_AST::pop(parselib::AST* ast) {
    common_remove(_components, ast);
}

void Wrapper_AST::append(parselib::AST* ast) {
    _components.push_back(std::move(parselib::uAST(ast)));
}
ACCEPT(Wrapper_AST)

#undef ACCEPT

parselib::Parser& integer_parser() {
    static parselib::Parser impl = parselib::Atom(LexemType::INTEGER);
    return impl.on_accept(parselib::primary_type_builder<Int_AST>());
}

parselib::Parser& float_parser() {
    static parselib::Parser impl = parselib::Atom(LexemType::FLOAT);
    return impl.on_accept(parselib::primary_type_builder<Float_AST>());
}

parselib::Parser& string_parser() {
    static parselib::Parser impl = parselib::Atom(LexemType::STRING);
    return impl.on_accept(parselib::primary_type_builder<String_AST>());
}

static parselib::Atom& new_mtl() {
    static parselib::Atom impl(LexemType::NEW_MTL);
    return impl;
}

parselib::Parser& new_mtl_parser() {
    static parselib::Parser impl(new_mtl() + string_parser());
    return parselib::bind<NewMtl_AST>(impl);
}

static parselib::Parser& vec3f_parser() {
    static parselib::Parser impl(float_parser() + float_parser() + float_parser());
    return parselib::bind<Vector3f_AST>(impl);
}

parselib::Atom& ka() {
    static parselib::Atom impl(LexemType::KA);
    return impl;
}

parselib::Parser& ka_parser() {
    static parselib::Parser impl(ka() + vec3f_parser());
    return parselib::bind<Ka_AST>(impl);
}

static parselib::Atom& kd() {
    static parselib::Atom impl(LexemType::KD);
    return impl;
}

parselib::Parser& kd_parser() {
    static parselib::Parser impl(kd() + vec3f_parser());
    return parselib::bind<Kd_AST>(impl);
}

parselib::Atom& ks() {
    static parselib::Atom impl(LexemType::KS);
    return impl;
}

parselib::Parser& ks_parser() {
    static parselib::Parser impl(ks() + vec3f_parser());
    return parselib::bind<Ks_AST>(impl);
}

parselib::Atom& ke() {
    static parselib::Atom impl(LexemType::KE);
    return impl;
}

parselib::Parser& ke_parser() {
    static parselib::Parser impl(ke() + vec3f_parser());
    return parselib::bind<Ke_AST>(impl);
}

parselib::Atom& ns() {
    static parselib::Atom impl(LexemType::NS);
    return impl;
}

parselib::Parser& ns_parser() {
    static parselib::Parser impl(ns() + float_parser());
    return parselib::bind<Ns_AST>(impl);
}

parselib::Atom& ni() {
    static parselib::Atom impl(LexemType::NI);
    return impl;
}

parselib::Parser& ni_parser() {
    static parselib::Parser impl(ni() + float_parser());
    return parselib::bind<Ni_AST>(impl);
}

parselib::Atom& d() {
    static parselib::Atom impl(LexemType::D);
    return impl;
}

parselib::Parser& d_parser() {
    static parselib::Parser impl(d() + float_parser());
    return parselib::bind<D_AST>(impl);
}

parselib::Atom& illum() {
    static parselib::Atom impl(LexemType::ILLUM);
    return impl;
}

parselib::Parser& illum_parser() {
    static parselib::Parser impl(illum() + integer_parser());
    return parselib::bind<Illm_AST>(impl);
}

parselib::Atom& map_kd() {
    static parselib::Atom impl(LexemType::MAP_KD);
    return impl;
}

parselib::Parser& map_kd_parser() {
    static parselib::Parser impl(map_kd() + string_parser());
    return parselib::bind<MapKd_AST>(impl);
}

static parselib::Parser& entry() {
    static parselib::Parser impl(
        ka_parser()      |
        new_mtl_parser() |
        kd_parser()      |
        ks_parser()      |
        ke_parser()      |
        ns_parser()      |
        ni_parser()      |
        d_parser()       |
        illum_parser()   |
        map_kd_parser()
    );
    return impl;
}

parselib::Parser& mtl_parser() {
    static parselib::Parser impl(parselib::one_or_more(entry()));
    return parselib::bind<Mtl_AST>(impl);
}


parselib::Lexems mtl_lexems(const std::filesystem::path path) {
    
}

}
