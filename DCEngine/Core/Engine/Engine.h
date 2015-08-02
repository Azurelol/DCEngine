/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  

*/
/******************************************************************************/

#pragma once
#include <memory> // unique_ptr

#include "..\Systems\System.h"


namespace DCEngine {
  class Engine {
    public:

      Engine();
      void Initialize();
      
      void Update(float dt);
      void Loop();

      auto Stop() { _active = false; } // Signals the engine to stop running
      void Terminate();

      template<typename T> std::shared_ptr<T> GetSystem(EnumeratedSystem sysType);
      auto Getdt() { return dt; }
     // auto GetEngine() { return std::shared_ptr<Engine> = this; }


    private:
      float dt;
      float _FrameRate = 60.0f;
      bool _active;

      SystemVec _systems;


      
  };

  // DEFINE MACRO 
  #define GETSYSTEM( systype ) \
  ENGINE->GetSystem<systype>(ES_##systype)

  /**************************************************************************/
  /*!
  \brief  Allows access to a system running in the engine via system type.
  \note   This function call can be made much easier using the GETSYSTEM
          macro that takes a system typename and expands it to fill the
          template parameters.
  \param[in]  sysType
  \return A shared poitner to the requested system.
  */
  /**************************************************************************/
  template <typename T>
  std::shared_ptr<T> Engine::GetSystem(EnumeratedSystem sysType) {
    for (auto &it : _systems) {
      if (it->_type == sysType)
        return std::static_pointer_cast<T>(it);
    }

    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");

  }


}