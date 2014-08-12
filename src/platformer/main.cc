
#include "tilemap.h"
#include "body.h"

#include <functional>
#include <memory>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/engine.h>
#include <ugdk/system/task.h>

using circuit::TileMap;
using circuit::Body;
using ugdk::Color;
using ugdk::graphic::Canvas;
using ugdk::math::Vector2D;
using ugdk::system::Configuration;
using ugdk::system::Task;
using std::unique_ptr;

namespace {

const double MAGE_SPEED = 4.0;

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

Body::Ptr mage;

TileMap::Ptr tilemap;

void Rendering(Canvas& canvas) {
    canvas.Clear(Color(.4, .2, .2));
    tilemap->Render(canvas);
    mage->Render(canvas);
}

void MoveMageTask(double dt) {
    auto input = ugdk::input::manager();
    if (input->keyboard().IsDown(ugdk::input::Scancode::UP))
      mage->AddSpeed(MAGE_SPEED*Vector2D(0.0, -1.0));
    if (input->keyboard().IsDown(ugdk::input::Scancode::DOWN))
      mage->AddSpeed(MAGE_SPEED*Vector2D(0.0, 1.0));
    if (input->keyboard().IsDown(ugdk::input::Scancode::RIGHT))
      mage->AddSpeed(MAGE_SPEED*Vector2D(1.0, 0.0));
    if (input->keyboard().IsDown(ugdk::input::Scancode::LEFT))
      mage->AddSpeed(MAGE_SPEED*Vector2D(-1.0, 0.0));
    Body::MoveAll(dt);
}

} // unnamed namespace

int main(int argc, char* argv[]) {
    Configuration config;
    config.base_path = "assets/";
    assert(ugdk::system::Initialize(config));
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    mage = Body::Create(Vector2D(2.0, 3.0));
    tilemap = TileMap::Create("sample", data);
    mage->Prepare();
    ourscene->set_render_function(Rendering);
    ourscene->AddTask(Task(MoveMageTask));
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

