
#include "model/firematerial.h"

#include <iostream>
#include "model/body.h"

namespace circuit {
namespace model {

namespace {

using std::shared_ptr;

} // unnamed namespace

FireMaterial::FireMaterial(const shared_ptr<Body>& the_body)
    : Material(the_body) {}

void FireMaterial::OnSceneryCollision() {
    std::cout << "Fire!" << std::endl;
    body()->destroy();
}

} // namespace model
} // namespace circuit

