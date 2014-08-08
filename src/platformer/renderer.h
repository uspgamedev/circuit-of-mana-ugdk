
#ifndef UGD_CIRCUIT_RENDER_H_
#define UGD_CIRCUIT_RENDER_H_

#include "tilemap.h"

#include <ugdk/graphic.h>
#include <utility>

namespace circuit {

class Renderer {

  public:

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&& another);

    Renderer(TileMap::Ptr&& the_tilemap) :
        tilemap_(std::move(the_tilemap)) {}

    void Render(ugdk::graphic::Canvas& canvas);

  protected:

    TileMap::Ptr tilemap_;

};

} // namespace circuit

#endif // UGD_CIRCUIT_RENDER_H_

