
#include "body.h"

#include <iostream>
#include <utility>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/rect.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/structure/types.h>

namespace circuit {

using pyramidworks::collision::CollisionObject;
using pyramidworks::geometry::Rect;
using ugdk::Color;
using ugdk::math::Integer2D;
using ugdk::math::Vector2D;
using ugdk::graphic::Canvas;
using ugdk::graphic::Geometry;
using ugdk::graphic::Manager;
using ugdk::graphic::TextureAtlas;
using ugdk::graphic::Primitive;
using ugdk::graphic::VertexData;
using ugdk::graphic::opengl::ShaderUse;
using ugdk::graphic::manager;
using ugdk::graphic::opengl::VertexType;
using ugdk::graphic::VisualEffect;
using std::pair;
using std::make_pair;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

namespace {

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v) {
        x = the_x, y = the_y;
        u = the_u, v = the_v;
    }
};

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
    double e = 0.2;
    return make_pair(
        ( m2*e*(v2-v1) + (m1*v1) + (m2*v2) ) / (m1 + m2),
        ( m1*e*(v1-v2) + (m1*v1) + (m2*v2) ) / (m1 + m2)
    );
}

} // unnamed namespace

unordered_set<Body::Ptr> Body::bodies;

Body::Body(const ugdk::math::Vector2D& the_position)
        : body_primitive_(nullptr), collision_(nullptr),
          position_(the_position), speed_(0.0, 0.0), force_(0.0, 0.0) {}

Body::Ptr Body::Create(const ugdk::math::Vector2D& the_position) {
    Ptr new_body(new Body(the_position));
    bodies.insert(new_body);
    return new_body;
}

void Body::MoveAll(const Space& space, const double dt) {
    for (auto& body : bodies) {
      body->ApplyForce(Vector2D(0.0, 20.0));
      body->ApplyForce(Vector2D(-5.0*body->speed_.x, 0));
      body->speed_ += body->force_*dt;
      if (IsColliding(space, body->position_ + body->speed_*dt)) {
          Vector2D horizontal = Vector2D(body->speed_.x, 0.0),
                   vertical = Vector2D(0.0, body->speed_.y);
          if (IsColliding(space, body->position_ + horizontal*dt))
              body->speed_.x *= 0.0;
          if (IsColliding(space, body->position_ + vertical*dt))
              body->speed_.y *= 0.0;
      }
      body->position_ += body->speed_*dt;
      body->force_ *= 0;
      body->collision_->MoveTo(body->position_ + Vector2D(0.0, -0.5));
      body->collided_.clear();
    }
}

void Body::Prepare() {
    /* Prepating renderization */ {
        shared_ptr<VertexData> data(new VertexData(4u, sizeof(VertexXYUV), false));
        body_primitive_.reset(new Primitive(manager()->white_texture(), data));
        VertexData::Mapper mapper(*data);
        mapper.Get<VertexXYUV>(0)->set_xyuv(-16.0f, -32.0f, 0.0f, 0.0f);
        mapper.Get<VertexXYUV>(1)->set_xyuv(-16.0f, 0.0f, 0.0f, 1.0f);
        mapper.Get<VertexXYUV>(2)->set_xyuv(16.0f, -32.0f, 1.0f, 0.0f);
        mapper.Get<VertexXYUV>(3)->set_xyuv(16.0f, 0.0f, 1.0f, 1.0f);
    }
    /* Preparing collision */ {
        collision_ = unique_ptr<CollisionObject>(
                new CollisionObject(this, "body", new Rect(1.0, 1.0)));
        collision_->AddCollisionLogic("body", [this] (const CollisionObject* other) {
            Body* target = dynamic_cast<Body*>(other->owner());
            if (this->collided_.count(target) > 0)
                return;
            target->collided_.insert(this);
            Vector2D collision_dir = (target->position_ - this->position_).Normalize();
            auto this_speed = DecomposeInDir(this->speed_, collision_dir);
            auto target_speed = DecomposeInDir(target->speed_, collision_dir);
            auto result = GetSpeedsAfterCollision(this_speed.first.length(), target_speed.first.length());
            this->speed_ = target_speed.first.Normalize()*result.first + this_speed.second;
            target->speed_ = this_speed.first.Normalize()*result.second + target_speed.second;
        });
        collision_->MoveTo(position_ + Vector2D(0.0, -0.5));
    }
}

void Body::Render(Canvas& canvas) const {
    shared_ptr<const VertexData> data = body_primitive_->vertexdata();

    canvas.PushAndCompose(Geometry(Vector2D(Integer2D(position_) * 32.0), Vector2D(32.0, 32.0)));
    canvas.PushAndCompose(VisualEffect(Color(.4, .4, .8, 1.0)));
    ugdk::graphic::DrawSquare(canvas.current_geometry(), canvas.current_visualeffect(), manager()->white_texture());
    canvas.PopVisualEffect();
    canvas.PopGeometry();

    canvas.PushAndCompose(Geometry(position_ * 32.0));

    ShaderUse shader_use(manager()->shaders().current_shader());

    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());

    shader_use.SendTexture(0, body_primitive_->texture());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::VERTEX, 0, 2,
                                data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::TEXTURE,
                                2*sizeof(GLfloat), 2, data->vertex_size());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, data->num_vertices());

    canvas.PopGeometry();
}

} // namespace circuit

