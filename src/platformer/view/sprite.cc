
#include "view/sprite.h"

#include <functional>
#include <memory>
#include <string>
#include <ugdk/action/spritetypes.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/system/task.h>
#include <ugdk/system/taskplayer.h>

namespace circuit {
namespace view {

namespace {

using model::Body;
using ugdk::action::LoadSpriteAnimationTableFromFile;
using ugdk::math::Integer2D;
using ugdk::math::Vector2D;
using ugdk::graphic::Canvas;
using ugdk::graphic::Geometry;
using ugdk::graphic::manager;
using ugdk::graphic::opengl::ShaderUse;
using ugdk::graphic::opengl::VertexType;
using ugdk::graphic::TextureAtlas;
using ugdk::graphic::Primitive;
using ugdk::graphic::VertexData;
using ugdk::system::Task;
using ugdk::system::TaskPlayer;
using std::bind;
using std::mem_fn;
using std::shared_ptr;
using std::string;
using std::list;
using namespace std::placeholders;

struct VertexXYUV {
    GLfloat x, y, u, v;
    void set_xyuv(GLfloat the_x, GLfloat the_y, GLfloat the_u, GLfloat the_v,
                  const TextureAtlas::BoundPiece& piece) {
        x = the_x, y = the_y;
        piece.ConvertToAtlas(the_u, the_v, &u, &v);
    }
};

struct Frame {
    VertexXYUV vertices[4];
    void set_vertices(const TextureAtlas::BoundPiece& piece) {
        Integer2D size = piece.size();
        vertices[0].set_xyuv(-size.x/2.0, -size.y, 0.0f, 0.0f, piece);
        vertices[1].set_xyuv(-size.x/2.0f, 0.0f, 0.0f, 1.0f, piece);
        vertices[2].set_xyuv(size.x/2.0f, -size.y, 1.0f, 0.0f, piece);
        vertices[3].set_xyuv(size.x/2.0f, 0.0f, 1.0f, 1.0f, piece);
    }
};

string DiscoverAnimationName(const shared_ptr<const Body>& body) {
    string dir;
    if (body->looking_direction() == Body::LOOKING_RIGHT)
        dir = "_RIGHT";
    else
        dir = "_LEFT";
    string state;
    if (std::fabs(body->speed().x) > 1.0)
        state = "WALKING";
    else
        state = "STANDING";
    return state+dir;
}

} // unnamed namespace

Sprite::Sprite(const string& name, TaskPlayer* task_player)
    : primitive_(nullptr), sheet_(nullptr), animation_player_(nullptr),
      animation_table_(LoadSpriteAnimationTableFromFile("animations/being.json")) {
    // Create animation player and register on task player
    animation_player_.reset(new AnimationPlayer(animation_table_.get()));
    auto task_function = bind(mem_fn(&AnimationPlayer::Update), animation_player_.get(), _1);
    task_player->AddTask(Task(task_function, 0.9));
    // Load tileset
    sheet_.reset(TextureAtlas::LoadFromFile("spritesheets/"+name));
    // Create primitive
    shared_ptr<VertexData> data(new VertexData(sheet_->piece_num(), sizeof(Frame), false));
    primitive_.reset(new Primitive(sheet_->texture(), data));
    // Prepare primitive
    data->CheckSizes("CreatingSprite", animation_table_->size(), sizeof(Frame));
    {
        VertexData::Mapper mapper(*data);
        for (size_t i = 0; i < sheet_->piece_num(); ++i) {
            TextureAtlas::BoundPiece piece = sheet_->PieceAt(i);
            mapper.Get<Frame>(i)->set_vertices(piece);
        }
    }
}

void Sprite::Render(Canvas& canvas, const list<shared_ptr<Body>>& bodies) {
    ShaderUse shader_use(manager()->shaders().current_shader());
    shared_ptr<const VertexData> data = primitive_->vertexdata();
    shader_use.SendTexture(0, primitive_->texture());
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::VERTEX,
                                0, 2,
                                data->vertex_size()/4);
    shader_use.SendVertexBuffer(data->buffer().get(), VertexType::TEXTURE,
                                2*sizeof(GLfloat), 2,
                                data->vertex_size()/4);
    for (auto& body : bodies) {
        // Animation frame
        animation_player_->Select(DiscoverAnimationName(body));
        auto& frame = animation_player_->current_animation_frame();
        string frame_name = frame.atlas_frame_name();
        size_t frame_index = stoul(frame_name)-1;
        // Geomtry + effects
        Geometry geom = Geometry(body->position() * 32.0);
        if (frame.mirror() & ugdk::MIRROR_HFLIP)
            geom *= Geometry(Vector2D(), Vector2D(-1.0, 1.0));
        canvas.PushAndCompose(geom);
        shader_use.SendGeometry(canvas.current_geometry());
        shader_use.SendEffect(canvas.current_visualeffect());
        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, frame_index*4, 4u);
        // Clean up
        canvas.PopGeometry();
    }
}

} // namespace view
} // namespace circuit

