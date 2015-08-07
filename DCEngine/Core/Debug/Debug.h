/******************************************************************************/
/*!
\file   Debug.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief

*/
/******************************************************************************/

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#pragma once

namespace Debug {
  
  class Trace {
  public:
    Trace(std::string fileName);
    ~Trace();    

    template<typename T> 
    Trace& operator<<(const T& data) {
      std::cout << data;
      _file << data;
      return *this;
    }

  private:
    std::ofstream _file;

  }; // Trace

  extern std::unique_ptr<Trace> traceObj;
  #define trace (*::Debug::traceObj)

}
