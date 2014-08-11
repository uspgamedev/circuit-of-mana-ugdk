
#include "tilemap.h"
#include "body.h"

#include <functional>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/input/events.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/engine.h>

using circuit::TileMap;
using circuit::Body;
using ugdk::Color;
using ugdk::graphic::Canvas;
using ugdk::math::Vector2D;
using ugdk::system::Configuration;

namespace {

TileMap::Data data = {
  7, 7,
  {
    21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21,
    21,  0,  1,  1,  1,  3, 21,
    21, 12, 13, 13, 13, 15, 21,
    21, 24, 25, 25, 25, 27, 21,
    21, 36, 37, 37, 37, 39, 21,
    21, 21, 21, 21, 21, 21, 21,
  }
};

Body mage(Vector2D(2.0, 2.0));

TileMap::Ptr tilemap;

void Rendering(Canvas& canvas) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap->Render(canvas);
    mage.Render(canvas);
}

} // unnamed namespace

int main(int argc, char* argv[]) {
    Configuration config;
    config.base_path = "assets/";
    assert(ugdk::system::Initialize(config));
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    tilemap = TileMap::Create("sample", data);
    mage.Prepare();
    ourscene->set_render_function(Rendering);
    ourscene->event_handler().AddListener<ugdk::input::KeyPressedEvent>(
        [ourscene](const ugdk::input::KeyPressedEvent& ev) {
            if(ev.scancode == ugdk::input::Scancode::ESCAPE)
                ourscene->Finish();
        });
    ugdk::system::PushScene(ourscene);
    ugdk::system::Run();
    ugdk::system::Release();
    return 0;
}

