
#ifndef CIRCUITOFMANA_TILEMAP_H_
#define CIRCUITOFMANA_TILEMAP_H_

#include <ugdk/graphic.h>

#include <memory>
#include <string>
#include <vector>

namespace circuit {
namespace view {

class TileMap final {
  public:
    using Ptr = std::unique_ptr<TileMap>;
    struct Data {
      size_t width, height;
      std::vector<size_t> indices;
    };
    ~TileMap();
    static Ptr Create(const std::string& name, const Data& tiles);
    void Render(ugdk::graphic::Canvas& canvas) const;
  private:
    TileMap();
    std::unique_ptr<ugdk::graphic::TextureAtlas>  tileset_;
    std::unique_ptr<ugdk::graphic::Primitive>     map_primitive_;
};

} // namespace view
} // namespace circuit

#endif // CIRCUITOFMANA_TILEMAP_H_

