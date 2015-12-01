/******************************************************************************/
/*!
\file   DebugAudio.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class DebugAudio : public Component {
  public:
    
    String Track1;
    String Track2;
    String Track3;
    String CurrentSoundCue;

    /* Initialize */
    DebugAudio(Entity& owner) : Component(std::string("DebugAudio"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    
    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void ChangeTrack(std::string& track);

  private:
  };
}