#pragma once

#ifndef _HELIOS_SHADER
#define _HELIOS_SHADER

//===================================================================================================
// included dependencies

#include "../../common.h"

namespace Helios
{

  //===================================================================================================
  // ShadersBank class
  class Shader : public RefCount, public OLinkBase<Shader>
  {
  private:
    HString _path;

    ID3D11VertexShader*     _g_pVertexShader;
    ID3D11PixelShader*      _g_pPixelShader;
    ID3D11InputLayout*      _g_pVertexLayout;

    HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

  public:
    Shader(const char *path);
    virtual  ~Shader();

    //------------------------------------------------------------------------------
    __forceinline ID3D11PixelShader *GetPixelShader() const {return _g_pPixelShader;};
    __forceinline ID3D11VertexShader *GetVertexShader() const {return _g_pVertexShader;};
    __forceinline ID3D11InputLayout *Getlayout() const {return _g_pVertexLayout;};

    void InitDX11(ID3D11Device* _g_pd3dDevice);
  };

} // Helios namespace

#endif //_HELIOS_SHADER