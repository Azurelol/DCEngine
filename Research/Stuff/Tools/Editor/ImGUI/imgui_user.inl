/******************************************************************************/
/*
  File:    imgui_user.inl
  Author:  Alejandro Hitti
  Date:    02/07/15
  Brief:   User file to extend ImGui functionality.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

bool VectorOfStringsItemGettor(void* data, int index, const char** outText)
{
  *outText = (*(std::vector<std::string>*)data)[index].c_str();

  return true;
}
