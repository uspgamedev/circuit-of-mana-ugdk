
#include "view/stagerenderer.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/taskplayer.h>
#include "view/tilemap.h"

namespace circuit {
namespace view {

namespace {

using model::Body;
using ugdk::Color;
using ugdk::graphic::Canvas;
using ugdk::system::TaskPlayer;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;

} // unnamed namespace

StageRenderer::StageRenderer(unique_ptr<TileMap>&& the_tilemap, TaskPlayer* task_player)
    : tilemap_(std::move(the_tilemap)), sprite_("female-mage-sprite", task_player) {}

void StageRenderer::Render(Canvas& canvas, const vector<shared_ptr<Body>>& bodies,
                           const shared_ptr<Body>& mage) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap_->Render(canvas);
    blank_.Render(canvas, bodies);
    sprite_.Render(canvas, vector<shared_ptr<Body>>(1u, mage));
}

} // namespace view
} // namespace circuit

