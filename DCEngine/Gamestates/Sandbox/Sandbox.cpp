#include "Sandbox.h"

#include "../../Core/Engine/Engine.h"

#include "../../Core/Debug/Debug.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> ENGINE;

  namespace Gamestates {

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void DCEngine::Gamestates::Sandbox::Initialize() {
      std::cout << "Sandbox::Initialize" << std::endl;

      //SetLogicalSpace(ENGINE->);

      // Register the spaces to be updated within this system
      RegisterSpace(ENGINE->GetDefaultSpace());

      // Set which space this gamestate's Update() should be used with
      SetLogicalSpace(ENGINE->GetDefaultSpace());

      // Register for drawable objects

      // Add an object
      
    }



    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void DCEngine::Gamestates::Sandbox::Update(float dt) {
      // Do something here
    }

    /**************************************************************************/
    /*!
    \brief  Terminates all the systems
    */
    /**************************************************************************/
    void DCEngine::Gamestates::Sandbox::Terminate() {

    }
  }
}
