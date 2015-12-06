#pragma once

//===================================================================================================
// included dependencies

#include "../../../common.h"
#include "../Engine/Components/ComponentHolder.h"

//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class UIBasicInfo : public ComponentHolder
  {
    typedef ComponentHolder base;

  private:
    //! color in UI
    HColor _infoColor;

  public:
    UIBasicInfo() : _infoColor(1.0f, 1.0f, 1.0f, 1.0f) {};
    virtual void InitClass(ComponentHolder *parent, WParamItem &entityConfig) override;

    HColor GetInfoColor() const { return _infoColor; };
    void SetInfoColor(HColor color) { _infoColor = color; };

    USE_CASTING(base);
    DECL_COMPONENTHOLDER_FACTORY_REG(UIBasicInfo);
  };

 

} //namespace
