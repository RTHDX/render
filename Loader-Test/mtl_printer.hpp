#include <iostream>

#include <Loader/parser.hpp>


class MtlPrinter final : public parselib::Visitor {
public:
    MtlPrinter(std::ostream&);

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

private:
    void print_prefix() const;
    std::string prefix() const;
    void print (const std::string& content);
    void header(const std::string& header);
    void footer();

private:
    std::ostream& _out;
    std::string _prefix;
    size_t _lvl = 0;
};
