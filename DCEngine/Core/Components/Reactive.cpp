#include "Reactive.h"
#include "EngineReference.h"
//#include "../Systems/

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Reactive, "Reactive", DCEngineCore, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(Reactive, Active);
    DCE_BINDING_DEFINE_PROPERTY(Reactive, CaptureMouse);
    DCE_BINDING_DEFINE_PROPERTY(Reactive, Noise);
  }
  #endif

  /**************************************************************************/
  /*!
  @brief  Reactive constructor.
  */
  /**************************************************************************/
  Reactive::Reactive(Entity & owner) : Component("Reactive", owner)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Reactive destructor.
  */
  /**************************************************************************/
  Reactive::~Reactive()
  {
  }

  /**************************************************************************/
  /*!
  @brief  Initializes the 'Reactive' component.
  */
  /**************************************************************************/
  void Reactive::Initialize()
  {
    Connect(Daisy->getMouse(), Events::MouseUpdate, Reactive::OnMouseUpdateEvent);
    Connect(Daisy->getMouse(), Events::MouseDown, Reactive::OnMouseDownEvent);
  }

  /**************************************************************************/
  /*!
  @brief  Receives 'LogicUpdate' events.
  */
  /**************************************************************************/
  void Reactive::OnLogicUpdate(Events::LogicUpdate * event)
  {
    //if (Active)
    //  CheckMousePosition();
  }

  /**************************************************************************/
  /*!
  @brief  Receives 'MouseUpdate' events.
  */
  /**************************************************************************/
  void Reactive::OnMouseUpdateEvent(Events::MouseUpdate * event)
  {
    auto mouseToWorldPos = SpaceRef->getComponent<CameraViewport>()->ScreenToViewport(event->ScreenPosition);
    CheckMousePosition(mouseToWorldPos);
    //if (Noise)
    //  DCTrace << "Reactive::OnMouseUpdateEvent - x: " << event->ScreenPosition.x << " y : " 
    //                                                  << event->ScreenPosition.y << "\n";
  }

  /**************************************************************************/
  /*!
  @brief  Receives 'MouseDown' events.
  */
  /**************************************************************************/
  void Reactive::OnMouseDownEvent(Events::MouseDown * event)
  {
    // Check whether the mouse was clicked in the last mouse position
    //auto currentMousePos = Daisy->getMouse()->
    auto inside = Collision::PointToRectangle(dynamic_cast<GameObject*>(Owner()), Vec3(LastPosition.x, LastPosition.y, 0));
    if (inside)
      DispatchMouseClickedOnEvent();
  }

  /**************************************************************************/
  /*!
  @brief  Receives 'MouseUp' events.
  */
  /**************************************************************************/
  void Reactive::OnMouseUpEvent(Events::MouseUp * event)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Receives 'MouseScroll' events.
  */
  /**************************************************************************/
  void Reactive::OnMouseScrollEvent(Events::MouseScroll * event)
  {
  }
  
  /**************************************************************************/
  /*!
  @brief  Checks the mouse's position against this object.
  */
  /**************************************************************************/

  void Reactive::CheckMousePosition(Vec2 worldPos)
  {
   //if (Noise)
   //DCTrace << "Reactive::CheckMousePosition - x: " << worldPos.x << " y: " << worldPos.y << "\n";
    
    // Calculate the size of the bounding box
    if (auto inside = Collision::PointToRectangle(dynamic_cast<GameObject*>(Owner()), Vec3(worldPos.x, worldPos.y, 0))) {
      if (!MouseEntered) {
        MouseEntered = true;
        DispatchMouseEnterEvent();
        //DCTrace << "Mouse entered the reactive area. \n";
      }      
    }
    else {
      if (MouseEntered) {
        MouseEntered = false;
        //DCTrace << "Mouse exited the reactive area. \n";
        DispatchMouseExitEvent();
      }
    }
    // Update the mouse's last position
    LastPosition = worldPos;
  }


  void Reactive::DispatchMouseEnterEvent()
  {
    if (Noise)
      DCTrace << "Reactive::DispatchMouseEnterEvent \n";
    auto mouseEnter = new Events::MouseEnter();      
    Owner()->Dispatch<Events::MouseEnter>(mouseEnter);    
    delete mouseEnter;
  }

  void Reactive::DispatchMouseExitEvent()
  {
    if (Noise)
      DCTrace << "Reactive::DispatchMouseExitEvent \n";
    auto mouseExit = new Events::MouseExit();
    Owner()->Dispatch<Events::MouseExit>(mouseExit);
    delete mouseExit;
  }

  void Reactive::DispatchMouseClickedOnEvent()
  {
    if (Noise)
      DCTrace << "Reactive::DispatchMouseClickedOnEvent \n";
    auto mouseEnter = new Events::MouseClickedOn();
    Owner()->Dispatch<Events::MouseClickedOn>(mouseEnter);
    delete mouseEnter;
  }


}