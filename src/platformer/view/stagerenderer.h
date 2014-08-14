
#ifndef CIRCUITOFMANA_VIEW_STAGERENDERER_H_
#define CIRCUITOFMANA_VIEW_STAGERENDERER_H_

#include <memory>
#include <ugdk/graphic.h>

namespace circuit {

class TileMap;

namespace view {

class StageRenderer final {
  public:
    StageRenderer(std::unique_ptr<TileMap>&& the_tilemap); 
    void Render(ugdk::graphic::Canvas& canvas);
  private:
    std::unique_ptr<TileMap> tilemap_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_VIEW_STAGERENDERER_H_

