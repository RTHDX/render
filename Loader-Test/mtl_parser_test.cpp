#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp>

#include <Loader/mtl_parser.hpp>


class Test_MTL_Parsers : public testing::Test {
public:
    Test_MTL_Parsers()
        : testing::Test()
        , lexer(loader::create_rules())
#if(DEBUG_OUT)
        , printer(std::cout)
#endif
    {}

    void print(auto* ast) {
#if(DEBUG_OUT)
        ast->accept(&printer);
#endif
        (void*)ast;
    }

public:
    parselib::Lexer lexer;
#if(DEBUG_OUT)
    loader::MtlPrinter printer;
#endif
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
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, ns_pattern) {
    auto lexems = lexer.tokenize("Ns 19.09");
    parselib::Driver grammar(loader::ns_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, d_pattern) {
    auto lexems = lexer.tokenize("d 1.0000");
    parselib::Driver grammar(loader::d_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, newmtl_pattern) {
    auto lexems = lexer.tokenize("newmtl lambert1.001");
    parselib::Driver grammar(loader::new_mtl_parser());
    ASSERT_EQ(lexems.size(), 2);
    ASSERT_EQ(lexems[0].tag, loader::LexemType::NEW_MTL);
    ASSERT_EQ(lexems[1].tag, loader::LexemType::STRING);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, illum_pattern) {
    auto lexems = lexer.tokenize("illum 2");
    parselib::Driver grammar(loader::illum_parser());
    ASSERT_EQ(lexems.size(), 2);
    auto* ast = new loader::Mtl_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, mtl_document_1) {
    auto lexems = lexer.tokenize("Ka 1.000000 1.000000 1.000000");
    ASSERT_EQ(lexems.size(), 4);

    parselib::Driver grammar(loader::mtl_parser());
    auto* ast = new loader::Wrapper_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, mtl_document_2) {
    auto lexems = lexer.tokenize(
        "newmtl Material\n"
        "Ns 359.999993\n"
    );
    //ASSERT_EQ(lexems.size(), 4);
    parselib::Driver grammar(loader::mtl_parser());
    auto* ast = new loader::Wrapper_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, mtl_document_3) {
    auto lexems = lexer.tokenize(
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

    parselib::Driver grammar(loader::mtl_parser());
    auto* ast = new loader::Wrapper_AST;
    ASSERT_TRUE(grammar.accept(lexems, ast));
    print(ast);
    delete ast;
}

TEST_F(Test_MTL_Parsers, build_material) {
    loader::Material actual = loader::read_material("./mesh-examples/stone_1.mtl");
}
