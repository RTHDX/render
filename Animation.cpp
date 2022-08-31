#include "Animation.hpp"


namespace render {

Animation::Animation(std::vector<index_t>&& indices)
    : _indices(std::move(indices))
    , _current_index(0)
{}

index_t Animation::current_index() const {
    return _indices[_current_index];
}

void Animation::update() {
    auto temp = _current_index + 1;
    _current_index = temp >= _indices.size() ?
        temp % _indices.size() :
        temp;
}

}
