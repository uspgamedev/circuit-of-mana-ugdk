
#include "body.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/opengl/shaderuse.h>

namespace circuit {

using ugdk::graphic::Canvas;
using ugdk::graphic::Manager;
using ugdk::graphic::TextureAtlas;
using ugdk::graphic::Primitive;
using ugdk::graphic::VertexData;
using ugdk::graphic::opengl::ShaderUse;
using ugdk::graphic::manager;
using ugdk::graphic::opengl::VertexType;
using std::shared_ptr;

namespace {

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v) {
        x = the_x, y = the_y;
        u = the_u, v = the_v;
    }
};

} // unnamed namespace

Body::Body(const ugdk::math::Vector2D& the_position)
        : position_(the_position), speed_(0.0, 0.0), body_primitive_(nullptr) {}

void Body::Prepare() {
    shared_ptr<VertexData> data(new VertexData(4u, sizeof(VertexXYUV), false));
    body_primitive_.reset(new Primitive(manager()->white_texture(), data));
    VertexData::Mapper mapper(*data);
    mapper.Get<VertexXYUV>(0)->set_xyuv(0.0f, 0.0f, 0.0f, 0.0f);
    mapper.Get<VertexXYUV>(1)->set_xyuv(0.0f, 32.0f, 0.0f, 1.0f);
    mapper.Get<VertexXYUV>(2)->set_xyuv(32.0f, 0.0f, 1.0f, 0.0f);
    mapper.Get<VertexXYUV>(3)->set_xyuv(32.0f, 32.0f, 1.0f, 1.0f);
}

void Body::Render(Canvas& canvas) const {
    ShaderUse shader_use(manager()->shaders().current_shader());
    shared_ptr<const VertexData> data = body_primitive_->vertexdata();

    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());

    shader_use.SendTexture(0, body_primitive_->texture());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::VERTEX, 0, 2,
                                data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::TEXTURE,
                                2*sizeof(GLfloat), 2, data->vertex_size());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, data->num_vertices());
}

} // namespace circuit

