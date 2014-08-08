
#include "tilemap.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/textureatlas.h>

namespace circuit {

using std::string;
using std::make_shared;

using ugdk::graphic::Canvas;
using ugdk::graphic::TextureAtlas;

TileMap::Ptr TileMap::Create(const string& name) {
    Ptr tilemap(new TileMap);
    tilemap->atlas_ = TextureAtlas::LoadFromFile("spritesheets/"+name);
    return tilemap;
}

void TileMap::Render(Canvas& canvas) const {

}

TileMap::TileMap() :
    atlas_(nullptr) {}

} // namespace circuit

