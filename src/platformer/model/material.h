
#ifndef CIRCUITOFMANA_MODEL_MATERIAL_H_
#define CIRCUITOFMANA_MODEL_MATERIAL_H_

#include <memory>
#include <string>
#include <unordered_set>

namespace circuit {
namespace model {

class Body;

class Material {
  public:
    virtual ~Material() {}
    virtual void OnSetupBody() = 0;
    virtual void OnPhysicsUpdate() = 0;
    virtual void OnPositionChange() = 0;
    virtual void OnSceneryCollision() = 0;
  protected:
    Material(const std::shared_ptr<Body>& the_body) : body_(the_body) {}
    std::shared_ptr<Body> body() const { return body_; }
  private:
    std::shared_ptr<Body> body_;
};

} // namespace model
} // namespace circuit

#endif // CIRCUITOFMANA_MODEL_MATERIAL_H_

