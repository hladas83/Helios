#pragma once

#ifndef _HELIOS_IGAME
#define _HELIOS_IGAME

//===================================================================================================
// included dependencies

#include "../../common.h"

namespace Helios
{

  //===================================================================================================
  // interface for game 
  
  class IGame : public RefCount, public OLinkBase<IGame> 
  {

  public:
    IGame(){};

    //-----------------------------------------------------------------------------
    virtual void Draw() = 0;
    virtual void ProcessControls(UINT message, WPARAM wParam, LPARAM lParam) = 0;
    virtual bool IsGameRunning() const = 0;
  };

} // Helios namespace

#endif // _HELIOS_IGAME