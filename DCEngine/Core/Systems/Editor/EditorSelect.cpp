/******************************************************************************/
/*!
@file   EditorSelect.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Checks if there's an object at the current position in the space.
    @param  pos The position of the mouse relative to world space.
    @todo   Not have the camera's current direction vector hardcoded?
    Find the camera's forward through an orientation component.
    Use std::sort rather than some hacky algorithm.
    */
    /**************************************************************************/
    void Editor::SelectObjectFromSpace(GameObject* object)
    {
      if (object)
        SelectObject(object);
    }

    /**************************************************************************/
    /*!
    @brief  Finds the current object at the position.
    @param  obj A reference to an object.
    */
    /**************************************************************************/
    GameObject * Editor::FindObjectFromSpace(Vec2 pos)
    {
      // 1. Find all objects on the current mouse position
      auto objsAtPos = Daisy->getSystem<Physics>()->FindAllObjectsAtPosition(Vec3(pos, 0), *CurrentSpace);
      if (objsAtPos.empty())
        return nullptr;

      // 2.1 Find the camera's position in space.
      auto camPos = CurrentSpace->getComponent<CameraViewport>()->getCamera()->TransformComponent->Translation;
      // 2.2 Find the camera's forward direction vector.
      auto camDir = Vec3(0, 0, -1);
      // 3. Sort them in the order of the ones closest to the front of the camera.
      GameObjectPtr closestObj = nullptr;
      for (auto& obj : objsAtPos) {

        // Do not select the EditorCamera
        if (obj->getObjectName() == "EditorCamera")
          continue;
        
        // Do nothing if the GameObject does not have a transform.
        if (! obj->HasComponent(std::string("Transform")))
          continue;

        // 3.1 Get the Z-pos of the current object in the container
        auto ObjectName = obj->Name();

        // Done on the first iteration
        if (!closestObj) {
          closestObj = obj;
          continue;
        }          

        auto closestObjectName = closestObj->Name();
        auto objZ = obj->getComponent<Transform>()->Translation.z;
        auto closestZ = closestObj->getComponent<Transform>()->Translation.z;

        if (objZ < camPos.z && objZ > closestZ) {
          closestObj = obj;
        }
      }
      
      // 4. Pick the object closest to the camera.
      //SelectObject(closestObj);
      return closestObj;
    }

    /**************************************************************************/
    /*!
    @brief  Checks if the object is a valid selectable GameObject. That is,
            one with an active transform component.
    @param  object An Object*
    */
    /**************************************************************************/
    GameObjectPtr Editor::IsSelectableGameObject(ObjectPtr object)
    {
      if (auto gameObject = dynamic_cast<GameObjectPtr>(object)) {
        if (gameObject->HasComponent(std::string("Transform"))) {
          return gameObject;
        }
      }
      return nullptr;
    }

    /**************************************************************************/
    /*!
    @brief  Sets the selected object.
    @param  obj A reference to an object.
    */
    /**************************************************************************/
    void Editor::SelectObject(GameObject* obj)
    {
      DCTrace << "Editor::SelectObject - " << obj->Name() << "\n";
      WindowPropertiesEnabled = true;
      SelectedObject = obj;
    }

    /**************************************************************************/
    /*!
    @brief  Selects the Space object for property inspection.
    */
    /**************************************************************************/
    void Editor::SelectSpace()
    {
      SelectedObject = CurrentSpace;
      WindowPropertiesEnabled = true;
    }

    /**************************************************************************/
    /*!
    @brief  Drags the object
    @param  The mouse's current position.
    */
    /**************************************************************************/
    void Editor::DragObject(Vec2 pos)
    {
      // Only drag GameObjects in the Space while in translate mode
      if (ActiveTool != EditorTool::Translate)
        return;

      // If the mouse is currently being dragged
      if (Settings.Dragging) {
        //DCTrace << "Dragging! \n ";
        // If the selected object is a GameObject on the space
        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Calculate the current mouse position
          auto mousePos = CurrentSpace->getComponent<CameraViewport>()->ScreenToViewport(pos);
          // Move the object
          gameObject->getComponent<Transform>()->setTranslation(Vec3(mousePos.x, 
                                                                    mousePos.y, 
                                                                    gameObject->getComponent<Transform>()->getTranslation().z));

          // Snap the object

        }
      }

    }

    /**************************************************************************/
    /*!
    @brief  Releases the object at the current dragged position.    
    */
    /**************************************************************************/
    void Editor::ReleaseObject()
    {
      if (!SelectedObject)
        return;

      if (Settings.Dragging) {       

        // Snap the object to the nearest (x,y) snapDistance      
        if (Settings.Snapping) {
          auto& translation = dynamic_cast<GameObjectPtr>(SelectedObject)->getComponent<Transform>()->getTranslation();
          auto snappedPos = Math::Snap(Vec2(translation.x, translation.y));
          dynamic_cast<GameObjectPtr>(SelectedObject)->getComponent<Transform>()->setTranslation(Vec3(snappedPos.x, snappedPos.y, translation.z));

          DCTrace << "Editor::ReleaseObject - Releasing '" << SelectedObject->getObjectName() << "' at: \n"
            << "x: " << snappedPos.x << ", y: " << snappedPos.y << "\n";
        }

      }
        
    }



  }
}