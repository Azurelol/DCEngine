/******************************************************************************/
/*!
\file   GUI.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   10/23/2015
\brief  The GUI interface class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../System.h"

// ImGui with SFML backend
#include "ImGuiSFML.h"

namespace DCEngine {

  class Engine;
  namespace Systems {
        
    static bool GUI_ENABLED = true;

    class InputSFML;
    class Editor;
    class GUI : public System {
      friend class Engine;
      friend class Editor;
      friend class InputSFML;
    
    public:

      struct PopUpData {
        std::string Title;
        std::string Message;
        std::string Confirmation;
      };
    
    public:
    	void ReloadVAO() { GUIHandler->ReloadVAO(); }
      static void PopUp(PopUpData& data);
    
    private:
            
      void Initialize();
      void StartFrame();
      void Render();
      void Toggle();

      GUI();
      void Update(float dt);
      void Terminate();
      
      std::unique_ptr<ImGuiSFML> GUIHandler;
      sf::Event* EventObj;
      std::string DefaultFont;

    
      

    };




  }
}