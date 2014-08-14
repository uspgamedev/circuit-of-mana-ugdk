
#ifndef CIRCUITOFMANA_VIEW_SPRITE_H_
#define CIRCUITOFMANA_VIEW_SPRITE_H_

#include <functional>
#include <vector>
#include <ugdk/math.h>
#include <ugdk/graphic.h>
#include "body.h"

namespace circuit {
namespace view {

class Sprite {
  public:
    Sprite();
    void Render(ugdk::graphic::Canvas& canvas, const std::vector<Body::Ptr>& bodies);
  private:
    std::unique_ptr<ugdk::graphic::Primitive> blank_primitive_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_VIEW_SPRITE_H_

