
#include "renderer.h"

#include <ugdk/graphic/canvas.h>

namespace circuit {

using ugdk::graphic::Canvas;

void Renderer::Render(Canvas& canvas) {
    tilemap_->Render(canvas);
}

} // namespace circuit

