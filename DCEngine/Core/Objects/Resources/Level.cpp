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
  void Level::AddEntity(EntityPtr entity) {
    Entities.push_back(entity);
  }


}


