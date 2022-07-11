#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp>

#include <Loader/parser.hpp>

auto create_lexer() {
    return parselib::Lexer (loader::create_rules());
}

TEST(float_parser, test) {
    std::string line("1.00000");
    parselib::Lexer lexer = create_lexer();
    auto lexems = lexer.tokenize(line);
    ASSERT_TRUE(lexems.size() == 1);
    ASSERT_TRUE(lexems[0].tag == loader::LexemType::FLOAT);
}

TEST(ka_parser, test) {
    std::string line("Ka 1.000000 1.000000 1.000000");
    parselib::Lexer lexer = create_lexer();
    auto lexems = lexer.tokenize(line);
    ASSERT_TRUE(lexems.size() == 4);
    ASSERT_EQ(lexems[0].tag, loader::LexemType::KA);
    ASSERT_EQ(lexems[1].tag, loader::LexemType::FLOAT);
    ASSERT_EQ(lexems[2].tag, loader::LexemType::FLOAT);
    ASSERT_EQ(lexems[3].tag, loader::LexemType::FLOAT);
    parselib::Driver grammar(loader::ka_parser());
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(kd_pattern, test) {
    std::string line("Kd 0.756863 0.756863 0.756863");
    parselib::Lexer lexer(loader::create_rules());
    auto lexems = lexer.tokenize(line);
    ASSERT_EQ(lexems.size(), 4);
    parselib::Driver grammar(loader::kd_parser());
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(ks_pattern, test) {
    std::string line("Ks 0.500000 0.500000 0.500000");
    parselib::Lexer lexer = create_lexer();
    auto lexems = lexer.tokenize(line);
    parselib::Driver grammar(loader::ks_parser());
    ASSERT_EQ(lexems.size(), 4);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(ni_pattern, test) {
    std::string line("Ni 1.450000");
    parselib::Lexer lexer = create_lexer();
    auto lexems = lexer.tokenize(line);
    parselib::Driver grammar(loader::ni_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(ns_pattern, test) {
    parselib::Lexer lexer = create_lexer();
    auto lexems = lexer.tokenize("Ns 19.09");
    parselib::Driver grammar(loader::ns_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(d_pattern, test) {
    auto lexems = create_lexer().tokenize("d 1.0000");
    parselib::Driver grammar(loader::d_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(newmtl_pattern, test) {
    auto lexems = create_lexer().tokenize("newmtl lambert1.001");
    parselib::Driver grammar(loader::new_mtl_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(illum_pattern, test) {
    auto lexems = create_lexer().tokenize("illum 2");
    parselib::Driver grammar(loader::illum_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}

TEST(mtl_document, test) {
    auto lexems = create_lexer().tokenize(
        "# Blender MTL File: 'None'\n"
        "# Material Count : 1\n"
        "newmtl Material\n"
        "Ns 359.999993\n"
        "Ka 1.000000 1.000000 1.000000\n"
        "Kd 0.800000 0.800000 0.800000\n"
        "Ks 0.500000 0.500000 0.500000\n"
        "Ke 0.000000 0.000000 0.000000\n"
        "Ni 1.450000\n"
        "d 1.000000\n"
        "illum 2\n"
    );
    ASSERT_EQ(lexems.size(), 26);
    ASSERT_EQ(lexems[0].tag, loader::LexemType::NEW_MTL);
    ASSERT_EQ(lexems[lexems.size() - 1].tag, loader::LexemType::INTEGER);

    parselib::Driver grammar(loader::mtl_parser());
    auto* ast = new loader::Wrapper_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    delete ast;
}
