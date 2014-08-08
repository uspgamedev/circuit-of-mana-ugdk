
#include "tilemap.h"
#include "renderer.h"

#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>

using ugdk::system::Configuration;
using circuit::TileMap;
using circuit::Renderer;

int main(int argc, char* argv[]) {
    Configuration config;
    config.base_path = "assets/";
    assert(ugdk::system::Initialize(config));
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    ourscene->set_render_function(Renderer(TileMap::Create("sample")));
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

