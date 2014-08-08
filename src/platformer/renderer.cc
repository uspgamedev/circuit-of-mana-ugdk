
#include "renderer.h"

#include <ugdk/graphic/canvas.h>

namespace circuit {

using ugdk::graphic::Canvas;

Renderer::Renderer(Renderer&& another) {
    tilemap_ = std::move(another.tilemap_);
}

void Renderer::Render(Canvas& canvas) {
    tilemap_->Render(canvas);
}

} // namespace circuit

