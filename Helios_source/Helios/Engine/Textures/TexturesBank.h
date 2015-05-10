#pragma once

#ifndef _HELIOS_TEXTURESBANK
#define _HELIOS_TEXTURESBANK

//===================================================================================================
// included dependencies

#include "../../common.h"
#include "../../Elementary/ItemsBankBase/ItemsBankBase.h"
#include "Texture.h"

namespace Helios
{

  //===================================================================================================
  // ShadersBank class
  class TexturesBank : public Singleton<TexturesBank>,  public ItemsBankBaseDX11<Texture>
  {
    typedef ItemsBankBaseDX11 base;

  public:
    TexturesBank();
    virtual ~TexturesBank();

    virtual void Init() override;
  };

} // Helios namespace

#endif //_HELIOS_SHADERSBANK