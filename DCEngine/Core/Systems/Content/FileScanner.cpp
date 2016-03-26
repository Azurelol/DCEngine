/*!******************************************************************\
@file     FileScanner.hpp
@brief    FileScanner System. Scans the assets directory and
sends FileScannerEvents when files are changed.
@author   Christian Sagel, Johannes Peter
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
\********************************************************************/
#include "FileScanner.h"

namespace DCEngine {
  namespace Systems {

    FileScanner::FileScanner() : Module("FileScanner")
    {
    }

    FileScanner::~FileScanner()
    {
    }

    void FileScanner::Initialize()
    {
    }

    void FileScanner::ScanByThread()
    {
    }

    void FileScanner::Scan()
    {
    }

    void FileScanner::OnWindowFocusGainedEvent(Events::WindowGainedFocus * event)
    {
    }

    void FileScanner::OnWindowLostFocusEvent(Events::WindowLostFocus * event)
    {
    }

  }
}