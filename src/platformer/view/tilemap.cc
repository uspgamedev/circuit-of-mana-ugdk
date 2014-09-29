
#include "view/tilemap.h"

#include <iostream>

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawmode.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/internal/opengl.h>

namespace circuit {
namespace view {

namespace {

using std::make_shared;
using std::shared_ptr;
using std::string;

using ugdk::graphic::Canvas;
using ugdk::graphic::DrawMode;
using ugdk::graphic::Manager;
using ugdk::graphic::TextureAtlas;
using ugdk::graphic::Primitive;
using ugdk::graphic::TextureUnit;
using ugdk::graphic::VertexData;
using ugdk::graphic::VertexType;
using ugdk::graphic::manager;

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v,
                  const TextureAtlas::BoundPiece& piece) {
        x = the_x, y = the_y;
        piece.ConvertToAtlas(the_u, the_v, &u, &v);
        show();
    }
    void show() const {
        //std::cout << "x=" << x << "\ty=" << y
        //          << "\tu=" << u << "\tv=" << v << std::endl;
    }
};

const GLfloat TILESIZE = 32.0f;

struct Tile {
    VertexXYUV vertices[6];
    void set_vertices(GLfloat x, GLfloat y, const TextureAtlas::BoundPiece& piece) {
        vertices[0].set_xyuv(x+TILESIZE, y, 1.0f, .0f, piece);
        vertices[1].set_xyuv(x, y, .0f, .0f, piece);
        vertices[2].set_xyuv(x, y+TILESIZE, .0f, 1.0f, piece);
        vertices[3].set_xyuv(x, y+TILESIZE, .0f, 1.0f, piece);
        vertices[4].set_xyuv(x+TILESIZE, y+TILESIZE, 1.0f, 1.0f, piece);
        vertices[5].set_xyuv(x+TILESIZE, y, 1.0f, .0f, piece);
    }
    void show() const {
      for (size_t i = 0; i < 6; ++i)
        vertices[i].show();
    }
};

void DrawTileMap(const Primitive& map_primitive, Canvas& canvas) {
    shared_ptr<const VertexData> data = map_primitive.vertexdata();

    TextureUnit holdit = manager()->ReserveTextureUnit(map_primitive.texture());
    canvas.SendVertexData(*data, VertexType::VERTEX, 0, 2, 6);
    canvas.SendVertexData(*data, VertexType::TEXTURE, 2*sizeof(GLfloat), 2, 6);
    canvas.DrawArrays(DrawMode::TRIANGLES(), 0, data->num_vertices()*6);
}

} // unnamed namespace

TileMap::~TileMap() {}

TileMap::Ptr TileMap::Create(const string& name, const Data& tiles) {
    Ptr tilemap(new TileMap);
    size_t tile_num = tiles.width*tiles.height;
    // Load tileset
    tilemap->tileset_.reset(TextureAtlas::LoadFromFile("spritesheets/"+name));
    // Create primitive
    shared_ptr<VertexData> data(new VertexData(tile_num, sizeof(Tile), false));
    tilemap->map_primitive_.reset(new Primitive(tilemap->tileset_->texture(), data));
    // Prepare primitive
    tilemap->map_primitive_->set_drawfunction(DrawTileMap);
    data->CheckSizes("CreatingTileMap", tile_num, sizeof(Tile));
    {
        VertexData::Mapper mapper(*data);
        for (size_t i = 0; i < tiles.height; ++i)
            for (size_t j = 0; j < tiles.width; ++j) {
                size_t offset = i*tiles.width + j;
                size_t piece_index = tiles.indices[offset];
                piece_index = static_cast<int>(piece_index)-1 < 0 ? 21 : piece_index-1;
                float x = j*TILESIZE, y = i*TILESIZE;
                TextureAtlas::BoundPiece piece = tilemap->tileset_->PieceAt(piece_index);
                mapper.Get<Tile>(offset)->set_vertices(x, y, piece);
            }
        for (size_t i = 0; i < tile_num; ++i)
            mapper.Get<Tile>(i)->show();
    }
    return tilemap;
}

void TileMap::Render(Canvas& canvas) const {
    map_primitive_->drawfunction()(*map_primitive_, canvas);
}

TileMap::TileMap() : tileset_(nullptr), map_primitive_(nullptr) {}

} // namespace view
} // namespace circuit

