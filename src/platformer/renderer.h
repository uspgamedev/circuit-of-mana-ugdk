
#ifndef UGD_CIRCUIT_RENDER_H_
#define UGD_CIRCUIT_RENDER_H_

#include "tilemap.h"

#include <ugdk/graphic.h>

namespace circuit {

class Renderer {

  public:

    Renderer(const TileMap::Ptr& the_tilemap) :
        tilemap_(the_tilemap) {}

    void Render(ugdk::graphic::Canvas& canvas);

    void operator() (ugdk::graphic::Canvas& canvas) {
        Render(canvas);
    }

  protected:

    TileMap::Ptr tilemap_;

};

} // namespace circuit

#endif // UGD_CIRCUIT_RENDER_H_

