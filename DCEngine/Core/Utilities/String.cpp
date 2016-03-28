/*!****************************************************************************\
@file      String.cpp
@author    Christian Sagel
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

\******************************************************************************/
#include "String.h"

namespace DCEngine {
  namespace Utils {
    namespace String {

      /*!************************************************************************\
      @brief  Makes a string lowercase.
      @param  data The reference to the string.
      @return The string, in lowercase.
      \**************************************************************************/
      std::string ToLower(std::string  data) {
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);
        return data;
      }

      /*!************************************************************************\
      @brief  Makes a string lowercase.
      @param  data The reference to the string.
      @return The string, in lowercase.
      \**************************************************************************/
      std::string ToLower(const char * data_c)
      {
        std::string data(data_c);
        return ToLower(data);
      }

      /*!************************************************************************\
      @brief  Makes a string uppercase..
      @param  data The reference to the string.
      @return The string, in lowercase.
      \**************************************************************************/
      std::string ToUpper(std::string data)
      {
        std::transform(data.begin(), data.end(), data.begin(), ::toupper);
        return data;
      }

      /*!************************************************************************\
      @brief  Makes a string uppercase..
      @param  data The reference to the string.
      @return The string, in lowercase.
      \**************************************************************************/
      std::string ToUpper(const char * data_c)
      {
        std::string data(data_c);
        return ToUpper(data);
      }

      /*!************************************************************************\
      @brief  Splits a string into a vector of strings given a delimiter.
      @param  data The reference to the string.
      @param  delim The delimiter to use.
      @return The string, in lowercase.
      \**************************************************************************/
      std::vector<std::string> Split(const std::string & data, std::string delims)
      {
        std::vector<std::string> words;
        std::size_t prevPos = 0, pos;
        while ((pos = data.find_first_of(delims.c_str(), prevPos)) != std::string::npos) {
          if (pos > prevPos)
            words.push_back(data.substr(prevPos, pos - prevPos));
          prevPos = pos + 1;
        }

        if (prevPos < data.length())
          words.push_back(data.substr(prevPos, std::string::npos));

        return words;
      }

    }
  }
}