
#include "model/body.h"

#include <iostream>
#include <utility>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/rect.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/structure/types.h>

namespace circuit {
namespace model {

using pyramidworks::collision::CollisionObject;
using pyramidworks::geometry::Rect;
using ugdk::Color;
using ugdk::math::Integer2D;
using ugdk::math::Vector2D;
using std::pair;
using std::make_pair;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

namespace {

bool IsColliding(const Body::Space& space, const Vector2D& position) {
    Integer2D tile_position(position);
    if (tile_position.x < 0 || tile_position.y < 0
        || tile_position.x >= space.width || tile_position.y >= space.height)
        return true;
    return space.tiles[tile_position.y*space.width + tile_position.x] > 0;
}

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

unordered_set<Body::Ptr> Body::bodies;

Body::Body(const ugdk::math::Vector2D& the_position)
        : position_(the_position), looking_direction_(LOOKING_RIGHT),
          last_position_(the_position), speed_(0.0, 0.0), force_(0.0, 0.0),
          collision_(nullptr) {}

Body::Ptr Body::Create(const ugdk::math::Vector2D& the_position) {
    Ptr body(new Body(the_position));
    bodies.insert(body);
    body->collision_ = unique_ptr<CollisionObject>(
            new CollisionObject(body.get(), "body", new Rect(1.0, 1.0)));
    body->collision_->AddCollisionLogic("body", [body] (const CollisionObject* other) {
        Body* target = dynamic_cast<Body*>(other->owner());
        if (body->collided_.count(target) > 0)
            return;
        target->collided_.insert(body.get());
        Vector2D collision_dir = (target->position_ - body->position_).Normalize();
        auto body_speed = DecomposeInDir(body->speed_, collision_dir);
        auto target_speed = DecomposeInDir(target->speed_, collision_dir);
        if ((target_speed.first - body_speed.first)*collision_dir >= 0.0)
            return;
        auto result = GetSpeedsAfterCollision(body_speed.first.length(), target_speed.first.length());
        body->speed_ = target_speed.first.Normalize()*result.first + body_speed.second;
        target->speed_ = body_speed.first.Normalize()*result.second + target_speed.second;
        body->set_position(body->last_position_);
        target->set_position(target->last_position_);
    });
    body->collision_->MoveTo(body->position_ + Vector2D(0.0, -0.5));
    return body;
}

void Body::set_position(const Vector2D& the_position) {
    last_position_ = position_;
    position_ = the_position;
    collision_->MoveTo(position_ + Vector2D(0.0, -0.5));
}

void Body::MoveAll(const Space& space, const double dt) {
    for (auto& body : bodies) {
      body->ApplyForce(Vector2D(0.0, 40.0));
      body->ApplyForce(Vector2D(-5.0*body->speed_.x, 0));
      body->speed_ += body->force_*dt;
      if (body->speed_.x < 0)
          body->looking_direction_ = LOOKING_LEFT;
      else if (body->speed_.x > 0)
          body->looking_direction_ = LOOKING_RIGHT;
      if (IsColliding(space, body->position_ + body->speed_*dt)) {
          Vector2D horizontal = Vector2D(body->speed_.x, 0.0),
                   vertical = Vector2D(0.0, body->speed_.y);
          if (IsColliding(space, body->position_ + horizontal*dt))
              body->speed_.x *= 0.0;
          if (IsColliding(space, body->position_ + vertical*dt))
              body->speed_.y *= 0.0;
      }
      body->set_position(body->position_ + body->speed_*dt);
      body->force_ *= 0;
      body->collided_.clear();
    }
}

} // namespace model
} // namespace circuit

