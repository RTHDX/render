#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>

int WIDTH = 800;
int HEIGHT = 600;

void process_cuda_error(cudaError_t error_code,
                        const char* file,
                        int line);
#define CUDA_SAFE_CALL(CALL)\
    

void init_cuda_device();

int main() {
    if (!ui::init_glfw_lite()) {
        return EXIT_FAILURE;
    }
    auto* window = ui::create_window(WIDTH, HEIGHT, "cuda + opengl");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}


void init_cuda_device() {
    int dev_count;
}

void process_cuda_error(cudaError_t error_code,
                        const char* file,
                        int line) {
    if (error_code == cudaSuccess) { return; }

    std::cout << "Some error at [" << file << ":" << line << std::endl;
}
