
#ifndef CIRCUITOFMANA_MODEL_FIREMATERIAL_H_
#define CIRCUITOFMANA_MODEL_FIREMATERIAL_H_

#include <memory>
#include <string>
#include <pyramidworks/collision.h>

#include "model/material.h"

namespace circuit {
namespace model {

class Body;

class FireMaterial : public Material {
  public:
    FireMaterial(const std::shared_ptr<Body>& the_body);
    virtual void OnSetupBody() override {}
    virtual void OnPositionChange() override {}
    virtual void OnPhysicsUpdate() override {}
    virtual void OnSceneryCollision() override;
};

} // namespace model
} // namespace circuit

#endif // CIRCUITOFMANA_MODEL_FIREMATERIAL_H_

