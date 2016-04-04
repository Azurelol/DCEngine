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

        ZilchDeclareDerivedType(SoundSpace, Component);

        DCE_DEFINE_PROPERTY(Real, Volume);
        DCE_DEFINE_PROPERTY(Real, Pitch);
        DCE_DEFINE_PROPERTY(Boolean, Pause);

        void PlayCueAt(std::string soundCueName);
        SoundInstanceWeakPtr PlayCue(std::string soundCueName);
        SoundInstanceWeakPtr PlayCue(SoundCuePtr soundCue);
        SoundInstanceWeakPtr PlayCueByHandle(std::string);
        static void ZilchPlayCue(Zilch::Call& call, Zilch::ExceptionReport& report);

        SoundSpace::SoundSpace(Entity & owner);
        void Initialize();

      private:
        bool Pause;
        Real Volume;
        Real Pitch;
        std::vector<SoundInstanceHandle> ActiveSoundInstances;
        std::vector<SoundInstancePtr> ActiveSoundInstancePtrs;
        void OnLogicUpdate(Events::LogicUpdate* event);        
        void Clear();

      };
    }

  

}