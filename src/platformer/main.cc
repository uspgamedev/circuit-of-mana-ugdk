
#include "tilemap.h"
#include "body.h"
#include "view/stagerenderer.h"

#include <ctime>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <libjson.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/text/textmanager.h>
#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/structure/types.h>
#include <ugdk/structure/box.h>
#include <ugdk/system/engine.h>
#include <ugdk/system/task.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>

using circuit::TileMap;
using circuit::Body;
using pyramidworks::collision::CollisionManager;
using ugdk::Color;
using ugdk::graphic::Canvas;
using ugdk::math::Vector2D;
using ugdk::structure::Box;
using ugdk::system::Configuration;
using ugdk::system::Task;
using std::unique_ptr;
using std::vector;

namespace {

const double MAGE_SPEED = 15.0;
const double FRAME_TIME = 1.0/60.0;
const size_t BODY_COUNT = 2;
double lag = 0.0;

TileMap::Data data = {
    24, 18,
    {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  1,  2,  3,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0, 13, 14, 15, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0, 25, 26, 27, 17,  2,  2,  2,  2,  3,  4,  0,  0,  0,  0,  1,  2,  3,  3,  3,  4,  0,  0,  0,
        0, 37, 38, 39, 29, 14, 14, 14, 14, 15, 16,  0,  0,  0,  0, 13, 14, 15, 15, 15, 16,  0,  0,  0,
        0, 37, 38, 26, 26, 26, 26, 26, 26, 27, 28,  0,  0,  0,  0, 49, 50, 51, 50, 51, 52,  0,  0,  0,
        0, 37, 38, 38, 38, 38, 38, 38, 38, 39, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0, 49, 50, 50, 50, 50, 50, 50, 51, 51, 52,  1,  2,  2,  2,  3,  4,  0,  0,  1,  2,  3,  4,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 13, 14, 14, 14, 15, 16,  0,  0, 13, 14, 15, 16,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 49, 50, 50, 51,  6, 17,  2,  3, 18,  5, 51, 52,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 37, 29, 14, 15, 30, 40,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 49, 50, 51, 50, 51, 52,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }
};

Body::Space space = {
    24, 18,
    {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
        0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
        0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }
};

circuit::view::StageRenderer  renderer(nullptr);
Body::Ptr                     mage;
vector<Body::Ptr>             stuff(BODY_COUNT, nullptr);
unique_ptr<CollisionManager>  collision_manager;

void Rendering(Canvas& canvas) {
    renderer.Render(canvas);
    mage->Render(canvas);
    for (auto& body : stuff)
        body->Render(canvas);
}

void CheckInputTask(double /*unused*/) {
    auto input = ugdk::input::manager();
    if (input->keyboard().IsDown(ugdk::input::Scancode::RIGHT))
      mage->ApplyForce(MAGE_SPEED*Vector2D(1.0, 0.0));
    if (input->keyboard().IsDown(ugdk::input::Scancode::LEFT))
      mage->ApplyForce(MAGE_SPEED*Vector2D(-1.0, 0.0));
}

void MoveMageTask(double dt) {
    lag += dt;
    while (lag >= FRAME_TIME) {
        Body::MoveAll(space, FRAME_TIME);
        lag -= FRAME_TIME;
    }
}

void GenerateBodies() {
    mage = Body::Create(Vector2D(2.0, 2.0));
    mage->set_name("mage");
    mage->Prepare();
    collision_manager->AddActiveObject(mage->collision());
    mage->collision()->StartColliding(collision_manager.get());
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<double> distribution(3.0,20.0);
    for (size_t i = 0; i < BODY_COUNT; ++i) {
        stuff[i] = Body::Create(Vector2D(distribution(generator), 2.0));
        stuff[i]->Prepare();
        collision_manager->AddActiveObject(stuff[i]->collision());
        stuff[i]->collision()->StartColliding(collision_manager.get());
        stuff[i]->set_name("stuff-" + std::to_string(i));
    }
}

} // unnamed namespace

int main(int argc, char* argv[]) {
    Configuration config;
    config.base_path = "assets/";
    assert(ugdk::system::Initialize(config));
    ugdk::system::text_manager()->AddFont("default", "fonts/Filmcrypob.ttf", 24.0);
    collision_manager = unique_ptr<CollisionManager>(new CollisionManager(
              Box<2>({-1.0, -1.0},{25.0, 19.0})));
    collision_manager->Find("body");
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    GenerateBodies();
    renderer = circuit::view::StageRenderer(TileMap::Create("sample", data));
    ourscene->set_render_function(Rendering);
    ourscene->AddTask(Task(CheckInputTask, 0.1));
    ourscene->AddTask(collision_manager->GenerateHandleCollisionTask(0.2));
    ourscene->AddTask(Task(MoveMageTask, 0.3));
    ourscene->event_handler().AddListener<ugdk::input::KeyPressedEvent>(
        [ourscene](const ugdk::input::KeyPressedEvent& ev) {
            if(ev.scancode == ugdk::input::Scancode::ESCAPE)
                ourscene->Finish();
            else if(ev.scancode == ugdk::input::Scancode::UP)
                mage->ApplyForce(Vector2D(0.0, -600.0));
        });
    ugdk::system::PushScene(ourscene);
    ugdk::system::Run();
    ugdk::system::Release();
    stuff.clear();
    mage = Body::Ptr(nullptr);
    return 0;
}

