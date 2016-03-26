/*!****************************************************************************\
@file      String.h
@author    Christian Sagel
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

\******************************************************************************/
#pragma once

namespace DCEngine {
  namespace Utils {
    namespace String {

      std::string ToLower(std::string data);
      std::string ToLower(const char* data_c);
      std::string ToUpper(std::string data);
      std::string ToUpper(const char *data_c);
    


    }
  }
}