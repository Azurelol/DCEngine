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
      Active = false;

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
      DCTrace << "FileScanner::Initialize: Scanning '" << Settings.DirectoryPath << "'\n";
      // Scans the first time on the main thread
      Scan();
      // Connect to events
      Daisy->Connect<Events::WindowGainedFocus>(&FileScanner::OnWindowFocusGainedEvent, this);
      Daisy->Connect<Events::WindowLostFocus>(&FileScanner::OnWindowLostFocusEvent, this);
      // Start the file-scanning thread
      Active = true;
      ScanThread = std::thread(&FileScanner::RunThreadScan, this);
    }

    /**************************************************************************/
    /*!
    @brief Scans when the window gains focus.
    */
    /**************************************************************************/
    void FileScanner::OnWindowFocusGainedEvent(Events::WindowGainedFocus * event)
    {
      //Scan();
    }

    /**************************************************************************/
    /*!
    @brief Scans when the window loses focus.
    */
    /**************************************************************************/
    void FileScanner::OnWindowLostFocusEvent(Events::WindowLostFocus * event)
    {
      //Scan();
    }
    /**************************************************************************/
    /*!
    @brief Main function for the scanner.
    */
    /**************************************************************************/
    void FileScanner::RunThreadScan()
    {
      try {
        while (Active) {
          Scan();
          std::this_thread::sleep_for(std::chrono::seconds(Settings.Frequency));
        }
      }
      catch (...) {
        DCTrace << "FileScanner::RunThreadScan: Failed to scan! \n";
      }
    }

    /**************************************************************************/
    /*!
    @brief Applies the scan.
    */
    /**************************************************************************/
    void FileScanner::Scan()
    {
      //DCTrace << "Scanning kyu~ \n";
      auto scanPathStr = boost::filesystem::initial_path().string() + "\\" + Settings.DirectoryPath;
      FileSystem::CorrectPath(scanPathStr);
      filepath scanPath = scanPathStr;
      if (!FileSystem::Exists(scanPath)) {
        DCTrace << "FileScanner::Scan: Could not find directory path! \n";
      }

      DirectoryScan* currentScan = new DirectoryScan();

      //    Create the recursive directory iterators
      boost::filesystem::recursive_directory_iterator dirIter(scanPath), end;

      // 1. Scan the current directory
      while (dirIter != end) {
        filepath path = dirIter->path();
        if (boost::filesystem::is_regular_file(path)) 
        {
          // Grab file info
          FileInfo info(path);

          // Track if the file should be skipped (nested loops)
          // Check if the file's extension is an ignored extension
          // Current file's extension matched an ignored type. Continue to next file.
          // Check if the file name is on the list of files to ignore.
          // Current file's name was in the ignore list. Continue to next file.

          // Grab a reference to the index in the map.
          FileInfo& scanRef = (*currentScan)[info.FileName];

          /* NOTE: Map subscript operators create elements if they don't exist,
             so we check for the default value of 'LastModified' */
          
          // If it alrady existed, it means we are trying to insert a filename twice,
          // when it should be unique. Unacceptable.
          if (scanRef.LastModified != 0) {
            DCTrace << "FileScanner:Scan: Duplicate filename in directory: " + scanPath.string()
              + "\n - File 1: " + info.FilePathAbs.string()
              + "\n - File 2: " + scanRef.FilePathAbs.string()
              + "\n Warning: Resoruce files must be uniquely named in the directory!";
          }

          // If it did not exist yet, we can safely insert via reference.
          scanRef = info;
        }
        ++dirIter;
      }

      ScanIter oldIt = ScanHistory->begin(), oldEnd = ScanHistory->end();
      ScanIter newIt = currentScan->begin(), newEnd = currentScan->end();

      // 2. We now have an old scan and a new one. Because both are ordered maps, we can walk them together
      //    When files exist in both, we send according events
      while (oldIt != oldEnd && newIt != newEnd) 
      {
        // a.) If the file still exists...
        if (oldIt->first == newIt->first) 
        {
          
          // If the file was moved...
          if (oldIt->second.FilePathAbs != newIt->second.FilePathAbs) {
            DispatchSystemEvents::ContentFileMoved(newIt->first);
          }
          // If the file was modified...
          else if (oldIt->second.LastModified != newIt->second.LastModified) {
            DispatchSystemEvents::ContentFileUpdated(newIt->first);
          }
          // Otherwise the file is the same..
          // Move both scans forward one element
          ++oldIt;
          ++newIt;
        }

        // b.) If the new scan contains files new files
        else if (oldIt->first > newIt->first) 
        {
          DispatchSystemEvents::ContentFileFound(newIt->first);
          ++newIt;
        }
        // c.) If the new scan is further along, and old has files that new did not, means the were deleted
        else 
        {
          DispatchSystemEvents::ContentFileDeleted(oldIt->first);
          ++oldIt;
        }
      }
      // 3. Any files left in oldIt must have been deleted
      while (oldIt != oldEnd) {
        DispatchSystemEvents::ContentFileDeleted(oldIt->first);
        ++oldIt;
      }
      // 4. Any files left in newIt must be new
      while (newIt != newEnd) {
        DispatchSystemEvents::ContentFileFound(newIt->first);
        ++newIt;
      }

      // Now that we are done with the current scan, store the new data as the historical data
      if (ScanHistory)
        delete ScanHistory;

      ScanHistory = currentScan;

    }



  }
}