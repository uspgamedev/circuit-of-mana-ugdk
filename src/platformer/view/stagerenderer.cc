
#include "view/stagerenderer.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/structure/types.h>
#include "view/tilemap.h"

namespace circuit {
namespace view {

namespace {

using model::Body;
using ugdk::Color;
using ugdk::graphic::Canvas;
using std::unique_ptr;
using std::vector;

} // unnamed namespace

StageRenderer::StageRenderer(unique_ptr<TileMap>&& the_tilemap)
    : tilemap_(std::move(the_tilemap)) {}

void StageRenderer::Render(Canvas& canvas, const vector<Body::Ptr>& bodies,
                           const Body::Ptr& mage) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap_->Render(canvas);
    blank_.Render(canvas, bodies);
    sprite_.Render(canvas, vector<Body::Ptr>(1u, mage));
}

} // namespace view
} // namespace circuit

