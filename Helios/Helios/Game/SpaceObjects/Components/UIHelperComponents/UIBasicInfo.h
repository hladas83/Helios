#pragma once

//===================================================================================================
// included dependencies

#include "../../../common.h"

//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class UIBasicInfo : public RefCount
  {
  private:
    //! color in UI
    HColor _infoColor;

  public:
    UIBasicInfo() : _infoColor(1.0f, 1.0f, 1.0f, 1.0f) {};

    HColor GetInfoColor() const { return _infoColor; };
    void SetInfoColor(HColor color) { _infoColor = color; };
  };

 

} //namespace
