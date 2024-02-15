#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

#include <UI/io.hpp>

struct LocalListener final : public ui::Listener {
    LocalListener() : ui::Listener(&ui::io::IO::instance()) {}
    ~LocalListener() override = default;

    void consume(const ui::KeyEvent& event) override {
        std::cout << event << std::endl;
        
    }

    void consume(const ui::MouseEvent& event) override {
        //std::cout << event << std::endl;
    }

    void consume(const ui::MouseButtonEvent& event) override {
        std::cout << event << std::endl;
    }

    void consume(const ui::ScrollEvent& event) override {
        std::cout << event << std::endl;
        if (!camera) { return; }
        if (event.yoffset > 0) {
            //camera->zoom_in();
        } else {
            //camera->zoom_out();
        }
    }

    void consume(const ui::DropEvent& event) override {
        std::cout << event << std::endl;
    }

    void consume(const ui::FramebufferEvent& event) override {
        std::cout << event << std::endl;
        opengl::viewport(event.width, event.height);
        if (camera) {
            camera->update_viewport({event.width, event.height});
        }
    }

    GLFWwindow* win = nullptr;
    opengl::Camera* camera = nullptr;

private:
    bool is_press(int action) {
        return action == GLFW_PRESS;
    }
};
