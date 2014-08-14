
#include "view/stagerenderer.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/structure/types.h>
#include "view/tilemap.h"

namespace {

using ugdk::Color;
using ugdk::graphic::Canvas;
using std::unique_ptr;
using std::vector;

} // unnamed namespace

namespace circuit {
namespace view {

StageRenderer::StageRenderer(unique_ptr<TileMap>&& the_tilemap)
    : tilemap_(std::move(the_tilemap)) {}

void StageRenderer::Render(Canvas& canvas, const vector<Body::Ptr>& bodies) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap_->Render(canvas);
    blank_.Render(canvas, bodies);
}

} // namespace view
} // namespace circuit

