/*****************************************************************************/
/*!
\file   Gamestate.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  

*/
/******************************************************************************/
#include "Gamestate.h"
#include "..\..\Debug\Debug.h"

#include "..\..\Config.h" // DefaultSpace

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  
  */
  /**************************************************************************/
  Gamestate::Gamestate(std::string name, EnumeratedGamestate gamestate) :
    System(name, EnumeratedSystem::Gamestate), _state(gamestate) {
    //_logicalSpace = name;
    std::cout << "Gamestate::Gamestate() - Initialized " << name << std::endl;
           // << " with _logicalSpace: " << _logicalSpace << std::endl;
  }

  /**************************************************************************/
  /*!
  \brief  
  */
  /**************************************************************************/
  bool Gamestate::CheckSpaceActive(std::string space) {
    for (auto it : _spaces)
      if (it == space)
        return true;
    return false;
  }

  /**************************************************************************/
  /*!
  \brief  
  */
  /**************************************************************************/
  void Gamestate::SetLogicalSpace(std::string name) {
    _logicalSpace = name;
    std::cout << "Gamestate::SetLogicalSpace - " << _logicalSpace
              << " has been set as the logical space." << std::endl;
  }

}


