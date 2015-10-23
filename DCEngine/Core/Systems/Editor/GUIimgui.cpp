#include "GUIimgui.h"

// Window Library
// Access to the Window System
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    GUI::GUI()
    {
    }

    void GUI::Initialize()
    {      
      if (TRACE_INITIALIZE)
        trace << "GUI::Initialize \n";
      
      // Save a pointer to the engine's window context
      WindowContext = Daisy->getSystem<Systems::Window>()->WindowHandler->GetWindow();

      // Setup ImGui bindng
      ImGuiSFMLInitialize(WindowContext, true);
    }

    void GUI::Update(float dt)
    {
      // Called every events are polled by the window
      ImGuiSFMLNewFrame();
      
      // ?? If there's issues, move after the graphics update?
      ImGui::Render();
    }

    void GUI::Terminate()
    {
      ImGuiSFMLTerminate();
    }


    /**************************************************************************/
    /*!                    ImGui Implementation Functions
    */
    /**************************************************************************/
    /**************************************************************************/
    /*!
    @brief  Initializes ImGui.
    @param  windowContext The Window Context used by SFML.
    @param  Whether to initialize callbacks?
    */
    /**************************************************************************/
    IMGUI_API bool GUI::ImGuiSFMLInitialize(sf::Window * windowContext, bool installCallbacks)
    {
      // Bind ImGui to SFML input events
      ImGuiSFMLBindEvents();
      return true;

    }
    /**************************************************************************/
    /*!
    \brief  Polls for all input events..
    */
    /**************************************************************************/
    IMGUI_API void GUI::ImGuiSFMLNewFrame()
    {

      // Setup the frame buffer size
    }

    /**************************************************************************/
    /*!
    \brief  Binds SFML input to ImGui.
    */
    /**************************************************************************/
    IMGUI_API void GUI::ImGuiSFMLBindEvents()
    {
      ImGuiIO& io = ImGui::GetIO();
      io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
      io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
      io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
      io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
      io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
      io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
      io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
      io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
      io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
      io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
      io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
      io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
      io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
      io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
      io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
      io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
      io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;
      TimeElapsed.restart();
    }

    /**************************************************************************/
    /*!
    @brief  Process input events into ImGui.
    @param  A reference to the event object.
    */
    /**************************************************************************/
    IMGUI_API void GUI::ImGuiSFMLProcessEvent(sf::Event & event)
    {
      switch (event.type) {
      case sf::Event::MouseButtonPressed:
      {
        MousePressed[event.mouseButton.button] = true;
        break;
      }
      case sf::Event::MouseButtonReleased:
      {
        MousePressed[event.mouseButton.button] = false;
        break;
      }
      case sf::Event::MouseWheelMoved:
      {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += (float)event.mouseWheel.delta;
        break;
      }
      case sf::Event::KeyPressed:
      {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.key.code] = true;
        io.KeyCtrl = event.key.control;
        io.KeyShift = event.key.shift;
        break;
      }
      case sf::Event::KeyReleased:
      {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.key.code] = false;
        io.KeyCtrl = event.key.control;
        io.KeyShift = event.key.shift;
        break;
      }
      case sf::Event::TextEntered:
      {
        if (event.text.unicode > 0 && event.text.unicode < 0x10000)
          ImGui::GetIO().AddInputCharacter(event.text.unicode);
        break;
      }
      default: 
        break;

      }
    }

    IMGUI_API void GUI::ImGuiSFMLInitializeRendering()
    {
      // Grab a reference to the input output
      ImGuiIO& io = ImGui::GetIO();
      // Setup the display size every frame
      io.DisplaySize = ImVec2(WindowContext->getSize().x, WindowContext->getSize().y);
      // Bind our implemented 'RenderDrawLists' function


    }

    /**************************************************************************/
    /*!
    @brief  Terminates ImGui.
    */
    /**************************************************************************/
    IMGUI_API void GUI::ImGuiSFMLTerminate()
    {
      ImGuiSFMLInvalidateDeviceObjects();
      ImGui::Shutdown();
    }

    IMGUI_API bool GUI::ImGuiSFMLCreateDeviceObjects()
    {
      return IMGUI_API bool();
    }

    IMGUI_API void GUI::ImGuiSFMLInvalidateDeviceObjects()
    {
      return IMGUI_API void();
    }


  }
}