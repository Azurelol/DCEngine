#pragma once
#include <string>

namespace DCEngine {

  enum EnumeratedSystem {
    // Window Handler
    ES_WindowSFML
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

  private:

  protected:
    const ESys _type;
    std::string _name;
  };

}