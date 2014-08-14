
#ifndef CIRCUITOFMANA_VIEW_STAGERENDERER_H_
#define CIRCUITOFMANA_VIEW_STAGERENDERER_H_

#include "view/blank.h"
#include "view/sprite.h"
#include <memory>
#include <vector>
#include <ugdk/graphic.h>

namespace circuit {
namespace view {

class TileMap;

class StageRenderer final {
  public:
    StageRenderer(std::unique_ptr<TileMap>&& the_tilemap); 
    void Render(ugdk::graphic::Canvas& canvas,
                const std::vector<Body::Ptr>& bodies, const Body::Ptr& mage);
  private:
    std::unique_ptr<TileMap>  tilemap_;
    Blank                     blank_;
    Sprite                    sprite_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_VIEW_STAGERENDERER_H_

