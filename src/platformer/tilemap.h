
#ifndef CIRCUITOFMANA_TILEMAP_H_
#define CIRCUITOFMANA_TILEMAP_H_

#include <ugdk/graphic.h>

#include <string>
#include <memory>

namespace circuit {

class TileMap final {
  public:
    using Ptr = std::unique_ptr<TileMap>;
    ~TileMap();
    static Ptr Create(const std::string& name);
    void Render(ugdk::graphic::Canvas& canvas) const;
  private:
    TileMap();
    std::unique_ptr<ugdk::graphic::TextureAtlas>  tileset_;
    std::unique_ptr<ugdk::graphic::Primitive>     map_primitive_;
};

} // namespace circuit

#endif // CIRCUITOFMANA_TILEMAP_H_

