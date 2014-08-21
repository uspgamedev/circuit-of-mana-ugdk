
#ifndef CIRCUITOFMANA_MODEL_SOLIDMATERIAL_H_
#define CIRCUITOFMANA_MODEL_SOLIDMATERIAL_H_

#include <memory>
#include <string>
#include <pyramidworks/collision.h>

#include "model/material.h"

namespace circuit {
namespace model {

class Body;

class SolidMaterial : public Material {
  public:
    SolidMaterial(const std::shared_ptr<Body>& the_body,
                  pyramidworks::collision::CollisionManager& the_manager);
    virtual void OnSetupBody() override;
    virtual void OnPositionChange() override;
    virtual void OnPhysicsUpdate() override;
    virtual void OnSceneryCollision() override;
  private:
    std::unordered_set<Body*>                                 collided_;
    std::unique_ptr<pyramidworks::collision::CollisionObject> collision_;
    pyramidworks::collision::CollisionManager                 &manager_;
};

} // namespace model
} // namespace circuit

#endif // CIRCUITOFMANA_MODEL_SOLIDMATERIAL_H_

