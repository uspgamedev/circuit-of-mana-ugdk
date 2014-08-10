
#include "renderer.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/structure/types.h>

namespace circuit {

using ugdk::graphic::Canvas;
using ugdk::Color;

Renderer::Renderer(Renderer&& another) {
    tilemap_ = std::move(another.tilemap_);
}

void Renderer::Render(Canvas& canvas) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap_->Render(canvas);
}

} // namespace circuit

