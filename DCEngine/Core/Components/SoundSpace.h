/******************************************************************************/
/*!
@file   SoundSpace.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  The SoundSpace component handles calls for playing sounds and forwards
        them to the Audio system.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

#include "../Resources/SoundInstance.h"

namespace DCEngine {
    class Entity;
    class SoundCue;
    class SoundEmitter;

    namespace Components
    {
      class SoundSpace : public Component {
        friend class Space;
        friend class SoundEmitter;

      public:

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
        ZilchDeclareDerivedType(SoundSpace, Component);
#endif

        DCE_DEFINE_PROPERTY(Real, Volume);
        DCE_DEFINE_PROPERTY(Real, Pitch);
        DCE_DEFINE_PROPERTY(Boolean, Pause);

        SoundInstanceHandle PlayCue(std::string soundCueName);
        void PlayCueAt(std::string soundCueName);
        void PauseCue(std::string soundCueName);
        void ResumeCue(std::string soundCueName);
        void StopCue(std::string soundCueName);

        SoundSpace::SoundSpace(Entity & owner);
        void Initialize();

      private:
        StringVec CuesCurrentlyPlaying;
        Real Volume; // Change the volume for all sounds in the space
        Real Pitch; // Pitch scale for all sounds in the space
        bool Pause; // Pause all sounds in the space

        void OnLogicUpdate(Events::LogicUpdate* event);
        void Update() {}

        void TestMusic();

      };
    }

  

}