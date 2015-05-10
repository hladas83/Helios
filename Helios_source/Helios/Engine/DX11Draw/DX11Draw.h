#pragma once

#ifndef _DX11DRAW_TYPE
#define _DX11DRAW_TYPE

//===================================================================================================
// included dependencies

#include <windows.h>
#include "../../common.h"
#include "../../resource.h"
#include "../../Elementary/TaskManager/TaskManager.h"
#include "RenderManager.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class RenderObject;
  class DX11Draw;
  class Task;

  //===================================================================================================
  //!Draw thread parameters
  class DX11DrawParams : public ITaskParametrs
  {
    typedef ITaskParametrs base;

  public:
    DX11Draw *_dX11Draw;

    long _tid;

    HINSTANCE _hInstance;
    HINSTANCE _hPrevInstance;
    LPWSTR _lpCmdLine; 
    int _nCmdShow;

  public:
    DX11DrawParams(DX11Draw *dX11Draw, long tid, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
    virtual ~DX11DrawParams();

    USE_CASTING(base)
  };

  //===================================================================================================
  // Thread task

  class  RenderTask : public Task
  {
    typedef Task base;

  public:
    RenderTask(DX11DrawParams *param) : base(param)  {};
    void virtual Pefrom() override;
  };


  //===================================================================================================
  // Draw class

  class DX11Draw : public SingletonTask<DX11Draw>, public RefCount, public OLinkBase<DX11Draw>
  {

    friend RenderManager;

  //--------------------------------------------------------------------------------------
  // DX11 Render Variables

  private:
    HINSTANCE               _g_hInst;
    HWND                    _g_hWnd;
    D3D_DRIVER_TYPE         _g_driverType;
    D3D_FEATURE_LEVEL       _g_featureLevel;
    ID3D11Device            *_g_pd3dDevice;
    ID3D11DeviceContext     *_g_pImmediateContext;
    IDXGISwapChain          *_g_pSwapChain;
    ID3D11RenderTargetView  *_g_pRenderTargetView;
    ID3D11Texture2D         *_g_pDepthStencil;
    ID3D11DepthStencilView  *_g_pDepthStencilView;
    ID3D11DepthStencilState *_g_pDSState;
    XMMATRIX                _g_World;
    XMMATRIX                _g_View;
    XMMATRIX                _g_Projection;
    ID3D11SamplerState      *_g_pSamplerLinear;
    ID3D11BlendState        *_g_pBlendState;

    ID3D11RasterizerState   *_g_RSwireFrame; 
    ID3D11RasterizerState   *_g_RSSolid;

    ID3D11Buffer           *_g_pConstantBuffer;

    ConstantBufferShape        _cbShape;
    ConstantBufferLine         _cbLine;
    ConstantBufferIcon         _cbIcon;

    //--------------------------------------------------------------------------------------
    // Helios Render Variables

    RenderManager _rendermanager;
    
    // window resolution
    float _width;
    float _height;

  private:

    //-----------------------------------------------------------------------------
    // private init
    HRESULT InitDevice();
    HRESULT InitRenderStates();
    HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
    void CleanupDevice();

    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    int WINAPI wWinMainss( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow );

  public:
    DX11Draw();
    ~DX11Draw();

    //-----------------------------------------------------------------------------
    // init
    bool Init() {return true;}
    bool InitGraphics(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
    
    //-----------------------------------------------------------------------------
    // conver world position screen position 
    Vector3 WorldToScreen(Vector3 position);
    float GetAspectRatio() const {return _width / _height;};

  protected:

    //-----------------------------------------------------------------------------
    // rendering
    void Render();
    void RenderShape(DrawContext &drawContext);
    void RenderLine(DrawContext &drawContext);
    void RenderIcon(DrawContext &drawContext);
    void ClearZBuffer();

  public:
    void RenderObject(DrawContext &drawContext);

    LRESULT CameraMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

    //thread start method
    static  void *TDrawSpaceStart(void *context);
    //draw loop
    void *DrawSpace(int threadid);

    //-----------------------------------------------------------------------------
    // device
    ID3D11Device *GetDevice() const {return _g_pd3dDevice;};
    ID3D11DeviceContext *GetDeviceContext() const {return _g_pImmediateContext;};

    void CleanUp() {};

    USE_CASTING_ROOT;
  };
} // Helios namespace

#endif

