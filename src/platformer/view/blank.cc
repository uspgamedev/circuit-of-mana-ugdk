
#include "view/blank.h"

#include <memory>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>

namespace circuit {
namespace view {

namespace {

using ugdk::graphic::Canvas;
using ugdk::graphic::Geometry;
using ugdk::graphic::manager;
using ugdk::graphic::opengl::ShaderUse;
using ugdk::graphic::opengl::VertexType;
using ugdk::graphic::Primitive;
using ugdk::graphic::VertexData;
using std::shared_ptr;
using std::vector;

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

void Blank::Render(Canvas& canvas, const vector<Body::Ptr>& bodies) {
    ShaderUse shader_use(manager()->shaders().current_shader());
    shared_ptr<const VertexData> data = blank_primitive_->vertexdata();
    shader_use.SendTexture(0, blank_primitive_->texture());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::VERTEX, 0, 2,
                                data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::TEXTURE,
                                2*sizeof(GLfloat), 2, data->vertex_size());
    for (auto& body : bodies) {
      canvas.PushAndCompose(Geometry(body->position() * 32.0));
      shader_use.SendGeometry(canvas.current_geometry());
      shader_use.SendEffect(canvas.current_visualeffect());
      glDrawArrays(GL_TRIANGLE_STRIP, 0, data->num_vertices());
      canvas.PopGeometry();
    }
}

} // namespace view
} // namespace circuit

