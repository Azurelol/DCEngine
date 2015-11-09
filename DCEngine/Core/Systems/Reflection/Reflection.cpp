#include "Reflection.h"

namespace DCEngine {
  
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Reflection system constructor.
    */
    /**************************************************************************/
   Reflection::Reflection() : System(std::string("ReflectionSystem"), EnumeratedSystem::Reflection) {
     DCTrace << "*Using Zilch to bind Object classes and provide reflection \n";
     ZilchHandler.reset(new ZilchInterface());
    }

   /**************************************************************************/
   /*!
   @brief  Initializes the Reflection system.
   */
   /**************************************************************************/
    void Reflection::Initialize()
    {
      ZilchHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    @brief  Updates the Reflection system.
    */
    /**************************************************************************/
    void Reflection::Update(float dt)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Terminates the Reflection system.
    */
    /**************************************************************************/
    void Reflection::Terminate()
    {
      ZilchHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a pointer to a BoundType object, which contains information
            about the object.
    */
    /**************************************************************************/
    Zilch::BoundType* Reflection::getBoundType(std::string name, Zilch::LibraryRef library)
    {
      return ZilchHandler->getBoundType(name, library);
    }


  }


}