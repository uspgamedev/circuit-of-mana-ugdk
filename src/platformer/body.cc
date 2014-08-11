
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
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v,
                  const TextureAtlas::BoundPiece& piece) {
        x = the_x, y = the_y;
        piece.ConvertToAtlas(the_u, the_v, &u, &v);
    }
};

void DrawBody(const Primitive& map_primitive, ShaderUse& shader_use) {
    // nothing
}

} // unnamed namespace

Body::Body(const ugdk::math::Vector2D& the_position)
        : position_(the_position), speed_(0.0, 0.0), body_primitive_(nullptr) {}

void Body::Prepare() {
    shared_ptr<VertexData> data(new VertexData(4u, sizeof(VertexXYUV), false));
    body_primitive_.reset(new Primitive(manager()->white_texture(), data));
    body_primitive_->set_drawfunction(DrawBody);
}

void Body::Render(Canvas& canvas) const {

}

} // namespace circuit

