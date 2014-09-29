
#include "model/body.h"
#include "model/solidmaterial.h"
#include "model/firematerial.h"
#include "view/tilemap.h"
#include "view/stagerenderer.h"

#include <ctime>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <list>
#include <libjson.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/structure/types.h>
#include <ugdk/structure/box.h>
#include <ugdk/system/engine.h>
#include <ugdk/system/task.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>

namespace {

using circuit::view::TileMap;
using circuit::model::Body;
using circuit::model::SolidMaterial;
using circuit::model::FireMaterial;
using pyramidworks::collision::CollisionManager;
using ugdk::Color;
using ugdk::graphic::Canvas;
using ugdk::math::Vector2D;
using ugdk::structure::Box;
using ugdk::system::Configuration;
using ugdk::system::Task;
using std::unique_ptr;
using std::list;
using std::shared_ptr;
using std::string;

const double MAGE_SPEED       = 30.0;
const double FRAME_TIME       = 1.0/60.0;
const size_t BODY_COUNT       = 2;
const size_t SHOOTING_PERIOD  = 2*60;

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

unique_ptr<circuit::view::StageRenderer>  renderer;
shared_ptr<Body>                          mage;
unique_ptr<CollisionManager>              collision_manager;
double                                    lag = 0.0;
size_t                                    counter = SHOOTING_PERIOD;

void Rendering(Canvas& canvas) {
    renderer->Render(canvas, Body::all());
}

void CheckInputTask(double /*unused*/) {
    auto input = ugdk::input::manager();
    if (input->keyboard().IsDown(ugdk::input::Scancode::RIGHT))
      mage->ApplyForce(MAGE_SPEED*Vector2D(1.0, 0.0));
    if (input->keyboard().IsDown(ugdk::input::Scancode::LEFT))
      mage->ApplyForce(MAGE_SPEED*Vector2D(-1.0, 0.0));
}

void AddBlankThing(const Vector2D pos) {
    shared_ptr<Body> new_body = Body::Create(pos);
    new_body->set_name("thing-" + std::to_string(Body::all().size()));
}

void AddSolid(const Vector2D pos) {
    shared_ptr<Body> new_body = Body::Create(pos);
    new_body->set_material(unique_ptr<SolidMaterial>(new SolidMaterial(new_body, *collision_manager)));
    new_body->set_name("solid-" + std::to_string(Body::all().size()));
}

shared_ptr<Body> AddFlame(const Vector2D pos) {
    shared_ptr<Body> new_body = Body::Create(pos);
    new_body->set_material(unique_ptr<FireMaterial>(new FireMaterial(new_body)));
    new_body->set_name("flame-" + std::to_string(Body::all().size()));
    return new_body;
}

void MoveTask(double dt) {
    lag += dt;
    while (lag >= FRAME_TIME) {
        if (--counter == 0) {
            list<shared_ptr<Body>> launchers;
            for (auto& body : Body::all())
                if (body->name().find("solid") != string::npos)
                    launchers.push_back(body);
            for (auto& body : launchers) {
                AddFlame(body->position())->ApplyForce(1500.0*Vector2D(1.0, 0.0));
                AddFlame(body->position())->ApplyForce(1500.0*Vector2D(0.0, -1.0));
                AddFlame(body->position())->ApplyForce(1500.0*Vector2D(-1.0, 0.0));
            }
            counter = SHOOTING_PERIOD;
        }
        Body::MoveAll(space, FRAME_TIME);
        Body::CleanUp();
        lag -= FRAME_TIME;
    }
}

void GenerateBodies() {
    mage = Body::Create(Vector2D(2.0, 2.0));
    mage->set_name("mage");
    mage->set_material(unique_ptr<SolidMaterial>(new SolidMaterial(mage, *collision_manager)));
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<double> distribution(3.0,20.0);
    for (size_t i = 0; i < BODY_COUNT; ++i)
        AddSolid(Vector2D(distribution(generator), 2.0));
}

} // unnamed namespace

int main(int argc, char* argv[]) {
    // Game configuration
    Configuration config;
    config.base_path = "assets/";
    // Initialize engine
    assert(ugdk::system::Initialize(config));
    // Set collisions up
    collision_manager = unique_ptr<CollisionManager>(new CollisionManager(
              Box<2>({{-1.0, -1.0}},{{25.0, 19.0}})));
    collision_manager->Find("body");
    collision_manager->Find("arcane");
    // Create scene
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    GenerateBodies();
    // Set renderer up
    renderer.reset(new circuit::view::StageRenderer(
              TileMap::Create("sample", data), ourscene));
    ourscene->set_render_function(Rendering);
    // Add scene tasks
    ourscene->AddTask(Task(CheckInputTask, 0.1));
    ourscene->AddTask(collision_manager->GenerateHandleCollisionTask(0.2));
    ourscene->AddTask(Task(MoveTask, 0.3));
    ourscene->event_handler().AddListener<ugdk::input::KeyPressedEvent>(
        [ourscene](const ugdk::input::KeyPressedEvent& ev) {
            if(ev.scancode == ugdk::input::Scancode::ESCAPE)
                ourscene->Finish();
            if((ev.scancode == ugdk::input::Scancode::Z) && mage->on_floor())
                mage->ApplyForce(Vector2D(0.0, -1200.0));
            if(ev.scancode == ugdk::input::Scancode::X) {
                auto fire = AddFlame(mage->front_position());
                fire->set_density(0.0);
                fire->ApplyForce(1500.0*mage->front_direction());
            }
            if(ev.scancode == ugdk::input::Scancode::C) {
                for (size_t i = 0; i < 3; ++i) {
                    auto fire = AddFlame(mage->front_position());
                    fire->set_density(0.0);
                    fire->ApplyForce(1500.0*mage->front_direction().Rotate(-0.25 + i*0.25));
                }
            }
        });
    ugdk::system::PushScene(unique_ptr<ugdk::action::Scene>(ourscene));
    ugdk::system::Run();
    ugdk::system::Release();
    mage.reset();
    return 0;
}

