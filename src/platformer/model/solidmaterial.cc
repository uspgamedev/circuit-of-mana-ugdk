
#include "model/solidmaterial.h"

#include <iostream>
#include <utility>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/rect.h>
#include <ugdk/math/vector2D.h>
#include "model/body.h"

namespace circuit {
namespace model {

namespace {

using pyramidworks::collision::CollisionManager;
using pyramidworks::collision::CollisionObject;
using pyramidworks::geometry::Rect;
using std::string;
using std::pair;
using std::make_pair;
using std::shared_ptr;
using std::unique_ptr;
using ugdk::math::Vector2D;

pair<Vector2D, Vector2D> DecomposeInDir(Vector2D vec, Vector2D dir) {
    Vector2D perpendicular = Vector2D(-dir.y, dir.x);
    return make_pair(dir * (dir * vec), perpendicular * (perpendicular * vec));
}

pair<double, double> GetSpeedsAfterCollision(double v1, double v2) {
    double m1 = 1.0, m2 = 1.0;
    double e = 1.0;
    return make_pair(
        ( m2*e*(v2-v1) + (m1*v1) + (m2*v2) ) / (m1 + m2),
        ( m1*e*(v1-v2) + (m1*v1) + (m2*v2) ) / (m1 + m2)
    );
}

} // unnamed namespace

SolidMaterial::SolidMaterial(const shared_ptr<Body>& the_body, CollisionManager& the_manager)
    : Material(the_body), manager_(the_manager) {}

void SolidMaterial::OnSetupBody() {
    collision_.reset(new CollisionObject(
          body().get(), "solid", new Rect(1.0, 1.0)));
    collision_->AddCollisionLogic("solid", [this] (const CollisionObject* other) {
        Body* target = dynamic_cast<Body*>(other->owner());
        if (this->collided_.count(target) > 0)
            return;
        dynamic_cast<SolidMaterial&>(target->material()).collided_.insert(body().get());
        Vector2D collision_dir = (target->position() - body()->position()).Normalize();
        auto body_speed = DecomposeInDir(body()->speed(), collision_dir);
        auto target_speed = DecomposeInDir(target->speed(), collision_dir);
        if ((target_speed.first - body_speed.first)*collision_dir >= 0.0)
            return;
        auto result = GetSpeedsAfterCollision(body_speed.first.length(),
                                              target_speed.first.length());
        body()->set_speed(target_speed.first.Normalize()*result.first + body_speed.second);
        target->set_speed(body_speed.first.Normalize()*result.second + target_speed.second);
    });
    manager_.AddActiveObject(collision_.get());
    collision_->StartColliding(&manager_);
}

void SolidMaterial::OnPositionChange() {
    collision_->MoveTo(body()->position() + Vector2D(0.0, -0.5));
}

void SolidMaterial::OnPhysicsUpdate() {
    // Apply gravity
    if (body()->density() > 0.0)
        body()->ApplyForce(Vector2D(0.0, 40.0));
    // Apply friction
    body()->ApplyForce(Vector2D(-5.0*body()->speed().x, 0));
    // Clear collision mask
    collided_.clear();
}

void SolidMaterial::OnSceneryCollision() {

}

} // namespace model
} // namespace circuit

