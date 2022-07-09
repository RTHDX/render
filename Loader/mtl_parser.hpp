#pragma once

#include <regex>
#include <string>
#include <format>
#include <sstream>

#include <glm/glm.hpp>

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

}
