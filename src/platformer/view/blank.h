
#ifndef CIRCUITOFMANA_VIEW_BLANK_H_
#define CIRCUITOFMANA_VIEW_BLANK_H_

#include <functional>
#include <list>
#include <ugdk/math.h>
#include <ugdk/graphic.h>
#include "model/body.h"

namespace circuit {
namespace view {

/// Renders simple 32x32 blank images.
/** <easter-egg>Sora: Blank must never lose!</easter-egg> */
class Blank {
  public:
    Blank();
    void Render(ugdk::graphic::Canvas& canvas,
                const std::list<std::shared_ptr<model::Body>>& bodies);
  private:
    std::unique_ptr<ugdk::graphic::Primitive> blank_primitive_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_VIEW_BLANK_H_

