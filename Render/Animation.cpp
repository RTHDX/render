#include "Animation.hpp"


namespace render {

Animation::Animation()
    : _frames(0, 0)
{}

Animation::Animation(index_t start, index_t end)
    : _frames(start, end)
{}

index_t Animation::current_index() const {
    return *(_frames.begin() + _offset);
}

index_t Animation::total_frames() const {
    return _frames.size();
}

void Animation::update() {
    auto temp = _offset + 1;
    _offset = temp >= _frames.size() ? 0 : temp;
}

}
