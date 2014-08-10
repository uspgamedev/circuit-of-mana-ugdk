
#include "tilemap.h"
#include "renderer.h"

#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>

#include <functional>

using ugdk::system::Configuration;
using circuit::TileMap;
using circuit::Renderer;
using std::bind;
using namespace std::placeholders;

//TileMap::Data data = {
//  4, 5,
//  {
//    0,  1,  2,  3,
//    12, 13, 14, 15,
//    24, 25, 26, 27,
//    36, 37, 38, 39,
//    48, 49, 50, 51
//  }
//};

TileMap::Data data = {
  3, 1,
  {
    0, 1, 2
  }
};

int main(int argc, char* argv[]) {
    Configuration config;
    config.base_path = "assets/";
    assert(ugdk::system::Initialize(config));
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    Renderer renderer(TileMap::Create("sample", data));
    ourscene->set_render_function(bind(&Renderer::Render, &renderer, _1));
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

