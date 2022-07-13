#include <iostream>
#include <typeinfo>

#include "mtl_parser.hpp"


namespace loader {

void local_print(parselib::AST* ast) {
    static MtlPrinter printer(std::cout);
    ast->accept(&printer);
}

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

String_AST::String_AST(const std::string c)
    : parselib::AST()
    , _value(c)
{}
ACCEPT(String_AST)

static inline bool pop_test(parselib::uAST& lhs, const parselib::AST* ast) {
    return (bool(lhs) && bool(ast)) && lhs->id() == ast->id();
}

void Vector3f_AST::pop(parselib::AST* ast) {
    auto ast_id = ast->id();
    if (pop_test(_x, ast)) {
        _x.reset();
    } else if (pop_test(_y, ast)) {
        _y.reset();
    } else if (pop_test(_z, ast)) {
        _z.reset();
    } else {
        std::cerr << "Could not find id " << ast_id << std::endl;
    }
}

void Vector3f_AST::append(parselib::AST* ast) {
    if (!_x) {
        _x.reset(ast);
    } else if (!_y) {
        _y.reset(ast);
    } else if (!_z) {
        _z.reset(ast);
    } else {
        std::cerr << "Could not append ast: " << ast->id() << std::endl;
    }
    local_print(this);
}
ACCEPT(Vector3f_AST)

void NewMtl_AST::pop(parselib::AST* ast) {
    if (pop_test(_name, ast)) {
        _name.reset();
    } else {
        std::cerr << "Could not find id: " << ast->id() << std::endl;
    }
    local_print(this);
}

void NewMtl_AST::append(parselib::AST* ast) {
    if (!_name) {
        _name.reset(ast);
    } else {
        std::cerr << "Could not append to filled tree" << std::endl;
    }
    local_print(this);
}
ACCEPT(NewMtl_AST)

void Ka_AST::pop(parselib::AST* ast) {
    if (pop_test(_vector, ast)) {
        _vector.reset();
    } else {
        std::cerr << "Could not find: " << ast->id() << std::endl;
    }
    local_print(this);
}

void Ka_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
    } else {
        std::cerr << "Could not append to filled tree" << std::endl;
    }
    local_print(this);
}
ACCEPT(Ka_AST)

void Kd_AST::pop(parselib::AST* ast) {
    if (pop_test(_vector, ast)) {
        _vector.reset();
    } else {
        std::cerr << "Could not find ast" << std::endl;
    }
    local_print(this);
}

void Kd_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(Kd_AST)

void Ke_AST::pop(parselib::AST* ast) {
    if (pop_test(_vector, ast)) {
        _vector.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}
void Ke_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(Ke_AST)

void Ks_AST::pop(parselib::AST* ast) {
    if (pop_test(_vector, ast)) {
        _vector.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}

void Ks_AST::append(parselib::AST* ast) {
    if (!_vector) {
        _vector.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(Ks_AST)

void Ns_AST::pop(parselib::AST* ast) {
    if (pop_test(_value, ast)) {
        _value.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}

void Ns_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(Ns_AST)

void Ni_AST::pop(parselib::AST* ast) {
    if (pop_test(_value, ast)) {
        _value.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}

void Ni_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(Ni_AST)

void D_AST::pop(parselib::AST* ast) {
    if (pop_test(_value, ast)) {
        _value.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}

void D_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(D_AST)

void Illm_AST::pop(parselib::AST* ast) {
    if (pop_test(_value, ast)) {
        _value.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}

void Illm_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
    local_print(this);
}
ACCEPT(Illm_AST)

void MapKd_AST::pop(parselib::AST* ast) {
    if (pop_test(_value, ast)) {
        _value.reset();
    } else {
        std::cerr << "Could not find " << ast->id() << std::endl;
    }
    local_print(this);
}
void MapKd_AST::append(parselib::AST* ast) {
    if (!_value) {
        _value.reset(ast);
    } else {
        std::cerr << "Could not append to filled ast" << std::endl;
    }
}
ACCEPT(MapKd_AST)

static void common_remove(std::vector<parselib::uAST>& components,
                           parselib::AST* ast) {
    auto iter = std::remove_if(components.begin(), components.end(),
        [ast](const parselib::uAST& item) {
            return item->id() == ast->id();
        }
    );
    components.erase(iter, components.end());
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


parselib::Parser integer_parser() {
    parselib::Parser impl = parselib::Atom(LexemType::INTEGER);
    return parselib::bind_primary<Int_AST>(impl);
}

parselib::Parser float_parser() {
    parselib::Parser impl = parselib::Atom(LexemType::FLOAT);
    return parselib::bind_primary<Float_AST>(impl);
}

parselib::Parser string_parser() {
    parselib::Parser impl = parselib::Atom(LexemType::STRING);
    return parselib::bind_primary<String_AST>(impl);
}

parselib::Parser new_mtl_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::NEW_MTL) + string_parser());
    return parselib::bind<NewMtl_AST>(impl);
}

static parselib::Parser vec3f_parser() {
    parselib::Parser impl(float_parser() + float_parser() + float_parser());
    return parselib::bind<Vector3f_AST>(impl);
}

parselib::Parser ka_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::KA) + vec3f_parser());
    return parselib::bind<Ka_AST>(impl);
}

parselib::Parser kd_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::KD) + vec3f_parser());
    return parselib::bind<Kd_AST>(impl);
}

parselib::Parser ks_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::KS) + vec3f_parser());
    return parselib::bind<Ks_AST>(impl);
}

parselib::Parser ke_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::KE) + vec3f_parser());
    return parselib::bind<Ke_AST>(impl);
}

parselib::Parser ns_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::NS) + float_parser());
    return parselib::bind<Ns_AST>(impl);
}


parselib::Parser ni_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::NI) + float_parser());
    return parselib::bind<Ni_AST>(impl);
}

parselib::Parser d_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::D) + float_parser());
    return parselib::bind<D_AST>(impl);
}

parselib::Parser illum_parser() {
    parselib::Parser impl(parselib::Atom(LexemType::ILLUM)+ integer_parser());
    return parselib::bind<Illm_AST>(impl);
}

parselib::Parser map_kd_parser() {
    parselib::Parser impl(parselib::Atom{LexemType::MAP_KD} + string_parser());
    return parselib::bind<MapKd_AST>(impl);
}

static parselib::Parser entry() {
    parselib::Parser impl(
        new_mtl_parser() |
        kd_parser()      |
        ks_parser()      |
        ke_parser()      |
        ns_parser()      |
        ni_parser()      |
        d_parser()       |
        illum_parser()   |
        ka_parser()      |
        map_kd_parser()
    );
    return impl.name("Entry_Parser");
}

parselib::Parser mtl_parser() {
    parselib::Parser impl(parselib::one_or_more(entry()));
    return parselib::bind<Mtl_AST>(impl, "mtl_parser");
}


MtlPrinter::MtlPrinter(std::ostream& out)
    : parselib::Visitor()
    , _out(out)
    , _prefix("  ")
{}

void MtlPrinter::visit(const loader::Int_AST& ast) {
    _lvl += 1;
    print_prefix();
    _out << "<int " << ast.value() << ">" << std::endl;
    _lvl -= 1;
}

void MtlPrinter::visit(const loader::Float_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<float " << ast.value() << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::Vector3f_AST& ast) {
    header("<vec3f ");
    _out << std::endl;
    if (ast.x()) { ast.x()->accept(this); }
    if (ast.y()) { ast.y()->accept(this); }
    if (ast.z()) { ast.z()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::String_AST& ast) {
    header("<string ");
    _out << ast.value();
    footer(true);
}

void MtlPrinter::visit(const loader::NewMtl_AST& ast) {
    header("<new mtl ");
    _out << std::endl;
    if (ast.name()) { ast.name()->accept(this); }
    _out << std::endl;
    footer();
}

void MtlPrinter::visit(const loader::Ka_AST& ast) {
    header("<Ka "); _out << std::endl;
    if (ast.vector()) { ast.vector()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Kd_AST& ast) {
    header("<Kd "); _out << std::endl;
    if (ast.vector()) { ast.vector()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Ks_AST& ast) {
    header("<Ks "); _out << std::endl;
    if (ast.vector()) { ast.vector()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Ke_AST& ast) {
    header("<Ke ");
    if (ast.vector()) { ast.vector()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Ns_AST& ast) {
    header("<Ns ");
    if (ast.value()) { ast.value()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Ni_AST& ast) {
    header("<Ni ");
    if (ast.value()) { ast.value()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::D_AST& ast) {
    header("<d ");
    if (ast.value()) { ast.value()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Illm_AST& ast) {
    header("<illum ");
    if (ast.value()) { ast.value()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::MapKd_AST& ast) {
    header("<Map_Kd ");
    if (ast.value()) { ast.value()->accept(this); }
    footer();
}

void MtlPrinter::visit(const loader::Mtl_AST& ast) {
    header("<Mtl "); _out << std::endl;
    for (const parselib::uAST& child : ast.components()) {
        child->accept(this);
    }
    footer();
}

void MtlPrinter::visit(const loader::Wrapper_AST& ast) {
    header("<doc "); _out << std::endl;
    for (const parselib::uAST& child : ast.components()) {
        child->accept(this);
    }
    footer();
}

void MtlPrinter::print_prefix() const {
    for (size_t i = 0; i < _lvl; ++i) { _out << _prefix; }
}

std::string MtlPrinter::prefix() const {
    std::string pref;
    for (size_t i = 0; i < _lvl; ++i) {
        pref += _prefix;
    }
    return pref;
}

void MtlPrinter::print(const std::string& content) {
    _out << prefix() << content;
}

void MtlPrinter::header(const std::string& header) {
    _lvl++;
    print(header);
}

void MtlPrinter::footer(bool silent) {
    if (!silent) { print(">"); }
    else { _out << ">"; }
    _lvl--;
    _out << std::endl;
}

}
