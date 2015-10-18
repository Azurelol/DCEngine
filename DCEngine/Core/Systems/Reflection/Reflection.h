/*****************************************************************************/
/*!
@file   Reflection.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/16/2015
@brief  The reflection system provides
@note   The credit to my implementation of the Reflection system used in this 
        engine goes to Randy Gaul. More information in the implementation
        header.
        
*/
/******************************************************************************/
#pragma once
#include "../System.h"

#include "ReflectionMeta.h"

namespace DCEngine {

  class Engine;

  namespace Systems {

    class Reflection : public System {
      friend class Engine;
    public:


    private:
      Reflection();
      void Initialize();
      void Update(float dt);
      void Terminate();

      void RegisterTypes();

      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);
    };


  }


}