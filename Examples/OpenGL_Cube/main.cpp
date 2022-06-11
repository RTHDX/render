#include <ui.hpp>
#include <opengl.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* window = ui::create_window(WIDTH, HEIGHT, "Cube");

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background({0.2f, 0.5f, 0.5f, 1.0f});

    return EXIT_SUCCESS;
}
