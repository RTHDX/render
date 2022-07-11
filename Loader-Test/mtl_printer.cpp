#include "mtl_printer.hpp"


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
    _lvl++;
    print("<vec3f ");
    _out << std::endl;
    ast.x()->accept(this);
    ast.y()->accept(this);
    ast.z()->accept(this);
    print(">"); _out << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::String_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<string " << ast.value() << ">";
    _lvl--;
}

void MtlPrinter::visit(const loader::NewMtl_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<new mtl: " << ast.name() << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::Ka_AST& ast) {
    header("<Ka "); _out << std::endl;
    ast.vector()->accept(this);
    footer();
}

void MtlPrinter::visit(const loader::Kd_AST& ast) {
    header("<Kd "); _out << std::endl;
    ast.vector()->accept(this);
    footer();
}

void MtlPrinter::visit(const loader::Ks_AST& ast) {
    header("<Ks "); _out << std::endl;
    ast.vector()->accept(this);
    footer();
}

void MtlPrinter::visit(const loader::Ke_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<Ke "; ast.vector()->accept(this); _out << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::Ns_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<Ns "; ast.value()->accept(this); _out << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::Ni_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<Ni "; ast.value()->accept(this); _out << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::D_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<d "; ast.value()->accept(this); _out << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::Illm_AST& ast) {
    _lvl++;
    print_prefix();
    _out << "<illum "; ast.value()->accept(this); _out << ">" << std::endl;
    _lvl--;
}

void MtlPrinter::visit(const loader::MapKd_AST& ast) {
    header("<Map_Kd ");
    ast.value()->accept(this);
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

void MtlPrinter::footer() {
    print(">");
    _lvl--;
    _out << std::endl;
}


