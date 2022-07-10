#include "parser.hpp"


namespace loader {

parselib::Rules create_rules() {
    return parselib::Rules {
        {"\d+", LexemType::INTEGER},
        {"\d+\.\d+", LexemType::FLOAT},

        {"newmtl", LexemType::NEW_MTL},
        {"Ka", LexemType::KA},
        {"Kd", LexemType::KD},
        {"Ks", LexemType::KS},
        {"Ns", LexemType::NS},
        {"Ni", LexemType::NI},
        {"d", LexemType::D},
        {"illum", LexemType::ILLUM},
        {"map_Kd", LexemType::MAP_KD},

        {".+", LexemType::STRING},
    };
}

Int_AST::Int_AST(const std::string& c)
    : parselib::AST()
    , _value(std::atoi(c))
{}

Float_AST::Float_AST(const std::string& c)
    : parselib::AST()
    , _value(std::atof(c))
{}

String_AST::String_AST(const std::string& c)
    : parselib::AST()
    , _value(c)
{}


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

static parselib::Atom& new_mtl_parser() {
    static parselib::Atom impl(LexemType::NEW_MTL);
    return impl;
}

parselib::Parser& new_mtl_parser() {
    static parselib::Parser impl(new_mtl_parser() + string_parser());
    
}

}
