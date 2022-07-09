#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp>

#include <Loader/mtl_parser.hpp>

TEST(ka_pattern, test) {
    std::string line("Ka 1.000000 1.000000 1.000000");
    const auto& regex = loader::Ka_template();
    std::smatch match_result;
    auto match = std::regex_match(line, match_result, regex);
    ASSERT_TRUE(match);
    glm::vec3 expected(1.0, 1.0, 1.0);
    auto actual = loader::vec3_converter(match_result.str(0));
    ASSERT_EQ(expected, actual);
}

TEST(kd_pattern, test) {
    std::string line("Kd 0.756863 0.756863 0.756863");
    std::smatch result;
    auto match = std::regex_match(line, result, loader::Kd_template());
    ASSERT_TRUE(match);
    glm::vec3 expected(0.756863, 0.756863, 0.756863);
    auto actual = loader::vec3_converter(result.str(0));
    ASSERT_EQ(expected, actual);
}

TEST(ks_pattern, test) {
    std::string line("Ks 0.500000 0.500000 0.500000");
    std::smatch result;
    auto match = std::regex_match(line, result, loader::Ks_template());
    ASSERT_TRUE(match);
    glm::vec3 expected(0.500000, 0.500000, 0.500000);
    auto actual = loader::vec3_converter(result.str(0));
    ASSERT_EQ(expected, actual);
}

TEST(ni_pattern, test) {
    std::string line("Ni 1.450000");
    std::smatch result;
    auto match = std::regex_match(line, result, loader::Ni_template());
    ASSERT_TRUE(match);
    ASSERT_TRUE(
        std::fabs(1.450000 - loader::float_converter(result.str(0))) <
        std::numeric_limits<float>::epsilon()
    );
}

TEST(ns_pattern, test) {
    std::string line("Ns 19.09");
    std::smatch res;
    auto match = std::regex_match(line, res, loader::Ns_template());
    ASSERT_TRUE(match);
    float actual = loader::float_converter(res.str(0));
    ASSERT_TRUE(std::fabs(19.09 - actual) < 0.001);
}

TEST(d_pattern, test) {
    std::string line("d 1.0000");
    std::smatch res;
    auto match = std::regex_match(line, res, loader::d_template());
    ASSERT_TRUE(match);
    float actual = loader::float_converter(res.str(0));
    ASSERT_TRUE(std::fabs(1.0f - actual) < std::numeric_limits<float>::epsilon());
}

TEST(newmtl_pattern, test) {
    std::string line("newmtl lambert1.001");
    std::smatch res;
    auto match = std::regex_match(line, res, loader::newmtl_pattern());
    ASSERT_TRUE(match);
    std::string actual = loader::string_converter(res.str(0));
    ASSERT_EQ(std::string("lambert1.001"), std::string(res.str(0)));
}

TEST(illum_pattern, test) {
    std::string line("illum 2");
}
