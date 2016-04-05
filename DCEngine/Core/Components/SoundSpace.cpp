/******************************************************************************/
/*!
@file  SoundSpace                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        .cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SoundSpace, "SoundSpace", DCEngineCore, builder, type) {
      //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SoundSpace);
      // Constructor / Destructor
      //ZilchBindConstructor(builder, type, SoundSpace, "owner", Entity&);
      //ZilchBindDestructor(builder, type, SoundSpace);
      // Properties

      Zilch::ParameterArray playCueParams; playCueParams.push_back(ZilchTypeId(Zilch::String));
      builder.AddBoundFunction(type, "PlayCue", &SoundSpace::ZilchPlayCue, playCueParams, ZilchTypeId(SoundInstance), Zilch::MemberOptions::None);
      //ZilchBindMethod(builder, type, &SoundSpace::PlayCue, (, "PlayCue", "soundCueName");
      DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Volume);
      DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Pitch);
      DCE_BINDING_DEFINE_PROPERTY(SoundSpace, Pause);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief  SoundSpace constructor.
    */
    /**************************************************************************/
    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner)     
    {
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the 'SoundSpace' component.
    */
    /**************************************************************************/
    void SoundSpace::Initialize() {
      Connect(SpaceRef, Events::LogicUpdate, SoundSpace::OnLogicUpdate);

      // Register this space to the sound system
      Daisy->getSystem<Systems::Audio>()->Register(*this);
    }

    /**************************************************************************/
    /*!
    @brief  Receives a LogicUpdate event.
    @param  event The update event.
    */
    /**************************************************************************/
    void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
    }

    /**************************************************************************/
    /*!
    @brief  Destroys all active sound instances played through this SoundSpace.
    */
    /**************************************************************************/
    void SoundSpace::Clear()
    {
      return;
      for (auto& instance : ActiveSoundInstances) {
        //SoundInstance::Dereference(instance)->Stop();
        instance.Delete();
      }
      for (auto& instance : ActiveSoundInstancePtrs) {
        //if (instance)
        //  instance->Stop();
        delete instance.get();
      }
      ActiveSoundInstances.clear();
      ActiveSoundInstancePtrs.clear();
    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue', returning a SoundInstance handle to it.
    @param  soundCueName The name of the 'SoundCue' to play.
    @return A SoundInstance, an object that acts as the particular instance
            of that playing of the sound cue.
    */
    /**************************************************************************/
    SoundInstanceWeakPtr SoundSpace::PlayCue(std::string soundCueName)
    {
      auto instance = Daisy->getSystem<Systems::Audio>()->PlaySound(soundCueName);
      ActiveSoundInstancePtrs.push_back(instance);
      return instance.get();
    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue', returning a SoundInstance handle to it.
    @param  soundCue A pointer to the SoundCue.
    @return A SoundInstance, an object that acts as the particular instance
            of that playing of the sound cue.
    */
    /**************************************************************************/
    SoundInstanceWeakPtr SoundSpace::PlayCue(SoundCuePtr soundCue)
    {
      auto instance = Daisy->getSystem<Systems::Audio>()->PlaySound(soundCue);
      ActiveSoundInstancePtrs.push_back(instance);
      return instance.get();
    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue' through the creation of a Zilch Handle.
    @param  soundCueName The name of the 'SoundCue' to play.
    @return A SoundInstanceHandle.
    */
    /**************************************************************************/
    SoundInstanceWeakPtr SoundSpace::PlayCueByHandle(std::string name)
    {
      auto instance = Daisy->getSystem<Systems::Audio>()->PlaySoundZilch(name);
      ActiveSoundInstances.push_back(instance);
      return SoundInstance::Dereference(instance);
    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue' through Zilch, returning a handle to it.
    @param  soundCueName The name of the 'SoundCue' to play.
    @return A SoundInstance, an object that acts as the particular instance
    of that playing of the sound cue.
    */
    /**************************************************************************/
    void SoundSpace::ZilchPlayCue(Zilch::Call& call, Zilch::ExceptionReport& report)
    {
      // Grab the name of the SoundCue as well as the SoundSpace to play it from
      auto name = call.Get<Zilch::String>(0);
      auto soundSpace = reinterpret_cast<Components::SoundSpace*>(call.GetHandle(Zilch::Call::This).Dereference());      
      // Create and instance, playing the sound immediately
      auto instance = Daisy->getSystem<Systems::Audio>()->PlaySoundZilch(name.c_str());
      soundSpace->ActiveSoundInstances.push_back(instance);
      // Return it
      call.Set(Zilch::Call::Return, instance);      
    }
    








  }
}