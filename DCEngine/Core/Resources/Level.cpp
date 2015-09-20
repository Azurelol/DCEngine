#include "Level.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Destructor.
  */
  /**************************************************************************/
  Level::~Level() {
  }

  /**************************************************************************/
  /*!
  \brief  Creates an entity, adds it to the level. (Temporary until
          there's proper serialization and archetypes)
  \return A pointer to the entity that was added.
  */
  /**************************************************************************/
  void Level::AddGameObject(GameObjectPtr gameObject) {
    GameObjects.push_back(gameObject);
  }


}


