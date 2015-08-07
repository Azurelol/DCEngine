/*****************************************************************************/
/*!
\file   Gamestate.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  The

*/
/******************************************************************************/
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "..\..\Engine\Space.h"
#include "..\System.h"

namespace DCEngine {
  
  enum class EnumeratedGamestate {
    MainMenu,
    PauseMenu,
    Sandbox
  };

  class Gamestate : public System {

  public:

    Gamestate(std::string name, EnumeratedGamestate gamestate);

    virtual ~Gamestate() {}

    /*! @brief This function once by the engine when the gamestate is created. */
    virtual void Initialize() = 0;

    /*! @brief Called once per frame while the gamestate is active.
        @param The time elapsed since the last frame.  */
    virtual void Update(float) = 0;

    /*! @brief Called by the engine when this gamestate is being destroyed. */
    virtual void Terminate() = 0;

    /*! @brief this function is called when this gamestate becomes active due to 
        a gamestate on top of it being popped.
        @param The GamestateType value of the gamestate that was popped. */
    virtual void PoppedTo(EnumeratedGamestate gamestate) {}    


    inline EnumeratedGamestate GetState() const {
      return _state;      
    }

    inline const std::vector<std::string>& GetSpaces() const {
      return _spaces;
    }

    /*! @brief Exposes the name of the space that this gamestate's logic will be used 
        with. Only one space will have the gamestate's logic applied. The logical space. */
    const std::string& GetLogicalSpace() {
      return _logicalSpace;
    }

    bool CheckSpaceActive(std::string space);


  private:
    Gamestate() = delete; // No default construction
    
    // CONTAINERS //
    std::string _defaultSpace;
    std::vector<std::string> _spaces; //!< Spaces updated while this is the active gamestate.
    const EnumeratedGamestate _state; //!< What gamestate is this?
    std::string _logicalSpace; //!< Space that Update() function will be used on.

  protected: // Available only to this object
    void SetLogicalSpace(std::string); // Sets the logical space

    /*
      These functions register a space to be updated while the gamestate is active
    */
    
    inline void RegisterSpace(SpacePtr space) { // CASE #1: Pointer to the space
      _spaces.push_back(space->GetName());
    }
    inline void RegisterSpace(std::string name) { // CASE #2: Name of the space
      _spaces.push_back(name);
    }
        
    void DeRegisterSpace(std::string name) {
      // How to refactor this so it uses a range-based for loop?
      auto it = _spaces.begin();
      for (; it != _spaces.end(); ++it)
      {
        if (*it == name)
          break;

        if (it != _spaces.end())
          _spaces.erase(it);
      }      
    }


  };

  using GamestatePtr = std::shared_ptr<Gamestate>;

} // DCEngine
