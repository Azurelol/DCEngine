/*********************
*********************************************************/
/*!
@file   Debug.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   7/30/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Debug.h"

#undef DCException

namespace DCEngine
{
  namespace Debug {

    std::unique_ptr<Trace> traceObj = nullptr;

    /**************************************************************************/
    /*!
    \brief  Creates the DCTrace object.
    \param  sentence
    */
    /**************************************************************************/
    Trace::Trace(std::string fileName) {
    #ifndef _PRODUCTION


      _file.open(fileName.c_str(), std::ios::trunc);

      _file.exceptions(std::ofstream::badbit | std::ofstream::failbit);

      if (!_file) {
        std::cerr << "Cannot open log file " << std::endl;
      }

      std::cout << "Debug::Trace - Log file: " << fileName << std::endl;
    #endif
    }

    /**************************************************************************/
    /*!
    \brief  Destructor. Closes the file.
    \param  sentence
    */
    /**************************************************************************/
    Trace::~Trace() {
    #ifndef _PRODUCTION
      _file.close();
    #endif
      //std::cout << "Debug::~Trace - Closing" << std::endl;
    }

    /**************************************************************************/
    /*!
    @brief Specific constructor for the exception, where a 'string' message
           is provided for the object to carry back it up as it unwinds
           through the program.
    @param file The filename.
    @param line The line number at which the exception occurred.
    @param message The specified message.
    */
    /**************************************************************************/
    Exception::Exception(std::string file, int line, const std::string message)
      : FileName(file), LineNumber(line), ExceptionMessage(message)
    {
    }

    /**************************************************************************/
    /*!
    @brief Friendly ostream operator prints the exception message.
    @param os A reference to the ostream object to insert the message into.
    @param The exception that is being printed out.
    @return A reference to the ostream, allowing for chaining the insertion
            operator.
    */
    /**************************************************************************/
    std::ostream & operator<<(std::ostream & os, const Exception exception)
    {
      os << "\n"
        << "////////////////////////////////////////////////////// \n"
        << " Daisy has encountered an exception! \n\n"
        << " Message: " << exception.ExceptionMessage << "\n"
        << " File: " << exception.FileName << "\n"
        << " Line: " << exception.LineNumber << "\n"
        << "////////////////////////////////////////////////////// \n";
      return os;
    }
  }
}

#define DCException(string) Debug::Exception(__FILE__, __LINE__, string)


/*
//// Takes a custom stream and returns it
//typedef Trace& (*TraceManip)(Trace&);
//// Takes in a function with the custom signature
//Trace& operator<<(TraceManip manip) {
//  // Call the function and return its value
//  return manip(*this);
//}

//static Trace& endl(Trace& stream) {
//  std::cout << std::endl;
//  //_file << std::endl;
//  return stream;
//}

//using CoutType = std::basic_ostream<char, std::char_traits<char>>;
//typedef CoutType& (*StandardEndline)(CoutType&);

//// Operator to take in std::endl
//Trace& operator<<(StandardEndline manip) {
//  manip(std::cout);
//  return *this;
//}

////Trace& operator<<(std::ostream& (*manip)(std::ostream&)) {
////  std::cout << std::endl;
////  _file << std::endl;
////  return *this;
////}

////Trace& operator<<(std::ostream& (*manip)(std::ostream&)) {
////  if
////}

*/