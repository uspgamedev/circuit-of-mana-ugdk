
#include "tilemap.h"

#include <iostream>

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/opengl/shaderuse.h>

namespace circuit {

using std::make_shared;
using std::shared_ptr;
using std::string;

using ugdk::graphic::Canvas;
using ugdk::graphic::Manager;
using ugdk::graphic::TextureAtlas;
using ugdk::graphic::Primitive;
using ugdk::graphic::VertexData;
using ugdk::graphic::opengl::ShaderUse;
using ugdk::graphic::manager;
using ugdk::graphic::opengl::VertexType;

namespace {

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v,
                  const TextureAtlas::BoundPiece& piece) {
        x = the_x, y = the_y;
        piece.ConvertToAtlas(the_u, the_v, &u, &v);
        std::cout << "x=" << x << "\ty=" << y << "\tu=" << u << "\tv=" << v << std::endl;
    }
};

void DrawTileMap(const Primitive& map_primitive, ShaderUse& shader_use) {
    shared_ptr<const VertexData> data = map_primitive.vertexdata();

    shader_use.SendTexture(0, map_primitive.texture());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::VERTEX, 0, 2,
                                data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::TEXTURE,
                                2*sizeof(GLfloat), 2, data->vertex_size());
    glDrawArrays(GL_TRIANGLES, 0, data->num_vertices());
}

const GLfloat TILESIZE = 128.0f;

} // unnamed namespace

TileMap::~TileMap() {}

TileMap::Ptr TileMap::Create(const string& name, const Data& tiles) {
    Ptr tilemap(new TileMap);
    size_t vertex_num = tiles.width*tiles.height*6;
    // Load tileset
    tilemap->tileset_.reset(TextureAtlas::LoadFromFile("spritesheets/"+name));
    // Create primitive
    shared_ptr<VertexData> data(new VertexData(vertex_num, sizeof(VertexXYUV), false));
    tilemap->map_primitive_.reset(new Primitive(tilemap->tileset_->texture(), data));
    // Prepare primitive
    tilemap->map_primitive_->set_drawfunction(DrawTileMap);
    data->CheckSizes("CreatingTileMap", vertex_num, sizeof(VertexXYUV));
    {
        VertexData::Mapper mapper(*data);
        for (size_t i = 0; i < tiles.height; ++i)
            for (size_t j = 0; j < tiles.width; ++j) {
                size_t offset = i*tiles.width + j;
                float x = j*TILESIZE, y = i*TILESIZE;
                TextureAtlas::BoundPiece piece = tilemap->tileset_->PieceAt(
                        tiles.indices[offset]);
                mapper.Get<VertexXYUV>(offset+2)->set_xyuv(x+TILESIZE, y, 1.0f,
                                                           .0f, piece);
                mapper.Get<VertexXYUV>(offset+1)->set_xyuv(x, y, .0f, .0f, piece);
                mapper.Get<VertexXYUV>(offset+0)->set_xyuv(x, y+TILESIZE,
                                                           .0f, 1.0f, piece);
                mapper.Get<VertexXYUV>(offset+5)->set_xyuv(x, y+TILESIZE,
                                                           .0f, 1.0f, piece);
                mapper.Get<VertexXYUV>(offset+4)->set_xyuv(x+TILESIZE, y+TILESIZE,
                                                           1.0f, 1.0f, piece);
                mapper.Get<VertexXYUV>(offset+3)->set_xyuv(x+TILESIZE, y, 1.0f,
                                                           .0f, piece);
            }
    }
    return tilemap;
}

void TileMap::Render(Canvas& canvas) const {
    ShaderUse shader_use(manager()->shaders().current_shader());

    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());

    map_primitive_->drawfunction()(*map_primitive_, shader_use);
}

TileMap::TileMap() : tileset_(nullptr), map_primitive_(nullptr) {}

} // namespace circuit

