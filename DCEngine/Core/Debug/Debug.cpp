/******************************************************************************/
/*!
\file   Debug.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief

*/
/******************************************************************************/
#include "Debug.h"

namespace Debug {  

  std::unique_ptr<Trace> traceObj = nullptr;
  //#define trace (*::Debug::traceObj)
  //TracePtr traceObj = nullptr;
  

  /**************************************************************************/
  /*!
  \brief  Creates the trace object.
  \param  sentence  
  */
  /**************************************************************************/
  Trace::Trace(std::string fileName) {
    _file.open(fileName.c_str(), std::ios::trunc);

    _file.exceptions(std::ofstream::badbit | std::ofstream::failbit);

    if (!_file) {
      std::cerr << "Cannot open log file " << std::endl;
    }

    std::cout << "Debug::Trace - Log file: " << fileName << std::endl;
  }

  /**************************************************************************/
  /*!
  \brief  Destructor. Closes the file.
  \param  sentence
  */
  /**************************************************************************/
  Trace::~Trace() {
    _file.close();
    std::cout << "Debug::~Trace - Closing" << std::endl;
  }


} // Debug
