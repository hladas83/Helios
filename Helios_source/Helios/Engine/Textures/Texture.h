#pragma once

#ifndef _HELIOS_TEXTURE
#define _HELIOS_TEXTURE

//===================================================================================================
// included dependencies

#include "../../common.h"

namespace Helios
{
  //===================================================================================================
  // ShadersBank class
  class Texture : public RefCount, public OLinkBase<Texture> 
  {
    HString _path;
    ID3D11ShaderResourceView *_g_pTextureRV;

  public:
    Texture(const char *path);
    virtual ~Texture();

    void InitDX11(ID3D11Device* _g_pd3dDevice);
    __forceinline ID3D11ShaderResourceView *GetTexture() {return _g_pTextureRV;};
  };

} // Helios namespace

#endif //_HELIOS_TEXTURE