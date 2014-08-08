
#ifndef UGD_CIRCUIT_TILEMAP_H_
#define UGD_CIRCUIT_TILEMAP_H_

#include <ugdk/graphic.h>

#include <string>
#include <memory>

namespace circuit {

class TileMap final {

  public:

    using Ptr = std::shared_ptr<TileMap>;

    static Ptr Create(const std::string& name);

    void Render(ugdk::graphic::Canvas& canvas) const;

  private:

    ugdk::graphic::TextureAtlas *atlas_;

    TileMap();

};

} // namespace circuit

#endif // UGD_CIRCUIT_TILEMAP_H_

