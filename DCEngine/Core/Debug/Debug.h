/******************************************************************************/
/*!
\file   Debug.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#undef DCException

namespace DCEngine
{

  // Factory
#define DCE_TRACE_FACTORY_GAMEOBJECT_CONSTRUCTION 0
#define DCE_TRACE_FACTORY_COMPONENT_CONSTRUCTION 0
#define DCE_TRACE_FACTORY_COMPONENT_MAP 1
#define DCE_TRACE_FACTORY_RESOURCE_ADD 0

  namespace Debug {

#define TRACE_ON 1
#define TRACE_INITIALIZE 1
#define TRACE_UPDATE 0
#define TRACE_CONSTRUCTOR 0
#define TRACE_REGISTER 1
#define TRACE_CONNECT 0
#define TRACE_DISPATCH 0

#define DCE_TRACE_GAMEOBJECT_ADD 0
#define TRACE_GAMEOBJECT_INITIALIZE 0
#define DCE_TRACE_COMPONENT_ADD 0
#define TRACE_COMPONENT_INITIALIZE 0
#define DCE_TRACE_GAMEOBJECT_DESTRUCTOR 0
#define DCE_TRACE_COMPONENT_DESTRUCTOR 0





#define TRACE_PHYSICS 0
#define TRACE_GRAPHICS 0  
#define TRACE_SHADER 0
#define TRACE_AUDIO 0

#define DCE_EDITOR_TRACE_TOOLS 1


    class Trace {
    public:

      Trace(std::string fileName);
      ~Trace();

      template<typename T>
      Trace& operator<<(const T& data) {
      #ifndef _PRODUCTION
        std::cout << data;
        _file << data;
        return *this;
      #else
        return *this;
      #endif
        
      }

    private:
      std::ofstream _file;
    };

    class Exception {
    public:
      Exception(std::string file, int line, const std::string message);
      friend std::ostream& operator<<(std::ostream &os, const Exception ex);

    private:
      std::string ExceptionMessage;
      int LineNumber;
      std::string FileName;
    };

    extern std::unique_ptr<Trace> traceObj;
#define DCTrace (*::DCEngine::Debug::traceObj) 

  }
}

#define DCException(string) Debug::Exception(__FILE__, __LINE__, string)
