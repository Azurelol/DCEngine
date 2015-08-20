/******************************************************************************/
/*!
\file   Entity.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The x

*/
/******************************************************************************/
#include "Entity.h"

/**************************************************************************/
/*!
\brief  Checks if the entity has all of a set of components by OR-ing
        together multiple MaskComponente values.
\return True if the component has every specified component.
*/
/**************************************************************************/
bool DCEngine::Entity::CheckMask(mask m) {
  return ((_mask & m) == m);
}
