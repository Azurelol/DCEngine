#pragma once
#include <string>
#include <memory>
#include <vector>

namespace DCEngine {

  enum EnumeratedSystem {
    // Window Handler
    ES_WindowSFML,
    ES_WindowGLFW
  };

  // Alias for the enum
  using ESys = EnumeratedSystem;

  class System {

  public:
    System(std::string& name, ESys type) : _name(name),
                                           _type(type)
    { }

    virtual void Initialize() = 0;
    virtual void Update(float dt) = 0;
    virtual void Terminate() = 0;

    friend class Engine; // Allows Engine to access protected/private

  private:

  protected:
    const ESys _type;
    std::string _name;
  };

  // ALIASES
  using SystemPtr = std::shared_ptr<System>;
  using SystemVec = std::vector<SystemPtr>;

} // DCEngine