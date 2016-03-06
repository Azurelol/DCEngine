/*****************************************************************************/
/*!
\file   SystemConfiguration.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   3/3/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SystemsConfiguration.h"

#include "../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    GUIConfig::GUIConfig() : Style(ImGui::GetStyle())
    {
    }

    ConfigurationFiles & ConfigurationFiles::Access()
    {
      return Daisy->Configuration();
    }

  }
}