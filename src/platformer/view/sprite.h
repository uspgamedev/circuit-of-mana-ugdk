
#ifndef CIRCUITOFMANA_VIEW_SPRITE_H_
#define CIRCUITOFMANA_VIEW_SPRITE_H_

#include <functional>
#include <vector>
#include <ugdk/action.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/math.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/system.h>
#include "model/body.h"

namespace circuit {
namespace view {

class Sprite {
  public:
    Sprite(const std::string& name, ugdk::system::TaskPlayer* task_player);
    void Render(ugdk::graphic::Canvas& canvas,
                const std::vector<model::Body::Ptr>& bodies);
  private:
    using AnimationPlayer = ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimationFrame>;
    std::unique_ptr<ugdk::graphic::Primitive>           primitive_;
    std::unique_ptr<ugdk::graphic::TextureAtlas>        sheet_;
    std::unique_ptr<AnimationPlayer>                    animation_player_;
    std::unique_ptr<ugdk::action::SpriteAnimationTable> animation_table_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_VIEW_SPRITE_H_

