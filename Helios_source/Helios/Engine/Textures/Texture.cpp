#include "Texture.h"

namespace Helios
{
  //===================================================================================================

  Texture::Texture(const char *path): 
    _path(path) 
  {
    _g_pTextureRV = nullptr;
  };

  //------------------------------------------------------------------------------

  Texture:: ~Texture()
  {
    RELEASE_MEM(_g_pTextureRV);
  };

  //------------------------------------------------------------------------------

  void Texture::InitDX11(ID3D11Device* _g_pd3dDevice)
  {
    // Load the Texture
    std::wstring widestr = std::wstring(_path.begin(), _path.end());
    const wchar_t* widecstr = widestr.c_str();

    D3DX11CreateShaderResourceViewFromFile( _g_pd3dDevice, widecstr, NULL, NULL, &_g_pTextureRV, NULL );
  }
}//namespace