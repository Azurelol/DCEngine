/******************************************************************************/
/*!
@file   EditorObjects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/12/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorObjects.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief EditorObjects constructor.
    @param editor A reference to the Editor.
    */
    /**************************************************************************/
    EditorObjects::EditorObjects() : EditorModule(true),
      Attaching(false), Detaching(false), Deleting(false)
    {
      // The EditorObjects window starts displayed
      Toggle(true);
    }

    /**************************************************************************/
    /*!
    @brief  EditorObjects destructor.
    */
    /**************************************************************************/
    EditorObjects::~EditorObjects()
    {
    }

    /**************************************************************************/
    /*!
    @brief  Displays the objects window.
    */
    /**************************************************************************/
    void EditorObjects::Display()
    {
      if (!WindowEnabled)
        return;

      // Check if there's an action to be performed
      CheckState();

      // Set the position of the window
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver | ImGuiWindowFlags_ShowBorders);
      // Title
      ImGui::Begin("Objects", &WindowEnabled);
      // Capture a list of all objects on the space
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      // Show the attach menu
      //AttachMenu();
      // Swap position
      //ImGui::SameLine();
      if (ImGui::Button("Up")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(Access().SelectedObject()))
          ObjectsListSwapPosition(gameObject, Direction::Up);
      }
      ImGui::SameLine();
      if (ImGui::Button("Down")) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(Access().SelectedObject()))
          ObjectsListSwapPosition(gameObject, Direction::Down);
      }

      ImGui::Separator();
      unsigned objID = 0;
      // For every object
      for (auto& object : *objects) {
        // If you are parented, do not display on the root
        if (object->Parent())
          continue;
        //ImGui::PushID(objID++);
        // Display the object
        DisplayObject(object, objID);
        //ImGui::PopID();

        // Update their translations
        if (object->HasComponent("Transform"))
          object->getComponent<Components::Transform>()->UpdateTranslation();

      }


      // Ends the window
      ImGui::End();

    }

    /**************************************************************************/
    /*!
    @brief  Swaps the position of the currently selected GameObject.
    @param object A pointer to the object.
    @param dir The direction to which to move it.
    @todo   Find a way to swap and parent objects to one another with dragging.
    */
    /**************************************************************************/
    void EditorObjects::ObjectsListSwapPosition(GameObjectPtr object, Direction dir)
    {
      if (dir == Direction::Up) {
        Access().CurrentSpace->SwapGameObject(object, Direction::Up);
      }
      else if (dir == Direction::Down) {
        Access().CurrentSpace->SwapGameObject(object, Direction::Down);
      }
    }

    /**************************************************************************/
    /*!
    @brief Displays the specified object, and all its children, recursively.
    @param object A pointer to the object.
    @param dir The direction to which to move it.
    @todo   Find a way to swap and parent objects to one another with dragging.
    */
    /**************************************************************************/
    bool EditorObjects::DisplayObject(GameObjectPtr object, unsigned& id)
    {
      bool selected = Access().SelectedObject() && Access().SelectedObject()->getObjectID() == object->getObjectID();
      auto objectName = object->Name().c_str();
      auto& children = object->Children();

      ImGui::PushID(object->getObjectID());

      /* Jacque's pseudo-code
      bool showchildren = false
      if(haschildren) showchildren = treenode()

      selectable()
      ContextMenu()

      if(showchildren)
        DisplayChildren()
      */

      // Parent: If it has any children, display all children recursively
      if (!children.empty()) {
        bool treeOpened = false;
        ImGui::PushItemWidth(10);
        if (ImGui::TreeNode("")) {
          ImGui::PopItemWidth();
          treeOpened = true;
          // Display the parent selectable
          ImGui::SameLine();
          if (ImGui::Selectable(objectName, selected)) {
            // Attach
            if (Attaching)
              Attach(object);
            Access().SelectObject(object);
            Access().Inspector.Toggle(true);
          }
          // Display a context menu
          ContextMenu(object);

          // Display all children recursively
          for (auto& child : children) {
            DisplayObject(child, ++id);
          }
          ImGui::TreePop();
        }
        if (!treeOpened) {
          // Display the parent selectable
          ImGui::SameLine();

          if (ImGui::Selectable(objectName, selected)) {
            // Attach
            if (Attaching)
              Attach(object);
            Access().SelectObject(object);
            Access().Inspector.Toggle(true);
          }
          // Display a context menu
          ContextMenu(object);
        }
      }

      // If it has no children:
      else {

        std::string indent = "  "; 
        // If it has no parent, indent to match the treenodes' indent 
        if (!object->Parent()) {
          indent = "       ";
        }

        if (ImGui::Selectable(std::string(indent + objectName).c_str(), selected)) {
          // Attach
          if (Attaching)
            Attach(object);
          // Select
          Access().SelectObject(object);
          Access().Inspector.Toggle(true);
        }
        // Display a context menu
        ContextMenu(object);
        // If the object was locked..
        IsLocked(object);
      }

      ImGui::PopID();
      return false;
    }

    /**************************************************************************/
    /*!
    @brief Displays a context menu.
    @param object A pointer to the object.
    */
    /**************************************************************************/
    void EditorObjects::ContextMenu(GameObjectPtr object)
    {
      if (ImGui::BeginPopupContextItem("GameObject Context Menu"))
      {
        if (ImGui::Selectable("Attach")) {
          // The next object to be selected will be the one we will be attaching to
          Attaching = true;
          ImGui::GetIO().MouseDrawCursor = true;
        }
        if (ImGui::Selectable("Dettach")) {
          Detaching = true;
        }
        // Lock/Unlock the object
        if (ImGui::Selectable("Lock")) {
          auto locked = object->getLocked();
          object->setLocked(!locked);
        }
        if (ImGui::Selectable("Rename")) {

        }
        if (ImGui::Selectable("Delete")) {
          Deleting = true;
        }
        ImGui::EndPopup();
      }
    }

    /**************************************************************************/
    /*!
    @brief Checks if an object is locked, and if it is displays a marker.
    @param object A pointer to the object.
    */
    /**************************************************************************/
    bool EditorObjects::IsLocked(GameObjectPtr object)
    {
      if (object->getLocked()) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "L");
        return true;
      }
      return false;
    }

    bool EditorObjects::IsParent(GameObjectPtr object)
    {
      if (!object->Children().empty()) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "P");
        return true;
      }
      return false;
    }

    /**************************************************************************/
    /*!
    @brief Displays an attach menu.
    */
    /**************************************************************************/
    void EditorObjects::AttachMenu()
    {
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      auto objectIdentifiers = Access().CurrentSpace->IdentifyAllObjects();
      auto names = objectIdentifiers.NamesAsChars();
      ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.0f);

      auto gameObject = dynamic_cast<GameObjectPtr>(Access().SelectedObject());
      bool attached = false;
      if (gameObject && gameObject->Parent())
        attached = true;

      // Attachable object list
      static int currentAttachable = 0;
      // Attempt to attach a GameObject if there is one selected
      if (ImGui::Combo("##Attach", &currentAttachable, names.data(), static_cast<int>(names.size()))) {
        DCTrace << "Attaching to " << objectIdentifiers.Names[currentAttachable] << ", with ID: " << objectIdentifiers.IDs[currentAttachable] << "\n";
        //if (auto gameObject = dynamic_cast<GameObjectPtr>(Access().SelectedObject())) {
        //  gameObject->AttachTo(objects->at(currentAttachable));
        //}
      }

      // Button settings
      ImGui::SameLine();
      std::string caption = "Attach To";
      ImVec4 captionColor = ImVec4(255, 0, 0, 255);
      if (attached) {
        caption = "Detach";
        captionColor = ImVec4(127, 127, 0, 255);
      }
      // Button
      if (ImGui::Button(caption.c_str())) {
        if (gameObject) {
          if (!attached) {
            gameObject->AttachTo(objects->at(currentAttachable));
          }
          else {
            gameObject->Detach();
          }
        }
      }
    }

    void EditorObjects::Attach(GameObjectPtr object)
    {
      // If attaching is enabled, attach to the selected object
      if (auto gameObject = dynamic_cast<GameObjectPtr>(Access().SelectedObject())) {
        gameObject->AttachTo(object);
        ImGui::GetIO().MouseDrawCursor = false;
        Attaching = false;
      }
    }

    /**************************************************************************/
    /*!
    @brief Checks the current state.
    */
    /**************************************************************************/
    void EditorObjects::CheckState()
    {
      if (Deleting) {
        Access().DeleteObject();
        Deleting = false;
      }

      if (Detaching) {
        if (auto gameObject = dynamic_cast<GameObjectPtr>(Access().SelectedObject())) {
          gameObject->Detach();
        }
        Detaching = false;
      }

      if (Attaching) {
        // Disable attaching if right mouse button has been clicked.
        if (ImGui::IsMouseClicked(1)) {
          Attaching = false;
        }

        ShowOverlay();
      }
    }

    /**************************************************************************/
    /*!
    @brief Shows an overlay depicting state changes.
    */
    /**************************************************************************/
    void EditorObjects::ShowOverlay()
    {
      // Show overlay
      static bool opened;
      auto& cursorPos = ImGui::GetMousePos();
      auto overlaySize = ImVec2(2, 2);
      ImGui::SetNextWindowPos(cursorPos, ImGuiSetCond_Always);
      if (!ImGui::Begin("Example: Fixed Overlay", &opened, overlaySize, 0.3f, ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
      {
        ImGui::End();
        return;
      }
      std::string action;
      if (Attaching) action = "Attach";
      ImGui::Text(action.c_str());
      ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
      ImGui::End();
    }


  }
}