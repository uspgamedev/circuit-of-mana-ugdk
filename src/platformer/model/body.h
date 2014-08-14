
#ifndef CIRCUITOFMANA_MODEL_BODY_H_
#define CIRCUITOFMANA_MODEL_BODY_H_

#include <memory>
#include <unordered_set>
#include <vector>
#include <ugdk/action/entity.h>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>

namespace circuit {
namespace model {

class Body final : public ugdk::action::Entity {
  public:
    using Ptr = std::shared_ptr<Body>;
    struct Space {
        size_t              width, height;
        std::vector<size_t> tiles;
    };
    std::string name() const {
        return name_;
    }
    void set_name(const std::string& the_name) {
        name_ = the_name;
    }
    ugdk::math::Vector2D position() const {
        return position_;
    }
    void set_position(const ugdk::math::Vector2D& the_position);
    pyramidworks::collision::CollisionObject* collision() const {
        return collision_.get();
    }
    void ApplyForce(const ugdk::math::Vector2D& the_force) {
        force_ += the_force;
    }
    void AddSpeed(const ugdk::math::Vector2D& diff) {
        speed_ += diff;
    }
    void AddSpeed(const double dx, const double dy) {
        AddSpeed(ugdk::math::Vector2D(dx, dy));
    }
    void Update(double dt) override {}
    static Ptr Create(const ugdk::math::Vector2D& the_position);
    static void MoveAll(const Space& space, const double dt);
  private:
    Body(const ugdk::math::Vector2D& the_position);
    std::string                                               name_;
    std::unique_ptr<pyramidworks::collision::CollisionObject> collision_;
    ugdk::math::Vector2D                                      position_, last_position_;
    ugdk::math::Vector2D                                      speed_;
    ugdk::math::Vector2D                                      force_;
    std::unordered_set<Body*>                                 collided_;
    static std::unordered_set<Ptr>                            bodies;
};

} // namespace model
} // namespace circuit

#endif // CIRCUITOFMANA_MODEL_BODY_H_
