#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp>

#include <Loader/parser.hpp>

#include "mtl_printer.hpp"



class Test_MTL_Parsers : public testing::Test {
public:
    Test_MTL_Parsers()
        : testing::Test()
        , lexer(loader::create_rules())
        , printer(std::cout)
    {}

    void print(auto* ast) {
        ast->accept(&printer);
    }

public:
    parselib::Lexer lexer;
    MtlPrinter printer;
};

TEST_F(Test_MTL_Parsers, floats) {
    auto lexems = lexer.tokenize("1.00000");
    ASSERT_TRUE(lexems.size() == 1);
    ASSERT_TRUE(lexems[0].tag == loader::LexemType::FLOAT);
}

TEST_F(Test_MTL_Parsers, ka_tree) {
    auto lexems = lexer.tokenize("Ka 1.000000 1.000000 1.000000");
    ASSERT_TRUE(lexems.size() == 4);
    ASSERT_EQ(lexems[0].tag, loader::LexemType::KA);
    ASSERT_EQ(lexems[1].tag, loader::LexemType::FLOAT);
    ASSERT_EQ(lexems[2].tag, loader::LexemType::FLOAT);
    ASSERT_EQ(lexems[3].tag, loader::LexemType::FLOAT);
    parselib::Driver grammar(loader::ka_parser());
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, kd_pattern) {
    std::string line("Kd 0.756863 0.756863 0.756863");
    parselib::Lexer lexer(loader::create_rules());
    auto lexems = lexer.tokenize(line);
    ASSERT_EQ(lexems.size(), 4);
    parselib::Driver grammar(loader::kd_parser());
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, ks_pattern) {
    auto lexems = lexer.tokenize("Ks 0.500000 0.500000 0.500000");
    parselib::Driver grammar(loader::ks_parser());
    ASSERT_EQ(lexems.size(), 4);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, ni_pattern) {
    auto lexems = lexer.tokenize("Ni 1.450000");
    parselib::Driver grammar(loader::ni_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST_F(Test_MTL_Parsers, ns_pattern) {
    auto lexems = lexer.tokenize("Ns 19.09");
    parselib::Driver grammar(loader::ns_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST_F(Test_MTL_Parsers, d_pattern) {
    auto lexems = lexer.tokenize("d 1.0000");
    parselib::Driver grammar(loader::d_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST_F(Test_MTL_Parsers, newmtl_pattern) {
    auto lexems = lexer.tokenize("newmtl lambert1.001");
    parselib::Driver grammar(loader::new_mtl_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST_F(Test_MTL_Parsers, illum_pattern) {
    auto lexems = lexer.tokenize("illum 2");
    parselib::Driver grammar(loader::illum_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

//TEST_F(Test_MTL_Parsers, mtl_document) {
//    auto lexems = lexer.tokenize(
//        "# Blender MTL File: 'None'\n"
//        "# Material Count : 1\n"
//        "newmtl Material\n"
//        "Ns 359.999993\n"
//        "Ka 1.000000 1.000000 1.000000\n"
//        "Kd 0.800000 0.800000 0.800000\n"
//        "Ks 0.500000 0.500000 0.500000\n"
//        "Ke 0.000000 0.000000 0.000000\n"
//        "Ni 1.450000\n"
//        "d 1.000000\n"
//        "illum 2\n"
//    );
//    ASSERT_EQ(lexems.size(), 26);
//    ASSERT_EQ(lexems[0].tag, loader::LexemType::NEW_MTL);
//    ASSERT_EQ(lexems[lexems.size() - 1].tag, loader::LexemType::INTEGER);
//
//    parselib::Driver grammar(loader::mtl_parser());
//    auto* ast = new loader::Wrapper_AST;
//    ASSERT_TRUE(grammar.accept(lexems, ast));
//    MtlPrinter printer(std::cout);
//    ast->accept(&printer);
//    delete ast;
//}
