
#include "tilemap.h"

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

namespace {

struct VertexXYUV {
    GLfloat x, y, u, v;
};

void DrawTileMap(const Primitive& map_primitive, ShaderUse& shader_use) {

}

} // unnamed namespace

TileMap::~TileMap() {}

TileMap::Ptr TileMap::Create(const string& name) {
    Ptr tilemap(new TileMap);
    // Load tileset
    tilemap->tileset_.reset(TextureAtlas::LoadFromFile("spritesheets/"+name));
    // Create primitive
    shared_ptr<VertexData> vertex_data(new VertexData(4u, sizeof(VertexXYUV), false));
    tilemap->map_primitive_.reset(new Primitive(tilemap->tileset_->texture(), vertex_data));
    // Prepare primitive
    tilemap->map_primitive_->set_drawfunction(DrawTileMap);
    return tilemap;
}

void TileMap::Render(Canvas& canvas) const {
    ShaderUse shader_use(manager()->shaders().current_shader());
    map_primitive_->drawfunction()(*map_primitive_, shader_use);
}

TileMap::TileMap() : tileset_(nullptr), map_primitive_(nullptr) {}

} // namespace circuit

