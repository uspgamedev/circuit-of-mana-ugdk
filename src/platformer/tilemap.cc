
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
using ugdk::graphic::opengl::VertexType;

namespace {

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v) {
      x = the_x, y = the_y, u = the_u, v = the_v;
    }
};

void DrawTileMap(const Primitive& map_primitive, ShaderUse& shader_use) {
    shared_ptr<const VertexData> data = map_primitive.vertexdata();

    shader_use.SendTexture(0, map_primitive.texture());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::VERTEX, 0, 2, data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::TEXTURE, 2*sizeof(GLfloat), 2, data->vertex_size());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

const GLfloat TEXSIZE = 1.0f;
const GLfloat VEXSIZE = 100.0f;

} // unnamed namespace

TileMap::~TileMap() {}

TileMap::Ptr TileMap::Create(const string& name) {
    Ptr tilemap(new TileMap);
    // Load tileset
    tilemap->tileset_.reset(TextureAtlas::LoadFromFile("spritesheets/"+name));
    // Create primitive
    shared_ptr<VertexData> data(new VertexData(4u, sizeof(VertexXYUV), false));
    tilemap->map_primitive_.reset(new Primitive(tilemap->tileset_->texture(), data));
    // Prepare primitive
    tilemap->map_primitive_->set_drawfunction(DrawTileMap);
    data->CheckSizes("CreatingTileMap", 4, sizeof(VertexXYUV));
    {
        VertexData::Mapper mapper(*data);
        mapper.Get<VertexXYUV>(0)->set_xyuv(.0f, .0f, .0f, .0f);
        mapper.Get<VertexXYUV>(1)->set_xyuv(.0f, VEXSIZE, .0f, TEXSIZE);
        mapper.Get<VertexXYUV>(2)->set_xyuv(VEXSIZE, .0f, TEXSIZE, .0f);
        mapper.Get<VertexXYUV>(3)->set_xyuv(VEXSIZE, VEXSIZE, TEXSIZE, TEXSIZE);
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

