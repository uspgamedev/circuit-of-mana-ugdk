
#include "tilemap.h"

#include <ugdk/graphic/canvas.h>

namespace circuit {

using std::string;
using std::make_shared;

using ugdk::graphic::Canvas;

TileMap::Ptr TileMap::Create(const string& name) {
  return Ptr(new TileMap);
}

void TileMap::Draw(Canvas& canvas) const {

}

} // namespace circuit

