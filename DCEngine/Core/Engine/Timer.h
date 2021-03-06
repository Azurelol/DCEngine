/******************************************************************************/
/*!
@file   Timer.h
@author Allan Deutsch, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   7/30/2015
@brief Functions include:
      - ScopeTimer        
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
/* More references;
  http://www.informit.com/articles/article.aspx?p=1881386&seqNum=2 */

#include <string>

namespace DCEngine {

  namespace Time {
    // A framework that relates a time point to real physical time
    using clock = std::chrono::high_resolution_clock;
    // A reference to a specific point in time, like one's birthday, etc
    //  high_resolution_clock is the clock with the shortest tick period
    using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using ms = std::chrono::milliseconds;
    using us = std::chrono::microseconds;

    struct FunctionTimeSlice {
      std::string Name;
      float Time;
      unsigned Calls;
      FunctionTimeSlice(std::string name, float time) : Name(name), Time(time), Calls(1) {}
    };
    //using FunctionTimeSlice = std::pair<std::string, float>;
    using FunctionTimeSliceVec = std::vector<FunctionTimeSlice>;
    using FunctionTimes = std::pair<FunctionTimeSliceVec, FunctionTimeSliceVec>;


    class Timer {
    public:

      enum class Mode {
        StopWatch,
        Countdown,
      };

      Timer(float duration, Mode mode, bool reset);
      bool Update();
      bool Update(float dt);
      float CurrentTime;
      
    private:
      float Duration;
      bool Reset;
      Mode CurrentMode;      
    };

    // Durations measure time spans, like one minute, two hours, ten ms
    using floatSeconds = std::chrono::duration<float>;


    class ScopeTimer {
    public:
      /**************************************************************************/
      /*!
      \brief
      The constructor..
      \param output
      A pointer to a float.
      */
      /**************************************************************************/
      ScopeTimer(float* output) : StartTime(Time::clock::now()), _RetVal(output) {}

      /**************************************************************************/
      /*!
      \brief
      The destructor. When the timer leaves the scope, gets the end time. It then
      converts the duration to a float where 1.0f == 1 second.
      It then stores the result in a pointer given at initialization.
      */
      /**************************************************************************/
      ~ScopeTimer() {

        EndTime = Time::clock::now();
        Time::floatSeconds duration = EndTime - StartTime;
        *_RetVal = duration.count();

      }

    private:
      Time::timePoint StartTime;
      Time::timePoint EndTime;
      float* _RetVal;
    };


    /**************************************************************************/
    /*!
    @class FunctionTimer Times a function, invoking a report method on the
           destructor.
    */
    /**************************************************************************/
    class FunctionTimer {
    public:
      FunctionTimer() : StartTime(Time::clock::now()) {}
      void Record();
      virtual void Report() = 0;
      float ElapsedTime;

    private:
      Time::timePoint StartTime;
      Time::timePoint EndTime;
    };


    


  }

};