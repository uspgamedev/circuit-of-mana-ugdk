
#include "view/stagerenderer.h"

#include <algorithm>
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
using std::copy_if;
using std::distance;
using std::list;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

} // unnamed namespace

StageRenderer::StageRenderer(unique_ptr<TileMap>&& the_tilemap, TaskPlayer* task_player)
    : tilemap_(std::move(the_tilemap)), sprite_("female-mage-sprite", task_player) {}

void StageRenderer::Render(Canvas& canvas, const list<shared_ptr<Body>>& bodies) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap_->Render(canvas);
    {
        list<shared_ptr<Body>> blanks(bodies.size());
        auto it = copy_if(bodies.begin(), bodies.end(), blanks.begin(),
                [](const shared_ptr<Body>& body) {
                    return body->name().find("mage") == string::npos;
                });
        blanks.resize(distance(blanks.begin(), it));
        blank_.Render(canvas, blanks);
    }
    {
        list<shared_ptr<Body>> mages(bodies.size());
        auto it = copy_if(bodies.begin(), bodies.end(), mages.begin(),
                [](const shared_ptr<Body>& body) {
                    return body->name().find("mage") != string::npos;
                });
        mages.resize(distance(mages.begin(), it));
        sprite_.Render(canvas, mages);
    }
}

} // namespace view
} // namespace circuit

