#pragma once

#ifndef _HELIOS_SHADERSBANK
#define _HELIOS_SHADERSBANK

//===================================================================================================
// included dependencies

#include "../../common.h"
#include "../../Elementary/ItemsBankBase/ItemsBankBase.h"
#include "Shader.h"


namespace Helios
{
  //===================================================================================================
  // ShadersBank class
  class ShadersBank  : public Singleton<ShadersBank>,  public ItemsBankBaseDX11<Shader>
  {
    typedef ItemsBankBaseDX11 base;

  public:

    ShadersBank(){};
    virtual ~ShadersBank(){};

    virtual void Init() override;
  };

} // Helios namespace

#endif //_HELIOS_SHADERSBANK