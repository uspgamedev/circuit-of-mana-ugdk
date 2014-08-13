
#ifndef CIRCUITOFMANA_BODY_H_
#define CIRCUITOFMANA_BODY_H_

#include <memory>
#include <unordered_set>
#include <vector>
#include <ugdk/action/entity.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/primitive.h>

namespace circuit {

class Body final : public ugdk::action::Entity {
  public:
    using Ptr = std::shared_ptr<Body>;
    struct Space {
        size_t              width, height;
        std::vector<size_t> tiles;
    };
    ugdk::math::Vector2D position() const {
        return position_;
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
    void Prepare();
    void Update(double dt) override {}
    void Render(ugdk::graphic::Canvas& canvas) const;
    static Ptr Create(const ugdk::math::Vector2D& the_position) {
        Ptr new_body(new Body(the_position));
        bodies.insert(new_body);
        return new_body;
    }
    static void MoveAll(const Space& space, const double dt);
  private:
    Body(const ugdk::math::Vector2D& the_position);
    std::unique_ptr<ugdk::graphic::Primitive> body_primitive_;
    ugdk::math::Vector2D                      position_;
    ugdk::math::Vector2D                      speed_;
    ugdk::math::Vector2D                      force_;
    static std::unordered_set<Ptr>            bodies;
};

} // namespace circuit

#endif // CIRCUITOFMANA_BODY_H_

