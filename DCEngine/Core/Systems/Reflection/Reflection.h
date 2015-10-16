/*****************************************************************************/
/*!
@file   Reflection.h
@author Christian Sagel, Chen Shu
@par    email: c.sagel\@digipen.edu
@date   9/12/2015
@brief  The reflection system provides
@note   The credit to my implementation of the Reflection system used in this 
        engine goes to Randy Gaul's C++ Reflection: Type-MetaData tutorial
        series:
        http://www.randygaul.net/2012/10/01/c-reflection-type-metadata-introduction/
        
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
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

      void ReflectionTest();


    };


  }


}