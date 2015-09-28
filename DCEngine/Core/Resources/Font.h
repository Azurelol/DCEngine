#pragma once
#include "ResourceReference.h"

namespace DCEngine {

  class Font : public Resource {

    std::string DefaultFont = "";

    /**************************************************************************/
    /*!
    \brief Default constructor for the SpriteSource resource.
    */
    /**************************************************************************/
    Font() : Resource("Font") {
      
    }


  };

}