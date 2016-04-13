/******************************************************************************/
/*!
@file   EditorObjects.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorObjects : public EditorModule {
    public:
      EditorObjects();
      ~EditorObjects();
      void LockSelected();
    private:
      void Display();
      void ObjectsListSwapPosition(GameObjectPtr object, Direction dir);
      bool DisplayObject(GameObjectPtr object, unsigned& id);
      
      void ContextMenu(GameObjectPtr object);
      bool IsLocked(GameObjectPtr object);
      bool IsParent(GameObjectPtr object);
      void AttachMenu();
      void Attach(GameObjectPtr);

      void CheckState();
      void ShowOverlay();
      bool Attaching;   
      bool Detaching;
      bool Deleting;


    };

  }
}