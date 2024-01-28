#include "opengl_proc.hpp"


namespace opengl {

buffer_bind_guard::buffer_bind_guard(VertexArrayBindCommand&& cmd)
    : mode_(VAO_BINDER)
{
    SAFE_CALL(glBindVertexArray(cmd.vao))
}

buffer_bind_guard::buffer_bind_guard(BufferBindCommand&& cmd)
    : mode_(VBO_BINDER)
    , buff_type_(cmd.type)
{
    SAFE_CALL(glBindBuffer(cmd.type, cmd.id));
}

buffer_bind_guard::~buffer_bind_guard() {
    switch (mode_) {
    case VAO_BINDER: {
        SAFE_CALL(glBindVertexArray(0));
        break;
    } case VBO_BINDER: {
        SAFE_CALL(glBindBuffer(buff_type_, 0));
    } default:;
    }
}

}
