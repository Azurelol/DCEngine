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
      auto camPos = CurrentSpace->getComponent<Components::CameraViewport>()->getCamera()->TransformComponent->Translation;
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
        auto objZ = obj->getComponent<Components::Transform>()->Translation.z;
        auto closestZ = closestObj->getComponent<Components::Transform>()->Translation.z;

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
      Windows.PropertiesEnabled = true;
      
      // Save its boundaries
      if (obj->HasComponent("Transform")) {
        Selection.SelectedBoundingCenter = obj->getComponent<Components::Transform>()->getTranslation();
        Selection.SelectedBoundingWidth = obj->getComponent<Components::Transform>()->getScale().x;
        Selection.SelectedBoundingHeight = obj->getComponent<Components::Transform>()->getScale().y;
      }

      Select(obj);      
      if (EditorCamera && Settings.TransformTool_IsComponent)
        EditorCamera->getComponent<Components::TransformTool>()->Select(obj);
    }


    /**************************************************************************/
    /*!
    @brief  Returns the currently selected object.
    */
    /**************************************************************************/
    ObjectPtr Editor::SelectedObject()
    {
      // Return the first object 
      if (!SelectedObjects.empty())
        return SelectedObjects.front();

      // If no objects are selected..
      return nullptr;
    }

    ObjectContainer& Editor::AllSelectedObjects()
    {
      return SelectedObjects;
    }

    /**************************************************************************/
    /*!
    @brief  Adds a command to the editor's command stack.
    */
    /**************************************************************************/
    void Editor::Add(CommandPtr command)
    {
      Settings.Commands.Add(command);
    }

    /**************************************************************************/
    /*!
    @brief  Selects the specified object.
    */
    /**************************************************************************/
    void Editor::Select(ObjectPtr object)
    {
      SelectedObjects.clear();
      SelectedObjects.push_back(object);
    }

    /**************************************************************************/
    /*!
    @brief  Adds or removes the selected object depending on whether
            it is already being selected or not.
    @param gameObject A pointer to the GameObject.
    */
    /**************************************************************************/
    void Editor::SelectionAddOrRemoveToMultiple(GameObjectPtr gameObject)
    {
      // If the object is already selected, deselect
      if (IsSelected(gameObject)) {
        auto objectPos = std::find(SelectedObjects.begin(), SelectedObjects.end(), 
                                   dynamic_cast<ObjectPtr>(gameObject));
        // If the object was found, deselect it
        if (*objectPos) {
          auto back = SelectedObjects.end() - 1; // Because end returns 1 past
          // If it's not the last element already
          if (objectPos != back)
            std::iter_swap(objectPos, back);
          SelectedObjects.pop_back();
          return;
        }
      }

      // If the object was not found, add it
      SelectedObjects.push_back(dynamic_cast<ObjectPtr>(gameObject));


      //// Look for the object among the SelectedObjects
      //for (auto& object : SelectedObjects) {
      //  // If the object was found..
      //  if (object->getObjectID() == gameObject->getObjectID()) {
      //    std::swap(object, )
      //  }
      //}


    }

    /**************************************************************************/
    /*!
    @brief  Deselects the selected object.
    */
    /**************************************************************************/
    void Editor::Deselect()
    {
      DCTrace << "Editor::Deselect \n";
      SelectedObjects.clear();
      if (EditorCamera && Settings.TransformTool_IsComponent)
        EditorCamera->getComponent<Components::TransformTool>()->Deselect();
    }

    /**************************************************************************/
    /*!
    @brief  Selects the Space object for property inspection.
    */
    /**************************************************************************/
    void Editor::SelectSpace()
    {
      Select(CurrentSpace);
      Windows.PropertiesEnabled = true;
    }

    /**************************************************************************/
    /*!
    @brief  Center the editor camera on the selected object.
    */
    /**************************************************************************/
    void Editor::CenterSelected()
    {
      if (!SelectedObject())
        return;

      // Translate the camera to be centered on the object.
      auto& objectPos = dynamic_cast<GameObjectPtr>(SelectedObject())
                       ->getComponent<Components::Transform>()->getTranslation();
      auto& cameraPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
      EditorCamera->getComponent<Components::Transform>()->setTranslation(Vec3(objectPos.x, objectPos.y, cameraPos.z));
    }


    /**************************************************************************/
    /*!
    @brief  Attempts to select any objects within the boundary drawn
            by the mouse cursor.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Editor::SelectMultiple(Vec2 & mousePosition)
    {
      if (!Selection.Dragging)
        return;
      
      // Get the current mouse position.
      auto endPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(mousePosition), 0);
      auto& startPos = Selection.MultiSelectStartPos;
      // Calculate the bounding box created between the endpoints (where the selection started, 
      // and the currnent mouse position)
      Selection.MultiSelectArea = endPos - startPos;
      Selection.MultiSelectMidpoint = Vec3((endPos.x + startPos.x) / 2,
                                          (endPos.y + startPos.y) / 2,
                                          (endPos.y + startPos.y) / 2);

      
      // Check for objects within the selected area. Perform a bounding box check.
      SelectedObjects.clear();
      for (auto& gameObject : CurrentSpace->GameObjectContainer) {

        // Do nothing if it has a camera component
        if (gameObject->HasComponent(std::string("Camera")))
          continue;

        // If the object lies within the bounding area..
        if (Daisy->getSystem<Physics>()->IsObjectWithinBoundingArea(Selection.MultiSelectMidpoint, Selection.MultiSelectArea.x, Selection.MultiSelectArea.y, gameObject)) {
          SelectedObjects.push_back(gameObject);
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief Checks if the object is currently selected.
    @param gameObject A pointer to the GameObject.
    @return Whether the object is currently being selected or not.
    */
    /**************************************************************************/
    bool Editor::IsSelected(GameObjectPtr gameObject)
    {
      for (auto& object : SelectedObjects) {
        if (gameObject->getObjectID() == object->getObjectID())
          return true;
      }

      return false;
    }




  }
}