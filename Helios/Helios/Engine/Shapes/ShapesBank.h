#pragma once

#ifndef _SHAPES_BANK
#define _SHAPES_BANK

//===================================================================================================
// included dependencies


#include "../../common.h"
#include "../../Elementary/ItemsBankBase/ItemsBankBase.h"
#include "Shape.h"

namespace Helios
{

  //===================================================================================================
  // ShapesBank class

  class ShapesBank : public ItemsBankBaseDX11<Shape, ShapesBank>
  {
    typedef ItemsBankBaseDX11 base;

  public:
    ShapesBank();
    virtual ~ShapesBank();

    virtual void Init() override;
    //! load new item, if not already present
    virtual bool LoadItem(const char* itemPath) override;
  };

} // Helios namespace

#endif