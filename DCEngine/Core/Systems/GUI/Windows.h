/******************************************************************************/
/*!
@file   GUIWindows.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/13/2016
@brief  Static class for various, generic GUI windows.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// ImGui with SFML backend
#include "ImGuiSFML.h"

namespace DCEngine {
  class Component;

  namespace Systems {

    class Windows {
    public:
      struct PopUpData {
        std::string Title;
        std::string Message;
        std::vector<std::string> List;
        std::string Confirmation;
      };

      class Window {
      public:
        virtual ~Window();
        virtual void Display() = 0;
        void Remove();
      };

      /**************************************************************************/
      /*!
      @class PopUp A simple popup with a title and a message.
      */
      /**************************************************************************/
      class PopUp : public Window {
      public:
        PopUp(PopUpData data) : Data(data) {}
        void Display();
        PopUpData Data;
      };

      /**************************************************************************/
      /*!
      @class PopUpComponentDependencies A detailed popup for showing a component's
             dependencies.
      */
      /**************************************************************************/
      class PopUpComponentDependencies : public Window {
      public:
        PopUpComponentDependencies(PopUpData data, Component* component) : Data(data), ComponentRef(component) {}
        void Display();
      private:
        Component* ComponentRef;

        PopUpData Data;
      };
    };

    using WindowPtr = Windows::Window*;
    //using WindowPtr = std::shared_ptr<Windows::Window>;


  }
}