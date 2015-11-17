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

  class ShapesBank   : public  Singleton<ShapesBank>, public ItemsBankBaseDX11<Shape>
  {
    typedef ItemsBankBaseDX11 base;

  public:
    ShapesBank();
    virtual ~ShapesBank();

    virtual void Init() override;
    virtual void CleanupDX11Device();

    //! load new item, if not already present
    virtual bool LoadItem(const char* itemPath) override;
  };

} // Helios namespace

#endif