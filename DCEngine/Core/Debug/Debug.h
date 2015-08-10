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


  private:
    std::ofstream _file;

  }; // Trace

  extern std::unique_ptr<Trace> traceObj;
  #define trace (*::Debug::traceObj)

}
