/******************************************************************************/
/*
  File:    Colors.h
  Author:  Alejandro Hitti
  Date:    11/25/14
  Brief:   A collection of colors for the editor's themes

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "ImGUI\imgui.h"

namespace Colors
{
#define RGB_TO_F(x) x / 255.0f

  namespace Monokai
  {
    // BGs
    ImVec3 BurnedClay    = { RGB_TO_F(39),  RGB_TO_F(40),  RGB_TO_F(34)  };

    // Buttons/Actives 1                                                                     
    ImVec3 BrightGreen   = { RGB_TO_F(166), RGB_TO_F(226), RGB_TO_F(42)  };
    ImVec3 PastelGreen   = { RGB_TO_F(106), RGB_TO_F(198), RGB_TO_F(46)  };
    ImVec3 DarkGreen     = { RGB_TO_F(39),  RGB_TO_F(109), RGB_TO_F(42)  };

    // Buttons/Actives 2                                                                    
    ImVec3 BrightBlue    = { RGB_TO_F(102), RGB_TO_F(217), RGB_TO_F(177) };
    ImVec3 PastelBlue    = { RGB_TO_F(61),  RGB_TO_F(163), RGB_TO_F(239) };
    ImVec3 DarkBlue      = { RGB_TO_F(39),  RGB_TO_F(105), RGB_TO_F(177) };

    // Buttons/Actives 3                                                                         
    ImVec3 BrightPink    = { RGB_TO_F(249), RGB_TO_F(38),  RGB_TO_F(114) };
    ImVec3 PastelPink    = { RGB_TO_F(185), RGB_TO_F(38),  RGB_TO_F(101) };
    ImVec3 DarkPink      = { RGB_TO_F(152), RGB_TO_F(39),  RGB_TO_F(34)  };

    // Highlights                                                                   
    ImVec3 BrightOrange  = { RGB_TO_F(230), RGB_TO_F(164), RGB_TO_F(63)  };
    ImVec3 PaleOrange    = { RGB_TO_F(230), RGB_TO_F(219), RGB_TO_F(90)  };
  }

  namespace Pumpkin
  {
    // BGs
    ImVec3 BlackPumpkin = { RGB_TO_F(79),  RGB_TO_F(37),  RGB_TO_F(10)  };
    ImVec3 NightSky     = { RGB_TO_F(75),  RGB_TO_F(72),  RGB_TO_F(85)  };
    ImVec3 Bye          = { RGB_TO_F(28),  RGB_TO_F(39),  RGB_TO_F(41)  };
    ImVec3 LightBrown   = { RGB_TO_F(113), RGB_TO_F(84),  RGB_TO_F(85)  };

    // Buttons/Actives
    ImVec3 Orange       = { RGB_TO_F(255), RGB_TO_F(117), RGB_TO_F(26)  };
    ImVec3 DarkOrange   = { RGB_TO_F(228), RGB_TO_F(84),  RGB_TO_F(0)   };
    ImVec3 PaleOrange   = { RGB_TO_F(241), RGB_TO_F(156), RGB_TO_F(8)   };

    // Highlights
    ImVec3 YellowOrange = { RGB_TO_F(241), RGB_TO_F(200), RGB_TO_F(8)   };
    ImVec3 WhiteOrange  = { RGB_TO_F(252), RGB_TO_F(244), RGB_TO_F(208) };
  }

#undef RGB_TO_F
}
