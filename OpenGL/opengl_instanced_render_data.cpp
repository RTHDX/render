#include "opengl_proc.hpp"
#include "opengl_instanced_render_data.hpp"


namespace opengl {

void instant_render_data_t::free() {
    impl.free();
    if (Context::instance().is_context_active()) {
        for (auto& [name, buff_id] : buffers) {
            SAFE_CALL(glDeleteBuffers(1, &buff_id));
        }
    }
    instance_count = 0;
}

}
