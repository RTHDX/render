#include <filesystem>
#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/opengl.hpp>
#include <UI/ui.hpp>
#include <UI/property.hpp>
#include <RTX/rtx.hpp>
#include <UI-Nuklear/rtx_application.hpp>
#include <UI-Nuklear/nuklear_widgets.hpp>

size_t WIDTH = 860;
size_t HEIGHT = 680;

auto create_camera() {
    return rtx::Camera(
        {2.0, 2.0, 2.0},
        {0.0, 0.0, 0.0},
        glm::radians(60.0f),
        WIDTH, HEIGHT
    );
}

auto mirror() {
    return rtx::Material{
        {0.0, 0.5, 0.1, 0.8},
        {0.6, 0.7, 0.8},
        125.0,
        1.5
    };
}

auto red_rubber(const rtx::Color& color={0.3f, 0.1f, 0.1f}) {
    return rtx::Material{
        {0.9, 0.1, 0.0, 0.0},
        color,
        10.0f,
        1.0
    };
}

std::string current_dir() {
    std::wstring temp(std::filesystem::current_path().c_str());
    return std::string(temp.begin(), temp.end());
}

std::vector<rtx::Light> create_lights() {
    return {
        rtx::Light {
            {0.0, 30.0, 0.0},
            4.0
        }
    };
}

std::vector<rtx::Mesh> create_meshes() {
    rtx::model::Loader loader;
    std::string path(std::format("{}/{}", current_dir(), "stone_5.obj"));
    auto meshes = loader.read(path);
    for (auto& mesh : meshes) {
        for (auto& face : mesh.faces) {
            face.material = red_rubber();
        }
    }

    return meshes;
}

rtx::Mesh create_ground() {
    rtx::Mesh ground;
    ground.faces = {
        rtx::Triangle(
            {-100,  -10, 100},
            { 100,  -10, 100},
            { 100,  -10, -100},
            {0, 1, 0},
            red_rubber({0.1, 0.1, 0.1})
        ),
        rtx::Triangle(
            {-100, -10, -100},
            {-100, -10, -100},
            { 100, -10, -100},
            {0, 1, 0},
            red_rubber({0.1, 0.1, 0.1})
        )
    };
    return ground;
}

auto create_scene() {
    auto objects = create_meshes();
    return rtx::Scene<rtx::Mesh>(
        std::move(objects),
        std::move(create_lights())
    );
}

auto create_render() {
    rtx::Color back{ 0.5, 0.7, 0.7 };

#if(DEBUG)
    return rtx::Render<rtx::Mesh> (std::move(create_scene()),
                                   std::move(create_camera()),
                                   back);
#else
    return rtx::MultiThreadRender<rtx::Mesh> (std::move(create_scene()),
                                              std::move(create_camera()),
                                              back);
#endif
}

int main() {
    ui::nuklear::RTX_App app("Meshes", WIDTH, HEIGHT);
    app.initialize();
    auto render = create_render();
    app.run(render);
    return 0;
}
