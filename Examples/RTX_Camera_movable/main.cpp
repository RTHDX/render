#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <UI/ui.hpp>
#include <UI/property.hpp>
#include <RTX/rtx.hpp>

#include <Common/rtx_scene_default.hpp>


int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    

    rtx::MultiThreadRender render(std::move(rtx::make_scene()),
                                  rtx::make_camera(),
                                  {0.8, 0.8, 0.8});

    return EXIT_SUCCESS;
}
