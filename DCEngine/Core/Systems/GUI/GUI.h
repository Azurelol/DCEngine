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
#include "Windows.h"

namespace DCEngine {

  class Engine;
  namespace Systems {
        
    static bool GUI_ENABLED = true;

    class InputSFML;
    class WindowSFML;
    class Editor;
    class GUI : public System {
      friend class Engine;
      friend class Editor;
      friend class InputSFML;
      friend class WindowSFML;
      friend class Windows;

    public:
    
    public:
      static void PopUp(Windows::PopUpData& data);
      static void Add(WindowPtr window);
    
    private:
                        
      void Remove(WindowPtr Window);
      void ClearInactive();
    	void ReloadVAO() { GUIHandler->ReloadVAO(); }
      void Initialize();
      void StartFrame();
      void Render();
      void Toggle();

      GUI();
      void Update(float dt);
      void Terminate();
      
      std::unique_ptr<ImGuiSFML> GUIHandler;
      std::vector<WindowPtr> ActiveWindows;
      std::vector<WindowPtr> InactiveWindows;
      sf::Event* EventObj;
      std::string DefaultFont;

    
      

    };




  }
}