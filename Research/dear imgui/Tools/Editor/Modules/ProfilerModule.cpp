/******************************************************************************/
/*
  File:    ProfilerModule.cpp
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The profiler module displays graphs and metrics about the performance
           of the game at runtime.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "ProfilerModule.h"

#include "..\ServiceLocator.h"
#include "..\Utilities\FrameRateTimer\FrameRateTimer.h"

namespace Editor
{
  bool ProfilerModule::m_IsActive = false;
  bool ProfilerModule::m_IsVisible = true;
  float* ProfilerModule::systemsData = nullptr;
  std::string ProfilerModule::systemsNames = "";
  bool ProfilerModule::m_IsFirstTime = true;

  // Initializes the Module
  bool ProfilerModule::ProfilerModuleInitialize()
  {
    systemsData = new float[g_Profiler.GetMap().size()];

    return true;
  }

  // Checks if the module is currently active
  bool ProfilerModule::ProfilerModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void ProfilerModule::ProfilerModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool ProfilerModule::ProfilerModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void ProfilerModule::ProfilerModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  // Updates the module
  void ProfilerModule::ProfilerModuleUpdate()
  {
    if (!m_IsVisible || !m_IsActive)
      return;

    // Begin the Profiler module
    ImGui::Begin("Profiler", &m_IsActive, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::Center, -5, (int)(EDITOR->GetGFX()->GetResolution().height / 3.0f));
      m_IsFirstTime = false;
      ImGui::End();
      return;
    }

    // Display Framerate
    std::string fpsMessage = "";
    fpsMessage += "FPS: " + std::to_string(Utilities::FrameRateTimer::GetFramerate());
    ImGui::Text(fpsMessage.c_str());

    // Generates a histogram with the deltaTime of each system
    if (ImGui::CollapsingHeader("Systems Histogram"))
    {
      // Min and Max edge cases for the histogram chart
      static float minTime = 0.0f;
      static float maxTime = 0.016f;

      ImGui::PushItemWidth(90);
      // Slider float to adjust the min time
      ImGui::SliderFloat("Min Time", &minTime, 0.0f, 0.001f, "%.4f");
      SetHelpTooltip("Min time displayed in the histogram.");

      ImGui::SameLine();
      // Slider float to adjust the max time
      ImGui::SliderFloat("Max Time", &maxTime, 0.001f, 0.033f, "%.4f");
      SetHelpTooltip("Max time displayed in the histogram.");

      // Generates the systems histogram
      PlotSystemsHistogram(minTime, maxTime);
      ImGui::PushItemWidth(0);
    }
    
    // Generates individual line graphs for each system with the deltatime
    if (ImGui::CollapsingHeader("Systems Line Charts"))
    {
      // Min and Max edge cases for the histogram chart
      static float minTime2 = 0.0f;
      static float maxTime2 = 0.016f;

      ImGui::PushItemWidth(90);
      // Slider float to adjust the min time
      ImGui::SliderFloat("Min Time", &minTime2, 0.0f, 0.001f, "%.4f");
      SetHelpTooltip("Min time displayed in the line chart.");

      ImGui::SameLine();
      // Slider float to adjust the max time
      ImGui::SliderFloat("Max Time", &maxTime2, 0.001f, 1.0f, "%.4f");
      SetHelpTooltip("Max time displayed in the line chart.");

      PlotSystemLineChart(minTime2, maxTime2);
    }

    // End the Profiler module
    ImGui::End();
  }

  void ProfilerModule::PlotSystemsHistogram(float minTime, float maxTime)
  {
    auto map = g_Profiler.GetMap();

    // These variables are used to slow down the refresh rate of the histogram
    static int slowDown = 15;
    static int refreshTime = 15;

    // Adjusts the refresh time at runtime
    ImGui::Text("Update every");
    SetHelpTooltip("Frequency at which the histogram is being updated.");
    ImGui::SameLine();
    ImGui::SliderInt("frames", &refreshTime, 1, 60);
    SetHelpTooltip("Frequency at which the histogram is being updated.");
    ++slowDown;

    // Checks if the counter is higher than the regresh time
    if (slowDown > refreshTime)
    {
      // Clear the buffer
      systemsNames.clear();
      int i = 0;
      // Iterates through each system and generates the deltatime and name of the system
      for (auto it = map.begin(); it != map.end(); ++it, ++i)
      {
        systemsData[i] = (float)it->second.m_elapsedTime.count();

        systemsNames += std::to_string(i);
        systemsNames += ": ";
        systemsNames += it->first;
        systemsNames += "\n";
      }

      slowDown = 0;
    }

    ImGui::PushItemWidth(270);
    ImGui::PlotHistogram("", systemsData, map.size(), 0, "Systems", minTime, maxTime, ImVec2(0, 85));
    SetHelpTooltip("Shows the amount of time taken by each system.");
    ImGui::PushItemWidth(0);
    ImGui::SameLine();

    // Print the Legend for the systems
    ImGui::Text(systemsNames.c_str());
  }

  // Plots each system's line chart
  void ProfilerModule::PlotSystemLineChart(float minTime, float maxTime)
  {
    auto map = g_Profiler.GetMap();

    static bool pause;
    ImGui::Checkbox("Pause", &pause);
    SetHelpTooltip("Pauses the line chart update.");

    // Holds the vector of the past 100 recorded times for each system
    static std::vector<std::vector<float>> times;
    if (times.empty())
    {
      times.resize(map.size());
      for (unsigned i = 0; i < map.size(); ++i)
        times[i].resize(100);
    }

    // Holds the time offset of each system
    static std::vector<size_t> timesOffset;
    if (timesOffset.empty())
      timesOffset.resize(map.size());

    // For every system
    int i = 0;
    for (auto it = map.begin(); it != map.end(); ++it, ++i)
    {
      // Don't update if paused
      if (!pause)
      {
        // Calculate the current time and shift it by the time offset
        times[i][timesOffset[i]] = (float)map[it->first.c_str()].m_elapsedTime.count();
        timesOffset[i] = (timesOffset[i] + 1) % times[i].size();
      }

      // Plot the array
      ImGui::PushItemWidth(340);
      ImGui::PlotLines(std::to_string(i).c_str(), &times[i].front(), (int)times[i].size(), (int)timesOffset[i], it->first.c_str(), minTime, maxTime, ImVec2(0, 60));
      if (ImGui::IsHovered() && EditorManager::GetTooltipState())
        ImGui::SetTooltip("Amount of time taken by the %s", it->first.c_str());
    }
  }
}
