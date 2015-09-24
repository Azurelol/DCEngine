/*****************************************************************************/
/*!
\file   Physics.h
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics system...
*/
/******************************************************************************/
#pragma once
#include "../System.h"
#include "../../Components/PhysicsSpace.h"

namespace DCEngine {
  class Engine;

  namespace Systems {    
    class Physics : public System {
      friend class Engine;
    public:
      void RegisterSpace(PhysicsSpace& physicsSpace);

    private:

      std::vector<PhysicsSpace*> physicsSpaces_;

      Physics();
      void Initialize();
      void Update(float dt);
      void Terminate();
      
      void Integrate();
      void BroadPhaseDetection();
      void NarrowPhaseDetection();
      void Resolve();

    };

  }
}