/******************************************************************************/
/*!
@file   EditorInspectorPreview.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorInspector.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    static SoundInstanceHandle EditorSoundPreview;

    /**************************************************************************/
    /*!
    @brief  Previews an image on the Editor's properties inspector.
    @param  property A pointer to the property.
    @param  object A pointer to the object the property belongs to.
    @todo   Currently we are pulling the image from the Content system..
    */
    /**************************************************************************/
    void EditorInspector::PreviewImage(Zilch::Property * property, ObjectPtr object)
    {
      // Create an exception report object
      Zilch::ExceptionReport report;
      Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->GetState());
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      getCall.Invoke(report);
      // Grab the image's path
      auto imagePath = getCall.Get<Zilch::String>(Zilch::Call::Return);

      // Grab a pointer to the texture data of the image from the SpriteSource it's on
      auto spriteSource = dynamic_cast<SpriteSource*>(object);
      auto textureData = spriteSource->getTexture();
      //ImTextureID texID = textureData;
      ImGui::Image((void*)(textureData.TextureID), ImVec2(textureData.Width, textureData.Height),
        ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
    }

    /**************************************************************************/
    /*!
    @brief  Previews a sound on the Editor's properties inspector.
    @param  property A pointer to the property.
    @param  object A pointer to the object the property belongs to.
    @todo   Currently we are pulling the sound from the Content system..
    */
    /**************************************************************************/
    void EditorInspector::PreviewSound(Zilch::Property * property, ObjectPtr object)
    {
      static bool playedOnce = false;

      // Create an exception report object
      Zilch::ExceptionReport report;
      Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->GetState());
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      getCall.Invoke(report);
      // Grab the sound's path
      auto soundPath = getCall.Get<Zilch::String>(Zilch::Call::Return);

      // Grab the reference to the sound this SoundCue holds
      auto soundCue = dynamic_cast<SoundCue*>(object);

      if (ImGui::Button("Preview")) {
        // If it's loaded, call it to stop first
        if (playedOnce)
          EditorSoundPreview->Stop();

        EditorSoundPreview = Daisy->getSystem<Audio>()->PlaySound(std::string(soundCue->getObjectName()));
        playedOnce = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Stop Preview") && EditorSoundPreview) {
        EditorSoundPreview->Stop();
        //Daisy->getSystem<Audio>()->StopSound(std::string(soundCue->getObjectName()));
      }
    }


  }
}