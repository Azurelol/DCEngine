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
      EditorObjects(Editor& editor);
      ~EditorObjects();
    private:
      void Display();
      void ObjectsListSwapPosition(GameObjectPtr object, Direction dir);
      bool DisplayObject(GameObjectPtr object);
      
      void ContextMenu(GameObjectPtr object);
      void AttachMenu();
      void Attach(GameObjectPtr);

      void CheckState();
      bool Attaching;   
      bool Detaching;
      bool Deleting;


    };

  }
}