#include "Shader.h"

namespace Helios
{
  //=================================================================================================== 

  Shader::Shader(const char *path): _path(path), _g_pVertexShader(nullptr), _g_pPixelShader(nullptr), _g_pVertexLayout(nullptr) 
  {
  };

  //------------------------------------------------------------------------------ 

  Shader::~Shader()
  {
    RELEASE_MEM(_g_pVertexShader);
    RELEASE_MEM(_g_pPixelShader);
    RELEASE_MEM(_g_pVertexLayout);
  };

  //--------------------------------------------------------------------------------------
  // Helper for compiling shaders with D3DX11
  //--------------------------------------------------------------------------------------
  HRESULT Shader::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
  {
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
      dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
    if (FAILED(hr))
    {
      if (pErrorBlob != nullptr)
        OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
      if (pErrorBlob) pErrorBlob->Release();
      return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
  }

  //------------------------------------------------------------------------------ 

  void Shader::InitDX11(ID3D11Device* _g_pd3dDevice)
  {
    HRESULT hr = S_OK;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;

    std::wstring widestr = std::wstring(_path.begin(), _path.end());
    const wchar_t* widecstr = widestr.c_str();

    hr = CompileShaderFromFile( widecstr, "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
      MessageBox( nullptr,
        L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
      return;
    }

    // Create the vertex shader
    hr = _g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_g_pVertexShader );
    if( FAILED( hr ) )
    {	
      pVSBlob->Release();
      return;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
    hr = _g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
      pVSBlob->GetBufferSize(), &_g_pVertexLayout );
    pVSBlob->Release();
    if( FAILED( hr ) )
      return;


    // Compile the pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(widecstr, "PS", "ps_4_0", &pPSBlob);
    if( FAILED( hr ) )
    {
      MessageBox( nullptr,
        L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
      return;
    }

    // Create the pixel shader
    hr = _g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_g_pPixelShader );
    pPSBlob->Release();
    if( FAILED( hr ) )
      return;
  }
}//namespace