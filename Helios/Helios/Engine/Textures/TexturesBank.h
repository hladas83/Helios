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
  // Texture bank type

  class TexturesBank : public ItemsBankBaseDX11<Texture, TexturesBank>
  {
  };

} // Helios namespace

#endif //_HELIOS_SHADERSBANK