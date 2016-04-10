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

        // Properties   
        void setVolume(const Real& volume); DCE_DEFINE_GETTER(Real, Volume);
        void setPause(const Boolean& pause); DCE_DEFINE_GETTER(Boolean, Pause);
        void setPitch(const Real& pitch);  DCE_DEFINE_GETTER(Real, Pitch);
        void InterpolateVolume(Real newVolume, Real time);
        void InterpolatePitch(Real newPitch, Real time);
        // PlayCue
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
        // Events
        void OnGameFocusInEvent(Events::GameFocusIn* event);
        void OnGameFocusOutEvent(Events::GameFocusOut* event);
        void OnLogicUpdateEvent(Events::LogicUpdate* event);        
        void Clear();
        // Methods
        void PauseAll();
        void ResumeAll();
        void StopAll();

      };
    }

  

}