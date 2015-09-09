/******************************************************************************/
/*!
\file   Entity.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The Game Object composition class. Spaces are comprised of GameObject
        entities.

*/
/******************************************************************************/
#pragma once
#include "..\Entity.h"

namespace DCEngine {

  // FORWARD-DECLARATIONS //
  class Space;
  class GameSession;

  class GameObject : public Entity {
    friend class Space;

  public:
    GameObject(std::string name, Space& space, GameSession& gamesession);
    GameObject() {
      _name = "GameObject";
    }

    Space* GetSpace();
    GameSession* GetGameSession();  
    

  private:
    Space* space_;
    GameSession* gamesession_;

  };

  using GameObjectPtr = std::shared_ptr<GameObject>;
  using GameObjectVec = std::vector<GameObjectPtr>;

}