#pragma once
#include "../System.h"

// Implementation
#include "GUIimgui.h"

namespace DCEngine {
  
  class Engine;
  
  namespace Systems {

    class Editor : public System {
      friend class Engine;

    public:

    private:

      Editor();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

      std::unique_ptr<GUI> GUIHandler;

    };

  }


}