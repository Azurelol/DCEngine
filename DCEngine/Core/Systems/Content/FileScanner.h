/*!******************************************************************\
@file     FileScanner.hpp
@brief    FileScanner System. Scans the assets directory and 
          sends FileScannerEvents when files are changed.
@author   Christian Sagel, Johannes Peter
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
\********************************************************************/
#pragma once
#include "../../Objects/Module.h"

// Packages
#include "../Filesystem/FileSystem.h"
#include "../../Events/EngineEvents.h"

namespace DCEngine {
  namespace Systems {

    class FileScanner : public Module{
    public:
      //--------------------------------------------------------------//
      struct FSSettings {
        unsigned Frequency;
        std::string DirectoryPath;
        FSSettings() : Frequency(10) {}
      };
      using DirectoryScan = std::map<std::string, FileInfo>;
      using ScanPair = std::pair < std::string, FileInfo>;
      using ScanIter = std::map<std::string, FileInfo>::const_iterator;
      //--------------------------------------------------------------//
      FileScanner(FSSettings& settings);
      ~FileScanner();
      void Initialize();
      void RunThreadScan();
      void Scan();
      void OnWindowFocusGainedEvent(Events::WindowGainedFocus* event);
      void OnWindowLostFocusEvent(Events::WindowLostFocus* event);

    private:
      DirectoryScan* ScanHistory = nullptr;
      FSSettings Settings;
      std::thread ScanThread;
      bool Active;


    };

  }
}