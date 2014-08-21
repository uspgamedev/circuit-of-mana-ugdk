
#include "model/body.h"

#include <cmath>
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/rect.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/structure/types.h>
#include "model/material.h"

namespace circuit {
namespace model {

using pyramidworks::collision::CollisionObject;
using pyramidworks::geometry::Rect;
using ugdk::Color;
using ugdk::math::Integer2D;
using ugdk::math::Vector2D;
using std::list;
using std::pair;
using std::make_pair;
using std::shared_ptr;
using std::unique_ptr;
using namespace std::placeholders;

namespace {

bool IsColliding(const Body::Space& space, const Vector2D& position) {
    Integer2D tile_position(position);
    if (tile_position.x < 0 || tile_position.y < 0
        || tile_position.x >= space.width || tile_position.y >= space.height)
        return true;
    return space.tiles[tile_position.y*space.width + tile_position.x] > 0;
}

} // unnamed namespace

list<shared_ptr<Body>> Body::bodies;

Body::Body(const ugdk::math::Vector2D& the_position, const double the_density)
        : position_(the_position), looking_direction_(LOOKING_RIGHT),
          speed_(0.0, 0.0), force_(0.0, 0.0), material_(new NullMaterial),
          on_floor_(false), density_(the_density), to_be_destroyed_(false) {}

shared_ptr<Body> Body::Create(const ugdk::math::Vector2D& the_position,
                              const double the_density) {
    shared_ptr<Body> body(new Body(the_position, the_density));
    bodies.push_back(body);
    return body;
}

shared_ptr<Body> Body::Create(const ugdk::math::Vector2D& the_position) {
    return Create(the_position, 1.0);
}

void Body::set_position(const Vector2D& the_position) {
    position_ = the_position;
    material_->OnPositionChange();
}

void Body::MoveAll(const Space& space, const double dt) {
    for (auto& body : bodies) {
        // Check looking direction
        if (body->force_.x < 0)
            body->looking_direction_ = LOOKING_LEFT;
        else if (body->force_.x > 0)
            body->looking_direction_ = LOOKING_RIGHT;
        // Apply material properties
        body->material_->OnPhysicsUpdate();
        // Update speed
        body->speed_ += body->force_*dt;
        // Check if body is airborne
        if (body->on_floor_ && std::fabs(body->speed_.y) > 0.05)
            body->on_floor_ = false;
        // Check collision with scenery
        if (IsColliding(space, body->position_ + body->speed_*dt)) {
            Vector2D horizontal = Vector2D(body->speed_.x, 0.0),
                     vertical = Vector2D(0.0, body->speed_.y);
            if (IsColliding(space, body->position_ + horizontal*dt))
                body->speed_.x *= 0.0;
            if (IsColliding(space, body->position_ + vertical*dt)) {
                if (body->speed_.y > 0.0)
                    body->on_floor_ = true;
                body->speed_.y *= 0.0;
            }
            body->material_->OnSceneryCollision();
        }
        // Round speed to zero if it is too low
        if (body->scalar_speed() < 0.4)
            body->speed_ *= 0.0;
        // Update position
        body->set_position(body->position_ + body->speed_*dt);
        // Clearn up
        body->force_ *= 0;
    }
}

void Body::CleanUp() {
    auto end = std::remove_if(
            bodies.begin(), bodies.end(),
            [](shared_ptr<Body>& body) -> bool { return body->to_be_destroyed_; });
    bodies.erase(end, bodies.end());
}

} // namespace model
} // namespace circuit

