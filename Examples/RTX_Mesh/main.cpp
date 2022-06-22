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
#include <UI-Nuklear/nuklear_application.hpp>
#include <UI-Nuklear/nuklear_widgets.hpp>

size_t WIDTH = 860;
size_t HEIGHT = 680;

auto create_camera() {
    return rtx::Camera(
        {0.0, 0.0, 3.0},
        {0.0, 0.0, 0.0},
        glm::radians(90.0f),
        WIDTH, HEIGHT
    );
}

auto ivory() {
    return rtx::Material{
        rtx::Albedo{0.6, 0.3, 0.1, 0.0},
        rtx::Color(0.45, 0.45, 0.35),
        50.0,
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
            20.0
        }
    };
}

auto create_scene() {
    auto meshes = rtx::model::Loader().read(
        std::format(R"({}\{})", current_dir(),
                    R"(cube.obj)")
    );
    for (auto& mesh: meshes) {
        for (auto& face: mesh.faces) {
            face.material = ivory();
        }
    }

    return rtx::Scene<rtx::Mesh> (
        std::move(meshes),
        std::move(create_lights())
    );
}

int main() {
    ui::nuklear::Application app("Meshes", WIDTH, HEIGHT);
    //rtx::MultiThreadRender render(std::move(create_scene()),
    //                              std::move(create_camera()));
    rtx::Render<rtx::Mesh> render(std::move(create_scene()),
                                  std::move(create_camera()));
    std::function<void()> callback = [&render]() {
        render.render();
        const auto& buffer = render.buffer();
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT,
                     glm::value_ptr(*buffer.data()));
    };

    app.run(callback);
    return 0;
}
