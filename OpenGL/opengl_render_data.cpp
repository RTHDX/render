#include "opengl_proc.hpp"
#include "opengl_render_data.hpp"


namespace opengl {

void RenderData::free() {
    if (Context::instance().is_context_active()) {
        SAFE_CALL(glDeleteBuffers(1, &ebo));
        SAFE_CALL(glDeleteBuffers(vertex_buffers.size(),
                                  vertex_buffers.data()));
        SAFE_CALL(glDeleteVertexArrays(1, &vao));
        SAFE_CALL(glDeleteProgram(program));
    }
    ebo = 0;
    vertex_buffers.clear();
    vao = 0;
    stencil_ref = -1;
    ebo_count = 0;
}

}
