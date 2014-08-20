
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
    enum LookingDirection {
        LOOKING_RIGHT,
        LOOKING_LEFT
    };
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
    LookingDirection looking_direction() const {
        return looking_direction_;
    }
    ugdk::math::Vector2D position() const {
        return position_;
    }
    ugdk::math::Vector2D speed() const {
        return speed_;
    }
    double scalar_speed() const {
        return speed_.Length();
    }
    bool on_floor() const {
        return on_floor_;
    }
    void set_density(const double the_density) {
        density_ = the_density;
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
    static std::shared_ptr<Body> Create(const ugdk::math::Vector2D& the_position,
                                        const double the_density);
    static std::shared_ptr<Body> Create(const ugdk::math::Vector2D& the_position);
    static void MoveAll(const Space& space, const double dt);
  private:
    Body(const ugdk::math::Vector2D& the_position, const double the_density);
    std::string                                               name_;
    LookingDirection                                          looking_direction_;
    ugdk::math::Vector2D                                      position_, last_position_;
    ugdk::math::Vector2D                                      speed_;
    ugdk::math::Vector2D                                      force_;
    std::unordered_set<Body*>                                 collided_;
    std::unique_ptr<pyramidworks::collision::CollisionObject> collision_;
    bool                                                      on_floor_;
    double                                                    density_;
    static std::unordered_set<std::shared_ptr<Body>>          bodies;
};

} // namespace model
} // namespace circuit

#endif // CIRCUITOFMANA_MODEL_BODY_H_

