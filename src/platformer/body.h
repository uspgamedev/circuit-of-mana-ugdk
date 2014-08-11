
#ifndef CIRCUITOFMANA_BODY_H_
#define CIRCUITOFMANA_BODY_H_

#include <ugdk/math/vector2D.h>

namespace circuit {

class Body final {
  public:
    Body(const ugdk::math::Vector2D& the_position)
        : position_(the_position), speed_(0.0, 0.0) {}
    ugdk::math::Vector2D position() const { return position_; }
    void AddSpeed(const ugdk::math::Vector2D& diff) {
        speed_ += diff;
    }
    void AddSpeed(const double dx, const double dy) {
        AddSpeed(ugdk::math::Vector2D(dx, dy));
    }
    void Render(ugdk::graphic::Canvas& canvas) const;
  private:
    ugdk::math::Vector2D position_;
    ugdk::math::Vector2D speed_;
};

} // namespace circuit

#endif // CIRCUITOFMANA_BODY_H_

