/******************************************************************************/
/*!
\file   GUI.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   10/23/2015
\brief  The GUI interface class.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// ImGui with SFML backend
#include "ImGuiSFML.h"

namespace DCEngine {
  class Engine;
  namespace Systems {
        
    static bool GUI_ENABLED = false;

    class GUI : public System {
      friend class Engine;
    public:
    
    private:

      GUI();

      void Initialize();
      void StartFrame();
      void Render();

      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);
      
      std::unique_ptr<ImGuiSFML> GUIHandler;

    };




  }
}