#pragma once
//#include <ZILCH\Zilch.hpp>

namespace DCEngine {
  namespace Systems {
    
    class Reflection;
    class ReflectionZilch {
      friend class Reflection;
    public:
      


    private:
      ReflectionZilch();
      void Initialize();
      void Update(float dt);
      void Terminate();

    };



  }
}