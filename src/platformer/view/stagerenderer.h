
#ifndef CIRCUITOFMANA_VIEW_STAGERENDERER_H_
#define CIRCUITOFMANA_VIEW_STAGERENDERER_H_

#include <memory>
#include <list>
#include <ugdk/graphic.h>
#include <ugdk/system.h>
#include "model/body.h"
#include "view/blank.h"
#include "view/sprite.h"

namespace circuit {
namespace view {

class TileMap;

class StageRenderer final {
  public:
    StageRenderer(std::unique_ptr<TileMap>&& the_tilemap,
                  ugdk::system::TaskPlayer* task_player); 
    void Render(ugdk::graphic::Canvas& canvas,
                const std::list<std::shared_ptr<model::Body>>& bodies);
  private:
    std::unique_ptr<TileMap>  tilemap_;
    Blank                     blank_;
    Sprite                    sprite_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_VIEW_STAGERENDERER_H_

