/*!******************************************************************\
@file     FileScanner.hpp
@brief    FileScanner System. Scans the assets directory and
sends FileScannerEvents when files are changed.
@author   Christian Sagel, Johannes Peter
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
\********************************************************************/
#include "FileScanner.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief FileScanner constructor.
    */
    /**************************************************************************/
    FileScanner::FileScanner(FSSettings& settings) : Module("FileScanner"), Settings(settings)
    {
      ScanHistory = new DirectoryScan();
    }

    /**************************************************************************/
    /*!
    @brief FileScanner destructor.
    */
    /**************************************************************************/
    FileScanner::~FileScanner()
    {
      Active = true;

      // End the scanning thread
      if (ScanThread.joinable())
        ScanThread.join();

      delete ScanHistory;
    }

    /**************************************************************************/
    /*!
    @brief Initializes the FileScanner.
    */
    /**************************************************************************/
    void FileScanner::Initialize()
    {
      // Scans the first time on the main thread
      Scan();
      // Connect to events
      Daisy->Connect<Events::WindowGainedFocus>(&FileScanner::OnWindowFocusGainedEvent, this);
      Daisy->Connect<Events::WindowLostFocus>(&FileScanner::OnWindowLostFocusEvent, this);
      // Start the file-scanning thread

    }

    /**************************************************************************/
    /*!
    @brief Scans when the window gains focus.
    */
    /**************************************************************************/
    void FileScanner::OnWindowFocusGainedEvent(Events::WindowGainedFocus * event)
    {
      Scan();
    }

    /**************************************************************************/
    /*!
    @brief Scans when the window loses focus.
    */
    /**************************************************************************/
    void FileScanner::OnWindowLostFocusEvent(Events::WindowLostFocus * event)
    {
      Scan();
    }
    /**************************************************************************/
    /*!
    @brief Initializes the FileScanner.
    */
    /**************************************************************************/
    void FileScanner::ScanByThread()
    {
    }

    void FileScanner::Scan()
    {
    }



  }
}