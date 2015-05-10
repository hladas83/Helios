#pragma once

#ifndef  _HELIOS_DX11DRAWTYPES
#define  _HELIOS_DX11DRAWTYPES

//===================================================================================================
// included dependencies

#include "../../common.h"
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>


namespace Helios
{

  //--------------------------------------------------------------------------------------
  // Structures

  struct ConstantBufferShape
  {
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
    XMFLOAT4 vCameraPosition;
    XMFLOAT4 vLightPos[2];
    XMFLOAT4 vLightColor[2];
    XMFLOAT4 vAnimSources;
    XMFLOAT4 vOutputColor;
  };

  //-----------------------------------------------------------------------------
  //line
  struct ConstantBufferLine
  {
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
    XMFLOAT4 vCameraPosition;
    XMFLOAT4 typeColor;
    XMFLOAT4 vAnimSources;
    XMFLOAT4 vOutputColor;
  };

  //-----------------------------------------------------------------------------
  //icon
  struct ConstantBufferIcon
  {
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
    XMFLOAT4 vCameraPosition;
    XMFLOAT4 typeColor;
    XMFLOAT4 scale;
    XMFLOAT4 vAnimSources;
    XMFLOAT4 vOutputColor;
  };

  //-----------------------------------------------------------------------------
  //basic vertex
  struct SimpleVertex
  {
    XMFLOAT3 _pos;
    XMFLOAT3 _normal;
    XMFLOAT2 _texUV;

    SimpleVertex(XMFLOAT3 pos, XMFLOAT3 normal, XMFLOAT2 texUV) : _pos(pos), _normal(normal), _texUV(texUV)  {};
    SimpleVertex(): _pos(0,0,0), _normal(0,0,0), _texUV(0,0) {}
  };

} // Helios namespace


#endif // _HELIOS_DX11DRAWTYPES



