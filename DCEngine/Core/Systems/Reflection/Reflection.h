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
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../System.h"

// Reflection provided through Zilch
#include "ZilchInterface.h"

namespace DCEngine {

  class Engine;

  namespace Systems {

    class Reflection : public System {
      friend class Engine;
    public:

      static std::string PropertyAsString(Zilch::Property*, ObjectPtr object);
      template <typename T>
      T GetProperty(Zilch::Property*);

     // static Zilch::Call Get(Zilch::Property*);
      std::vector<Zilch::BoundType*> AllComponents();
      ZilchInterface* Handler();

    private:
      Reflection();
      void Initialize();
      void Update(float dt);
      void Terminate();    

      std::unique_ptr<ZilchInterface> ZilchHandler;

    };


    template<typename T>
    inline T Reflection::GetProperty(Zilch::Property *)
    {
      return T();
    }

  }


}