#include "DX11Draw.h"
#include "../Engine.h"
#include "../../Game/Game.h"
#include "RenderObject.h"

#include "../Shapes/ShapesBank.h" 
#include "../Shaders/ShadersBank.h" 
#include "../Textures/TexturesBank.h" 
#include "../UI/UI.h" 

#include "CEGUI/CEGUI.h"

namespace Helios
{

  DX11DrawParams::DX11DrawParams(DX11Draw *dX11Draw, long tid, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow):
    _dX11Draw(dX11Draw), _tid(tid)
  {
    _hInstance = hInstance;
    _hPrevInstance = hPrevInstance;
    _lpCmdLine = lpCmdLine;
    _nCmdShow = nCmdShow;
  };

  //--------------------------------------------------------------------------------------

  DX11DrawParams::~DX11DrawParams(){};

  //--------------------------------------------------------------------------------------

  DEFINE_CASTING(DX11DrawParams);

  //===================================================================================================
  // thread task

  void RenderTask::Pefrom() 
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());
    DX11DrawParams *params = dyn_cast<DX11DrawParams>(_parametrs.GetRef());
    if(params)
    {
      params->_dX11Draw->InitGraphics( params->_hInstance,  params->_hPrevInstance,  params->_lpCmdLine,  params->_nCmdShow);
      if(params->_dX11Draw->GetDevice())
      {
        GEngine->GShapes()->InitDX11(GEngine->GDraw()->GetDevice());
        GEngine->GTextures()->InitDX11(GEngine->GDraw()->GetDevice());
        GEngine->GShaders()->InitDX11(GEngine->GDraw()->GetDevice());
      }
      params->_dX11Draw->DrawSpace(params->_tid);
    }
  };

  //===================================================================================================
  // DX11Draw

  DEFINE_CASTING(DX11Draw);

    DX11Draw::DX11Draw() 
  {
    _g_hInst = nullptr;
    _g_hWnd = nullptr;
    _g_driverType = D3D_DRIVER_TYPE_NULL;
    _g_featureLevel = D3D_FEATURE_LEVEL_11_0;
    _g_pd3dDevice = nullptr;
    _g_pImmediateContext = nullptr;
    _g_pSwapChain = nullptr;
    _g_pRenderTargetView = nullptr;
    _g_pDepthStencil = nullptr;
    _g_pDepthStencilView = nullptr;
    _g_pDSState = nullptr;
    _g_pConstantBuffer = nullptr;
    _g_World =   XMMatrixIdentity();
    _g_View =  XMMatrixIdentity();
    _g_Projection =  XMMatrixIdentity();
    _g_pSamplerLinear = nullptr;
    _g_pBlendState = nullptr;

    _g_RSwireFrame = nullptr;
    _g_RSSolid = nullptr;

    _width = 1024;
    _height = 768;
  };

  //--------------------------------------------------------------------------------------

  DX11Draw::~DX11Draw() {};

  //--------------------------------------------------------------------------------------

  void *DX11Draw::DrawSpace(int threadid)
  {
    long tid = (long)threadid;
    printf("Rendering thread #%ld started!\n", tid);

    // Main message loop
    MSG msg = {0};
    while(GEngine->GetIGame()->IsGameRunning() != false && WM_QUIT != msg.message )
    {
      if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
      else
      {
        Render();
      }
    }

    CleanupDevice();
    DX11Draw::Terminate();

    return nullptr;
  }

  //--------------------------------------------------------------------------------------

  bool DX11Draw::InitGraphics(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
  {
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
      return 0;

    if( FAILED( InitDevice() ) )
    {
      CleanupDevice();
      return 0;
    }

    UI::GetInstance()->SetDevice(GetDevice(), GetDeviceContext());

    return 1;
  }


  //--------------------------------------------------------------------------------------
  // Create Direct3D device and swap chain
  //--------------------------------------------------------------------------------------
  HRESULT DX11Draw::InitDevice()
  {
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( _g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
      _g_driverType = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain( nullptr, _g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
        D3D11_SDK_VERSION, &sd, &_g_pSwapChain, &_g_pd3dDevice, &_g_featureLevel, &_g_pImmediateContext );
      if( SUCCEEDED( hr ) )
        break;
    }
    if( FAILED( hr ) )
      return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
      return hr;

    hr = _g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &_g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
      return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = _g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &_g_pDepthStencil );
    if( FAILED( hr ) )
      return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = _g_pd3dDevice->CreateDepthStencilView( _g_pDepthStencil, &descDSV, &_g_pDepthStencilView );
    if( FAILED( hr ) )
      return hr;

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    _g_pd3dDevice->CreateDepthStencilState(&dsDesc, &_g_pDSState);

    _g_pImmediateContext->OMSetRenderTargets( 1, &_g_pRenderTargetView, _g_pDepthStencilView );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _g_pImmediateContext->RSSetViewports( 1, &vp );

     // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = _g_pd3dDevice->CreateSamplerState( &sampDesc, &_g_pSamplerLinear );
    if( FAILED( hr ) )
        return hr;

    // Create the blending
    D3D11_BLEND_DESC BSDesc;
    ZeroMemory(&BSDesc, sizeof(D3D11_BLEND_DESC));

    BSDesc.RenderTarget[0].BlendEnable = TRUE;
    BSDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BSDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BSDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BSDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    BSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BSDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BSDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


    hr = _g_pd3dDevice->CreateBlendState(&BSDesc, &_g_pBlendState);
    if( FAILED( hr ) )
      return hr;

    // Create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBufferShape);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = _g_pd3dDevice->CreateBuffer( &bd, nullptr, &_g_pConstantBuffer );
    if( FAILED( hr ) ) 
      return hr;

    // Initialize the world matrices
    _g_World = XMMatrixIdentity();

    // Initialize the view matrix
    GEngine->GetCameraMatrix(_g_View);

    // Initialize the projection matrix
    _g_Projection = XMMatrixPerspectiveFovLH( XM_PI / 3, width / (FLOAT)height, 0.01f, 100000.0f );

    InitRenderStates();

    return S_OK;
  }

  //--------------------------------------------------------------------------------------

  HRESULT DX11Draw::InitRenderStates()
  {
    //wireframe render state
    D3D11_RASTERIZER_DESC RSWireFrameDesc; 
    ZeroMemory(&RSWireFrameDesc, sizeof(D3D11_RASTERIZER_DESC));
    RSWireFrameDesc.FillMode = D3D11_FILL_WIREFRAME; 
    RSWireFrameDesc.CullMode = D3D11_CULL_BACK; 
    RSWireFrameDesc.DepthClipEnable = true;

    //solid render state
    D3D11_RASTERIZER_DESC RSSolidDesc; 
    ZeroMemory(&RSSolidDesc, sizeof(D3D11_RASTERIZER_DESC));
    RSSolidDesc.FillMode = D3D11_FILL_SOLID; 
    RSSolidDesc.CullMode = D3D11_CULL_BACK; 
    RSSolidDesc.DepthClipEnable = true;

    _g_pd3dDevice->CreateRasterizerState ( &RSWireFrameDesc , &_g_RSwireFrame );  
    _g_pd3dDevice->CreateRasterizerState ( &RSSolidDesc , &_g_RSSolid );  

    //as default, use solid
    _g_pImmediateContext->RSSetState ( _g_RSSolid ); 
    _g_pImmediateContext->PSSetSamplers( 0, 1, &_g_pSamplerLinear );
    _g_pImmediateContext->OMSetBlendState(_g_pBlendState, 0, 0xffffffff);

    return S_OK;
  }

  //--------------------------------------------------------------------------------------
  // Register class and create window
  //--------------------------------------------------------------------------------------
  HRESULT DX11Draw::InitWindow( HINSTANCE hInstance, int nCmdShow )
  {
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_HELIOS );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"HeliosWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_HELIOS );
    if( !RegisterClassEx( &wcex ) )
      return E_FAIL;

    // Create window
    _g_hInst = hInstance;
    RECT rc = { 0, 0, 1024, 768 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    _g_hWnd = CreateWindow( L"HeliosWindowClass", L"Helios", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
      nullptr );
    if( !_g_hWnd )
      return E_FAIL;

    ShowWindow( _g_hWnd, nCmdShow );

    return S_OK;
  }


  //------------------------------------------------------------------------------

  Vector3 DX11Draw::WorldToScreen(Vector3 postion)
  {
    XMFLOAT4 position = XMFLOAT4(postion.X(),postion.Y(),postion.Z(),1);
    XMVECTOR vectorPos = XMLoadFloat4(&position);

    vectorPos = XMVector4Transform( vectorPos, _g_View );
  //  vectorPos = XMVector4Transform( vectorPos, _g_Projection );

    vectorPos.x /= vectorPos.w;
    vectorPos.y /= vectorPos.w;
    vectorPos.z /= vectorPos.w;
    vectorPos.w = 1;

    return Vector3(vectorPos.x,vectorPos.y,vectorPos.z);
  }

  //--------------------------------------------------------------------------------------
  // Render the frame
  //--------------------------------------------------------------------------------------


  void DX11Draw::Render()
  {
    // Update our time
    static float deltaT = 0.0f;
    if( _g_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
      deltaT += ( float )XM_PI * 0.0125f;
    }
    else
    {
      static DWORD dwTimeStart = 0;
      DWORD dwTimeCur = GetTickCount();
      if( dwTimeStart == 0 )
        dwTimeStart = dwTimeCur;
      deltaT = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

    // Rotate cube around the origin
    // _g_World = XMMatrixRotationY(  deltaT );

    // Setup our lighting parameters
    XMFLOAT4 vLightPos[2] =
    {
      XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ),
      XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ),
    };
    XMFLOAT4 vLightColors[2] =
    {
      XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ),
      XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f )
    };
    XMFLOAT4 vAnimSources = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
    XMFLOAT4 vCameraPosition = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
    
    // Bind depth stencil state
    _g_pImmediateContext->OMSetDepthStencilState(_g_pDSState, 1);

    // Clear the back buffer
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha
    _g_pImmediateContext->ClearRenderTargetView( _g_pRenderTargetView, ClearColor );

    // Clear the depth buffer to 1.0 (max depth)
    ClearZBuffer();

    //check if any game is hooked
    CheckNull(GEngine->GetIGame());

    //prepaire content for drawing
    GEngine->GetIGame()->Draw();
    GEngine->GetCameraMatrix(_g_View);

    // Update matrix variables and lighting variables
    _cbShape.mWorld = XMMatrixTranspose( _g_World );
    _cbShape.mView = XMMatrixTranspose( _g_View );
    _cbShape.mProjection = XMMatrixTranspose( _g_Projection );
    _cbShape.vLightPos[0] = vLightPos[0];
    _cbShape.vLightPos[1] = vLightPos[1];
    _cbShape.vLightColor[0] = vLightColors[0];
    _cbShape.vLightColor[1] = vLightColors[1];
    _cbShape.vAnimSources = vAnimSources;
    _cbShape.vCameraPosition = vCameraPosition;
    _cbShape.vOutputColor = XMFLOAT4(0, 0, 0, 0);

    _cbLine.mWorld = XMMatrixTranspose( _g_World );
    _cbLine.mView = XMMatrixTranspose( _g_View );
    _cbLine.mProjection = XMMatrixTranspose( _g_Projection );

    _cbIcon.mWorld = XMMatrixTranspose( _g_World );
    _cbIcon.mView = XMMatrixTranspose( _g_View );
    _cbIcon.mProjection = XMMatrixTranspose( _g_Projection );

    _g_pImmediateContext->UpdateSubresource( _g_pConstantBuffer, 0, nullptr, &_cbShape, 0, 0 );

    //render scene 
    _rendermanager.RenderAll();

    //render UI
    CEGUI::System::getSingleton().renderAllGUIContexts();

    _g_pSwapChain->Present( 0, 0 );
  }

  //--------------------------------------------------------------------------------------

  void DX11Draw::RenderObject(DrawContext &drawContext)
  {
    _rendermanager.RenderObject(drawContext);
  }

  //--------------------------------------------------------------------------------------

  XMMATRIX Matrix4ToXMMATRIX(Matrix4 *matrix, float sizeX, float sizeY, float sizeZ)
  {
    XMMATRIX xmmatrix = XMMatrixIdentity();
    Vector3 position = matrix->GetPosition();
    Matrix3 rotation = matrix->GetRotation();

    xmmatrix._11 = rotation.GetAside().X() * sizeX;
    xmmatrix._21 = rotation.GetAside().Y() * sizeX;
    xmmatrix._31 = rotation.GetAside().Z() * sizeX;

    xmmatrix._12 = rotation.GetUp().X() * sizeY;
    xmmatrix._22 = rotation.GetUp().Y() * sizeY;
    xmmatrix._32 = rotation.GetUp().Z() * sizeY;

    xmmatrix._13 = rotation.GetDir().X() * sizeZ;
    xmmatrix._23 = rotation.GetDir().Y() * sizeZ;
    xmmatrix._33 = rotation.GetDir().Z() * sizeZ;

    xmmatrix._14 = position.X();
    xmmatrix._24 = position.Y();
    xmmatrix._34 = position.Z();

    return xmmatrix;
  }

  //--------------------------------------------------------------------------------------

  void DX11Draw::RenderShape(DrawContext &drawContext)
  {
    if(!drawContext._renderObject || 
      !drawContext._renderObject->GetShape() ||
      !drawContext._renderObject->GetShader()
      ) return;

    // Set the input layout
    _g_pImmediateContext->IASetInputLayout( drawContext._renderObject->GetShader()->Getlayout());

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    XMVECTOR cameraPos = GEngine->GetCameraPosition();

    Matrix4 transform = drawContext._renderObject->GetTransform() * drawContext._transform; 
    _cbShape.mWorld = Matrix4ToXMMATRIX(&transform, drawContext._sizeX, drawContext._sizeY, drawContext._sizeZ);
    _cbShape.vAnimSources = XMFLOAT4(drawContext._animationPeriod,0,0,0);
    _cbShape.vCameraPosition = XMFLOAT4(cameraPos.x,cameraPos.y,cameraPos.z,cameraPos.w);

    //todo - identify correct _cbShape
    _g_pImmediateContext->UpdateSubresource( _g_pConstantBuffer, 0, nullptr, &_cbShape, 0, 0 );
    // Set vertex buffer
    _g_pImmediateContext->IASetVertexBuffers( 0, 1, &(drawContext._renderObject->GetShape()->_g_pVertexBuffer), &stride, &offset );
    // Set index buffer
    _g_pImmediateContext->IASetIndexBuffer( drawContext._renderObject->GetShape()->_g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
    // Set primitive topology
    _g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Render shape
    _g_pImmediateContext->VSSetShader( drawContext._renderObject->GetShader()->GetVertexShader(), nullptr, 0 );
    _g_pImmediateContext->VSSetConstantBuffers( 0, 1, & _g_pConstantBuffer );
    _g_pImmediateContext->PSSetShader( drawContext._renderObject->GetShader()->GetPixelShader(), nullptr, 0 );
    _g_pImmediateContext->PSSetConstantBuffers( 0, 1, & _g_pConstantBuffer );

    //Todo
    ID3D11ShaderResourceView *texture0 = drawContext._renderObject->GetTexture(0);
    _g_pImmediateContext->PSSetShaderResources( 0, 1, &texture0 );
    ID3D11ShaderResourceView *texture1 = drawContext._renderObject->GetTexture(1);
    _g_pImmediateContext->PSSetShaderResources( 1, 1, &texture1 );
    ID3D11ShaderResourceView *texture2 = drawContext._renderObject->GetTexture(2);
    _g_pImmediateContext->PSSetShaderResources( 2, 1, &texture2 );
    ID3D11ShaderResourceView *texture3 = drawContext._renderObject->GetTexture(3);
    _g_pImmediateContext->PSSetShaderResources( 3, 1, &texture3 );

    _g_pImmediateContext->PSSetSamplers( 0, 1, &_g_pSamplerLinear );
    _g_pImmediateContext->OMSetBlendState(_g_pBlendState, 0, 0xffffffff);
    _g_pImmediateContext->DrawIndexed( drawContext._renderObject->GetShape()->GetIindicesCount(), 0, 0 );

  }

  //--------------------------------------------------------------------------------------

  void DX11Draw::RenderLine(DrawContext &drawContext)
  {
    if(!drawContext._renderObject || 
      !drawContext._renderObject->GetShape() ||
      !drawContext._renderObject->GetShader()
      ) return;


    // Set the input layout
    _g_pImmediateContext->IASetInputLayout( drawContext._renderObject->GetShader()->Getlayout());

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    XMVECTOR cameraPos = GEngine->GetCameraPosition();

    Matrix4 *transform = &drawContext._transform; 
    _cbLine.mWorld = Matrix4ToXMMATRIX(transform, drawContext._sizeX, drawContext._sizeY, drawContext._sizeZ);
    _cbLine.vAnimSources = XMFLOAT4(drawContext._animationPeriod,0,0,0);
    _cbLine.vCameraPosition = XMFLOAT4(cameraPos.x,cameraPos.y,cameraPos.z,cameraPos.w);
    _cbLine.typeColor = XMFLOAT4(drawContext._defaultColor.GetR(), drawContext._defaultColor.GetG(),drawContext._defaultColor.GetB(),drawContext._defaultColor.GetA() );

    _g_pImmediateContext->UpdateSubresource( _g_pConstantBuffer, 0, nullptr, &_cbLine, 0, 0 );
    // Set vertex buffer
    _g_pImmediateContext->IASetVertexBuffers( 0, 1, &(drawContext._renderObject->GetShape()->_g_pVertexBuffer), &stride, &offset );
    // Set index buffer
    _g_pImmediateContext->IASetIndexBuffer( drawContext._renderObject->GetShape()->_g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
    // Set primitive topology
    _g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );

    //
    // Render the line
    //
    _g_pImmediateContext->VSSetShader( drawContext._renderObject->GetShader()->GetVertexShader(), nullptr, 0 );
    _g_pImmediateContext->VSSetConstantBuffers( 0, 1, & _g_pConstantBuffer );
    _g_pImmediateContext->PSSetShader( drawContext._renderObject->GetShader()->GetPixelShader(), nullptr, 0 );
    _g_pImmediateContext->PSSetConstantBuffers( 0, 1, & _g_pConstantBuffer );
    _g_pImmediateContext->OMSetBlendState(_g_pBlendState, 0, 0xffffffff);
    _g_pImmediateContext->Draw(drawContext._renderObject->GetShape()->GetVerticesCount(),0);
  }

  //--------------------------------------------------------------------------------------

  void DX11Draw::RenderIcon(DrawContext &drawContext)
  {
    if(!drawContext._renderObject || 
      !drawContext._renderObject->GetShape() ||
      !drawContext._renderObject->GetShader()
      ) return;


    // Set the input layout
    _g_pImmediateContext->IASetInputLayout( drawContext._renderObject->GetShader()->Getlayout());

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    XMVECTOR cameraPos = GEngine->GetCameraPosition();

    Matrix4 *transform = &drawContext._transform; 
    _cbIcon.mWorld = Matrix4ToXMMATRIX(transform, drawContext._sizeX, drawContext._sizeY, drawContext._sizeZ);
    _cbIcon.vAnimSources = XMFLOAT4(drawContext._animationPeriod,0,0,0);
    _cbIcon.vCameraPosition = XMFLOAT4(cameraPos.x,cameraPos.y,cameraPos.z,cameraPos.w);
    _cbIcon.typeColor = XMFLOAT4(drawContext._defaultColor.GetR(), drawContext._defaultColor.GetG(),drawContext._defaultColor.GetB(),drawContext._defaultColor.GetA() );
    _cbIcon.scale = XMFLOAT4(drawContext._sizeX, drawContext._sizeY, drawContext._sizeZ, 1 );

    _g_pImmediateContext->UpdateSubresource( _g_pConstantBuffer, 0, nullptr, &_cbIcon, 0, 0 );
    // Set vertex buffer
    _g_pImmediateContext->IASetVertexBuffers( 0, 1, &(drawContext._renderObject->GetShape()->_g_pVertexBuffer), &stride, &offset );
    // Set index buffer
    _g_pImmediateContext->IASetIndexBuffer( drawContext._renderObject->GetShape()->_g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
    // Set primitive topology
    _g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

    // Render shape
    _g_pImmediateContext->VSSetShader( drawContext._renderObject->GetShader()->GetVertexShader(), nullptr, 0 );
    _g_pImmediateContext->VSSetConstantBuffers( 0, 1, & _g_pConstantBuffer );
    _g_pImmediateContext->PSSetShader( drawContext._renderObject->GetShader()->GetPixelShader(), nullptr, 0 );
    _g_pImmediateContext->PSSetConstantBuffers( 0, 1, & _g_pConstantBuffer );

    ID3D11ShaderResourceView *texture0 = drawContext._renderObject->GetTexture(0);
    _g_pImmediateContext->PSSetShaderResources( 0, 1, &texture0 );
    ID3D11ShaderResourceView *texture1 = drawContext._renderObject->GetTexture(1);
    _g_pImmediateContext->PSSetShaderResources( 1, 1, &texture1 );
    ID3D11ShaderResourceView *texture2 = drawContext._renderObject->GetTexture(2);
    _g_pImmediateContext->PSSetShaderResources( 2, 1, &texture2 );

    _g_pImmediateContext->PSSetSamplers( 0, 1, &_g_pSamplerLinear );
    _g_pImmediateContext->OMSetBlendState(_g_pBlendState, 0, 0xffffffff);
    _g_pImmediateContext->Draw(drawContext._renderObject->GetShape()->GetVerticesCount(),0);
  }

  //------------------------------------------------------------------------------------

  void DX11Draw::ClearZBuffer()
  {
    // Clear the depth buffer to 1.0 (max depth)
    _g_pImmediateContext->ClearDepthStencilView( _g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
  }

  //--------------------------------------------------------------------------------------
  // Called every time the application receives a message
  //--------------------------------------------------------------------------------------
  LRESULT CALLBACK DX11Draw::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
  {
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
    case WM_PAINT:
      hdc = BeginPaint( hWnd, &ps );
      EndPaint( hWnd, &ps );
      break;

    case WM_DESTROY:
      PostQuitMessage( 0 );
      break;

    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
      if(GEngine && GEngine->GetIGame())
      {
        GEngine->GetIGame()->ProcessControls(message, wParam, lParam );
      }
      break;

    default:
      return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
  }

  //--------------------------------------------------------------------------------------
  // Clean up the objects we've created
  //--------------------------------------------------------------------------------------
  void DX11Draw::CleanupDevice()
  {
    RELEASE_MEM( _g_pImmediateContext ) ;
    RELEASE_MEM( _g_pDepthStencil );
    RELEASE_MEM( _g_pDepthStencilView ) ;
    RELEASE_MEM( _g_pDSState ) ;
    RELEASE_MEM( _g_pRenderTargetView ); 
    RELEASE_MEM( _g_pSwapChain );
    RELEASE_MEM( _g_pImmediateContext ); 
    RELEASE_MEM( _g_pd3dDevice ); 

    RELEASE_MEM(_g_RSwireFrame); 
    RELEASE_MEM(_g_RSSolid);

    RELEASE_MEM(_g_pConstantBuffer);
    RELEASE_MEM(_g_pSamplerLinear);
    RELEASE_MEM(_g_pBlendState);
  }

} // Helios namespace