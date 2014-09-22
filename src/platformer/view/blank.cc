
#include "view/blank.h"

#include <memory>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawmode.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/internal/opengl.h>

namespace circuit {
namespace view {

namespace {

using model::Body;
using ugdk::graphic::Canvas;
using ugdk::graphic::DrawMode;
using ugdk::graphic::Geometry;
using ugdk::graphic::manager;
using ugdk::graphic::Primitive;
using ugdk::graphic::TextureUnit;
using ugdk::graphic::VertexData;
using ugdk::graphic::VertexType;
using std::shared_ptr;
using std::list;

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v) {
        x = the_x, y = the_y;
        u = the_u, v = the_v;
    }
};

} // unnamed namespace

Blank::Blank() : blank_primitive_(nullptr) {
    shared_ptr<VertexData> data(new VertexData(4u, sizeof(VertexXYUV), false));
    blank_primitive_.reset(new Primitive(manager()->white_texture(), data));
    VertexData::Mapper mapper(*data);
    mapper.Get<VertexXYUV>(0)->set_xyuv(-16.0f, -32.0f, 0.0f, 0.0f);
    mapper.Get<VertexXYUV>(1)->set_xyuv(-16.0f, 0.0f, 0.0f, 1.0f);
    mapper.Get<VertexXYUV>(2)->set_xyuv(16.0f, -32.0f, 1.0f, 0.0f);
    mapper.Get<VertexXYUV>(3)->set_xyuv(16.0f, 0.0f, 1.0f, 1.0f);
}

void Blank::Render(Canvas& canvas, const list<shared_ptr<Body>>& bodies) {
    //ShaderUse shader_use(manager()->shaders().current_shader());
    shared_ptr<const VertexData> data = blank_primitive_->vertexdata();
    TextureUnit holdit = manager()->ReserveTextureUnit(blank_primitive_->texture());
    canvas.SendVertexData(*data, VertexType::VERTEX, 0, 2);
    canvas.SendVertexData(*data, VertexType::TEXTURE, 2*sizeof(GLfloat), 2);
    for (auto& body : bodies) {
      canvas.PushAndCompose(Geometry(body->position() * 32.0));
      canvas.DrawArrays(DrawMode::TRIANGLE_STRIP(), 0, data->num_vertices());
      canvas.PopGeometry();
    }
}

} // namespace view
} // namespace circuit

