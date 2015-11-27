/******************************************************************************/
/*!
@file   CoreBinding.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/07/2015
@brief  This file includes the definition of the static library for all the
        Core engine classes (Object, Entities, Components).
        It will also initialize them.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Precompiled.h"
#include "CoreBinding.h"

// Core Objects
#include "..\Objects/Object.h"
// Core Entities
#include "..\Objects\Entities\EntitiesInclude.h"
// Core Components
#include "..\ComponentsInclude.h"

namespace DCEngine {  

  ZilchDefineStaticLibrary(DCEngineCore) {

    /*
    We have to initialize all the types that we have bound to our library.
    Ideally we could use pre-main or automatic registration, but there's a major issues where
    compilers automatically remove "unreferenced" classes, even if they are referenced
    by globals/pre-main initializations. This method ensures that all classes will be properly bound.
    */

    // Objects
    ZilchInitializeType(Object);
    ZilchInitializeType(Entity);
    ZilchInitializeType(Resource);
    ZilchInitializeType(Component);
    ZilchInitializeType(GameSession);
    ZilchInitializeType(Space);
    ZilchInitializeType(GameObject);

    // Resources
    ZilchInitializeType(SpriteSource);
    ZilchInitializeType(SoundCue);

    // Components

    // Spaces
    ZilchInitializeType(PhysicsSpace);
    ZilchInitializeType(GraphicsSpace);
    ZilchInitializeType(SoundSpace);
    ZilchInitializeType(TimeSpace);
    // Physics
    ZilchInitializeType(Transform);
    ZilchInitializeType(RigidBody);
    ZilchInitializeType(BoxCollider);
    // Graphics
    ZilchInitializeType(Sprite);
    ZilchInitializeType(Camera);
    ZilchInitializeType(EditorCameraController);
    // Audio
    ZilchInitializeType(SoundSpace);
    ZilchInitializeType(SoundEmitter);
    
    // Space
    

  }
}